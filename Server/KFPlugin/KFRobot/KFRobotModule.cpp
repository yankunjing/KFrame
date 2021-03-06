﻿#include "KFRobotModule.h"
#include "KFRobotConfig.h"
#include "KFStateManage.h"

namespace KFrame
{
    KFRobotModule::KFRobotModule()
    {
        _robot_count = 0;
        _kf_component = nullptr;
        _net_client = nullptr;
    }

    KFRobotModule::~KFRobotModule()
    {
        __KF_DELETE__( _net_client );
    }

    void KFRobotModule::InitModule()
    {
        __KF_ADD_CONFIG__( _kf_robot_config, true );
    }

    void KFRobotModule::BeforeRun()
    {
        //  __REGISTER_RUN_FUNCTION__( &KFRobotModule::Run );
        _net_client = __KF_NEW__( KFNetClientEngine );
        _net_client->InitEngine( 10000 );
        _net_client->BindNetFunction( this, &KFRobotModule::HandleNetMessage );
        _net_client->BindConnectFunction( this, &KFRobotModule::OnClientConnected );
        _net_client->BindDisconnectFunction( this, &KFRobotModule::OnClientDisconnect );

        _kf_state_manage->Initialize();
        _kf_component = _kf_kernel->FindComponent( __KF_STRING__( player ) );

        _kf_plugin_manage->RegisterCommandFunction( "state", this, &KFRobotModule::ChangeState );

        KFRobotPolicMgr::Instance()->Init();
        KFRobotPolicMgr::Instance()->InitStateList( _kf_robot_config->_oddStateList, _kf_robot_config->_evenStateList );
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        __REGISTER_MESSAGE__( KFMsg::MSG_LOGIN_VERIFY_ACK, &KFRobotModule::HandleLoginVerifyAck );
        __REGISTER_MESSAGE__( KFMsg::MSG_LOGIN_ENTER_GAME, &KFRobotModule::HandleEnterGame );
        __REGISTER_MESSAGE__( KFMsg::MSG_RESULT_DISPLAY, &KFRobotModule::HandleResultDisplay );
        __REGISTER_MESSAGE__( KFMsg::MSG_SYNC_UPDATE_DATA, &KFRobotModule::HandleUpdateData );
        __REGISTER_MESSAGE__( KFMsg::MSG_SYNC_ADD_DATA, &KFRobotModule::HandleAddData );
        __REGISTER_MESSAGE__( KFMsg::MSG_SYNC_REMOVE_DATA, &KFRobotModule::HandleRemoveData );
        __REGISTER_MESSAGE__( KFMsg::MSG_SEND_CHAT_INFO, &KFRobotModule::HandleDisplayChatInfo );
        __REGISTER_MESSAGE__( KFMsg::MSG_TELL_BE_KICK, &KFRobotModule::HandleBeKick );
        __REGISTER_MESSAGE__( KFMsg::MSG_TELL_QUERY_PLAYER, &KFRobotModule::HandleQueryPlayAck );
        //__REGISTER_MESSAGE__( KFMsg::MSG_SEND_FRIEND_CHAT_INFO, &KFRobotModule::HandleDisplayFriendChat );
        __REGISTER_MESSAGE__( KFMsg::MSG_QUERY_GUEST_ACK, &KFRobotModule::HandleQueryGuestAck );
        __REGISTER_MESSAGE__( KFMsg::MSG_SHOW_REWARD_AGENT, &KFRobotModule::HandleShowReward );
        __REGISTER_MESSAGE__( KFMsg::MSG_CANCEL_MATCH_ACK, &KFRobotModule::HandleCancleMatch );
        __REGISTER_MESSAGE__( KFMsg::MSG_QUERY_RANK_LIST_ACK, &KFRobotModule::HandleWholeRankDisplay );
        __REGISTER_MESSAGE__( KFMsg::MSG_QUERY_GUILD_LOG_ACK, &KFRobotModule::HandleQueryGuildLogAck );

        __REGISTER_MESSAGE__( KFMsg::MSG_SYNC_UPDATE_GUILD_DATA, &KFRobotModule::HandleUpdateGuildData );
        __REGISTER_MESSAGE__( KFMsg::MSG_SYNC_ADD_GUILD_DATA, &KFRobotModule::HandleAddGuildData );
        __REGISTER_MESSAGE__( KFMsg::MSG_SYNC_REMOVE_GUILD_DATA, &KFRobotModule::HandleRemoveGuildData );

        __REGISTER_MESSAGE__( KFMsg::MSG_QUERY_GUILD_LIST_ACK, &KFRobotModule::HandleQueryGuildListAck );
        __REGISTER_MESSAGE__( KFMsg::MSG_TELL_QUERY_GUILD, &KFRobotModule::HandleQueryGuildAck );
    }

    void KFRobotModule::BeforeShut()
    {
        __KF_REMOVE_CONFIG__( _kf_robot_config );
        __UNREGISTER_TIMER__();
        //__UNREGISTER_RUN_FUNCTION__();
    }

    void KFRobotModule::OnceRun()
    {
        _robot_count = _kf_robot_config->_start_count;
        __REGISTER_LIMIT_TIMER__( 0, _kf_robot_config->_login_span_time, _kf_robot_config->_robot_count, &KFRobotModule::OnTimerCreateRobot );
    }

    void KFRobotModule::Run()
    {
        auto kfrobot = _robot_list.First();

        while ( kfrobot != nullptr )
        {
            _net_client->RunEngine( KFGlobal::Instance()->_game_time );

            kfrobot->Run();
            kfrobot = _robot_list.Next();
        }
    }
    void KFRobotModule::ChangeState( const VectorString& param )
    {
        if ( param.size() < 2 )
        {
            return;
        }

        auto kfrobot = _robot_list.First();

        while ( kfrobot != nullptr )
        {
            kfrobot->ChangeState( atoi( param[ 1 ].c_str() ) );
            kfrobot = _robot_list.Next();
        }
    }



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define __ROBOT_PROTO_PARSE__( msgtype ) \
    auto robotid = __KF_HEAD_ID__( kfid );\
    auto kfrobot = _robot_list.Find( robotid );\
    if ( kfrobot == nullptr )\
    {\
        __LOG_ERROR__("can't find robot[{}]!",\
                      robotid);\
    }\
    __PROTO_PARSE__( msgtype );


    __KF_TIMER_FUNCTION__( KFRobotModule::OnTimerCreateRobot )
    {
        ++_robot_count;
        uint32 robotid = _kf_robot_config->_account_id_base + _robot_count;

        auto kfrobot = _robot_list.Create( robotid );
        kfrobot->Initialize( robotid, _net_client, _kf_component );
    }

    void KFRobotModule::HandleNetMessage( const KFId& kfid, uint32 msgid, const char* data, uint32 length )
    {
        bool handleresult = _kf_message->CallFunction( kfid, msgid, data, length );

        if ( handleresult )
        {
            return;
        }

        auto robotid = __KF_HEAD_ID__( kfid );
        __LOG_INFO__( "[{}] robot[{}] recv msgid[{}]!",
                      __FUNCTION__, robotid, msgid );
    }

    // 连接断开
    __KF_CLIENT_LOST_FUNCTION__( KFRobotModule::OnClientDisconnect )
    {
        __LOG_ERROR__( "[{}] robot[{}] disconect!",
                       __FUNCTION__, serverid );
        auto kfrobot = _robot_list.Find( serverid );

        if ( kfrobot == nullptr )
        {
            __LOG_ERROR__( "No Find Robot at [{}] robotid:{}",
                           __FUNCTION__, serverid );
            return;
        }

        kfrobot->DisconnectServer( serverid );
        kfrobot->ChangeState( RobotStateEnum::AuthState );

    }

    // 连接成功
    __KF_CLIENT_CONNECT_FUNCTION__( KFRobotModule::OnClientConnected )
    {
        auto kfrobot = _robot_list.Find( serverid );

        if ( kfrobot == nullptr )
        {
            __LOG_ERROR__( "No Find Robot at [{}] robotid:{}", __FUNCTION__, serverid );
            return;
        }

        kfrobot->OnConnectSendMessage();
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleLoginVerifyAck )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgLoginVerifyAck );

        kfrobot->_playerid = kfmsg.playerid();

        //kfrobot->_state_list = KFRobotPolicMgr::Instance()->GetStateListById( kfmsg.playerid() );
        kfrobot->_connect_token = kfmsg.token();
        kfrobot->_connect_ip = kfmsg.ip();
        kfrobot->_connect_port = kfmsg.port();
        _net_client->CloseClient( robotid, __FUNCTION__, __LINE__ );

        __LOG_INFO__( "[{}] robot[{}] login [{}:{}] verity ok!",
                      __FUNCTION__, robotid, kfmsg.ip().c_str(), kfmsg.port() );
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleEnterGame )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgEnterGame );
        //std::cout << "HandleEnterGame : " << kfmsg.DebugString() << std::endl;
        // 初始化机器人状态机

        auto playerdata = kfmsg.mutable_playerdata();
        auto playerid = playerdata->key();
        kfrobot->_playerid = playerid;
        KFRobotPolicMgr::Instance()->PushSortRoleInfo( kfrobot->_playerid );
        kfrobot->_state_list = KFRobotPolicMgr::Instance()->GetStateListById( playerid );

        auto player = _kf_component->CreateEntity( kfrobot->_playerid, playerdata );

        // 这边判断名字是否为空 决定切换的状态
        auto kfobject = player->GetData();
        auto kfbasic = kfobject->FindData( __KF_STRING__( basic ) );

        if ( nullptr == kfbasic )
        {
            return;
        }


        // 没有名字先创角
        auto modleid = kfbasic->GetValue< uint32 >( __KF_STRING__( modelid ) );

        if ( modleid == _invalid_int )
        {
            kfrobot->ChangeState( RobotStateEnum::CreateRole );
        }

        else
        {
            kfrobot->ChangeState( kfrobot->_state_list->_state );
        }

        KFRobotPolicMgr::Instance()->AddRoleInfo( kfrobot->_playerid, player );
        __LOG_INFO__( "[{}] robot[{}] enter game!", __FUNCTION__, robotid );

    }

    //__KF_MESSAGE_FUNCTION__( KFRobotModule::HandleLoginGameAck )
    //{
    //	__ROBOT_PROTO_PARSE__( KFMsg::MsgLoginGameAck );

    //	kfrobot->ChangeState( RobotStateEnum::AuthState );
    //}

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleResultDisplay )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgResultDisplay );

        __LOG_INFO__( "[{}] robot[{}] display result[{}]!", __FUNCTION__, robotid, kfmsg.result() );

        switch ( kfmsg.result() )
        {
        case KFMsg::CreateRoleOK:
        case KFMsg::CreateRoleAlready:
        {
            kfrobot->ChangeState( kfrobot->_state_list->_state );
        }
        break;

        case KFMsg::LoadDataFailed:
        {
            kfrobot->ChangeState( RobotStateEnum::AuthState );
            kfrobot->DisconnectServer( 2 );

        }

        case KFMsg::NameAlreadyExist:
        case KFMsg::NameEmpty:
        {
            auto name = __FORMAT__( "{}:{}:{}", __KF_STRING__( robot ), _kf_robot_config->_server_channel, kfrobot->_account.c_str() );

            KFMsg::MsgCreateRoleReq req;
            req.set_name( name );
            req.set_sex( 1 );
            req.set_model( 1 );
            kfrobot->SendNetMessage( KFMsg::MSG_CREATE_ROLE_REQ, &req );
        }
        break;

        case KFMsg::MatchRequestSuccess:
        {
            if ( kfrobot->_state_list->next && RobotStateEnum::CancelMatch == kfrobot->_state_list->next->_state )
            {
                kfrobot->ChangeStateProxy();
            }

            else
            {
                __LOG_INFO__( "[{}] robot[{}] start match success! waitting...", __FUNCTION__, robotid );
            }
        }
        break;

        default:
            break;
        }
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleUpdateData )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgSyncUpdateData );

        std::cout << "HandleUpdateData: " << kfmsg.DebugString() << std::endl;
        auto pbdata = kfmsg.mutable_pbdata();
        auto player = _kf_component->FindEntity( kfrobot->_playerid, __FUNCTION__, __LINE__ );

        if ( nullptr == player )
        {
            return;
        }

        auto kfobject = player->GetData();
        auto key = pbdata->key();
        kfrobot->ParseUpdatePB2KFData( player, pbdata, kfobject, key );
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleAddData )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgSyncAddData );
        std::cout << "HandleAddData: " << kfmsg.DebugString() << std::endl;
        auto pbdata = kfmsg.mutable_pbdata();
        auto player = _kf_component->FindEntity( kfrobot->_playerid, __FUNCTION__, __LINE__ );

        if ( nullptr == player )
        {
            return;
        }

        _kf_kernel->ParseFromProto( player->GetData(), pbdata );

        auto kfobject = player->GetData();

        if (  pbdata->pbrecord_size() > 0 )
        {
            auto record = &pbdata->pbrecord( 0 );

            if ( __KF_STRING__( friendinvite ) == record->name() )
            {
                kfrobot->AgreeInvite();
            }
        }
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleRemoveData )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgSyncRemoveData );
        //std::cout << "HandleRemoveData: " << kfmsg.DebugString() << std::endl;
        auto player = _kf_component->FindEntity( kfrobot->_playerid, __FUNCTION__, __LINE__ );

        if ( nullptr == player )
        {
            return;
        }

        uint64 key = 0;
        auto kfobject = player->GetData();
        auto pbdata = kfmsg.mutable_pbdata();
        kfrobot->ParsePB2KFData( player, pbdata, kfobject, key );
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleDisplayChatInfo )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgSendChatInfo );
        __LOG_INFO__( "[{}] robot[{}] recv chat[{}]!",
                      __FUNCTION__, robotid, kfmsg.chatinfo().c_str() );
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleBeKick )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgTellBeKick );

        __LOG_INFO__( "[{}] robot[{}] be kick[{}]!",
                      __FUNCTION__, robotid, kfmsg.type() );

        auto ok = _kf_component->RemoveEntity( kfrobot->_playerid );

        if ( ok )
        {
            KFRobotPolicMgr::Instance()->RemoveRole( kfrobot->_playerid );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleDisplayFriendChat )
    {
        //__ROBOT_PROTO_PARSE__( KFMsg::MsgSendFriendChatInfo );

    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleQueryPlayAck )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgTellQueryPlayer );

        __LOG_INFO__( "robot[{}] query player ok!", robotid	);
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleWholeRankDisplay )
    {
        //__ROBOT_PROTO_PARSE__( KFMsg::MsgQueryRankListAck );
        __PROTO_PARSE__( KFMsg::MsgQueryRankListAck );
        //	std::cout << "HandleWholeRankDisplay: " << kfmsg.DebugString() << std::endl;
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleQueryGuildLogAck )
    {
        //__ROBOT_PROTO_PARSE__( KFMsg::MsgQueryRankListAck );
        __PROTO_PARSE__( KFMsg::MsgQueryGuildLogAck );
        // std::cout << "HandleQueryGuildLogAck: " << kfmsg.DebugString() << std::endl;
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleQueryGuestAck )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgQueryGuestAck );

        __LOG_INFO__( "[{}] robot[{}] query guest ok!", __FUNCTION__, robotid );
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleShowReward )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgShowRewardAgent );

        __LOG_INFO__( "[{}] robot[{}] show reward[{}]!",
                      __FUNCTION__, robotid, kfmsg.reward().c_str() );
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleCancleMatch )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgCancelMatchAck );

        // 防止连续切换
        if ( _invalid_int != kfmsg.matchid() && kfrobot->_matchid == kfmsg.matchid() )
        {
            kfrobot->_matchid = _invalid_int;
            kfrobot->ChangeStateProxy();
        }
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleUpdateGuildData )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgSyncUpdateGuildData );
        std::cout << "HandleUpdateGuildData:" << kfmsg.DebugString() << std::endl;
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleAddGuildData )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgSyncAddGuildData );
        std::cout << "HandleAddGuildData:" << kfmsg.DebugString() << std::endl;
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleRemoveGuildData )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgSyncRemoveGuildData );
        std::cout << "HandleRemoveGuildData:" << kfmsg.DebugString() << std::endl;
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleQueryGuildListAck )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgQueryGuildListAck );
        std::cout << "HandleQueryGuildListAck:" << kfmsg.DebugString() << std::endl;
        //添加帮派列表
        for ( auto i = 0; i < kfmsg.guilddatas_size(); ++i )
        {
            auto pbguilddatas = kfmsg.guilddatas( i );
            for ( auto j = 0; j < pbguilddatas.pbstring_size(); ++j )
            {
                auto pbstring = pbguilddatas.pbstring( j );
                if ( pbstring.name() == __KF_STRING__( id ) )
                {
                    auto guildlistdata = __KF_CREATE__( GuildListData );
                    auto guildid = KFUtility::ToValue<uint64>( pbstring.value() );
                    guildlistdata->guildid = guildid;
                    kfrobot->_guild_list.Insert( guildid, guildlistdata );
                }
            }
        }
    }

    __KF_MESSAGE_FUNCTION__( KFRobotModule::HandleQueryGuildAck )
    {
        __ROBOT_PROTO_PARSE__( KFMsg::MsgTellQueryGuild );
        std::cout << "HandleQueryGuildAck:" << kfmsg.DebugString() << std::endl;

        auto guilddata = &kfmsg.guild();
        kfrobot->QueryGuildAck( guilddata );

    }

}


