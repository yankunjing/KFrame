﻿#include "KFMatchClientModule.h"
#include "KFMatchClientConfig.h"

namespace KFrame
{

    void KFMatchClientModule::InitModule()
    {
        __KF_ADD_CONFIG__( _kf_match_config, true );
    }

    void KFMatchClientModule::BeforeRun()
    {
        _kf_component = _kf_kernel->FindComponent( __KF_STRING__( player ) );
        _kf_player->RegisterEnterFunction( this, &KFMatchClientModule::OnEnterQueryMatchRoom );
        _kf_player->RegisterLeaveFunction( this, &KFMatchClientModule::OnLeaveCannelMatch );

        _kf_component->RegisterUpdateDataFunction( __KF_STRING__( player ), __KF_STRING__( matchid ), this, &KFMatchClientModule::OnMatchIdUpdateCallBack );
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        __REGISTER_MESSAGE__( KFMsg::MSG_START_MATCH_REQ, &KFMatchClientModule::HandleStartMatchReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_MATCH_TO_CLIENT_ACK, &KFMatchClientModule::HanldeMatchToClientAck );
        __REGISTER_MESSAGE__( KFMsg::MSG_CANCEL_MATCH_REQ, &KFMatchClientModule::HandleCancelMatchReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_QUERY_MATCH_ROOM_ACK, &KFMatchClientModule::HandleQueryMatchRoomAck );
        __REGISTER_MESSAGE__( KFMsg::S2S_NOTICE_MATCH_STATE_REQ, &KFMatchClientModule::HandleNoticeMatchStateAck );
    }

    void KFMatchClientModule::BeforeShut()
    {
        __KF_REMOVE_CONFIG__( _kf_match_config );
        _kf_player->UnRegisterEnterFunction( this );
        _kf_player->UnRegisterLeaveFunction( this );
        _kf_component->UnRegisterUpdateDataFunction( this, __KF_STRING__( player ), __KF_STRING__( matchid ) );
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        __UNREGISTER_MESSAGE__( KFMsg::MSG_START_MATCH_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_MATCH_TO_CLIENT_ACK );
        __UNREGISTER_MESSAGE__( KFMsg::MSG_CANCEL_MATCH_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_QUERY_MATCH_ROOM_ACK );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool KFMatchClientModule::SendMessageToMatch( uint32 msgid, ::google::protobuf::Message* message )
    {
        return _kf_cluster->SendToShard( __KF_STRING__( match ), msgid, message );
    }

    uint32 KFMatchClientModule::GetMatchMaxCount( uint32 matchid )
    {
        // 判断匹配是否存在
        auto kfsetting = _kf_match_config->FindMatchSetting( matchid );
        if ( kfsetting == nullptr )
        {
            return 1;
        }

        return kfsetting->_max_group_player_count;
    }

    __KF_MESSAGE_FUNCTION__( KFMatchClientModule::HandleStartMatchReq )
    {
        __CLIENT_PROTO_PARSE__( KFMsg::MsgStartMatchReq );

        // 测试服务器指定战场ip
        uint64 battleserverid = _invalid_int;
        if ( KFGlobal::Instance()->_app_id._union._app_data._channel_id == KFMsg::Internal )
        {
            if ( kfmsg.has_battleserverid() )
            {
                battleserverid = kfmsg.battleserverid();
            }
        }

        // 开始匹配
        auto result = ProcessStartMatch( player, kfmsg.matchid(), kfmsg.allowgroup(), battleserverid, kfmsg.version() );
        if ( result != KFMsg::MatchRequestSuccess )
        {
            _kf_display->SendToGroup( player, result );
        }
    }

    bool KFMatchClientModule::IsAllGroupMemberPrepare( KFData* kfmemberrecord )
    {
        auto kfmember = kfmemberrecord->FirstData();
        while ( kfmember != nullptr )
        {
            auto isparpre = kfmember->GetValue( __KF_STRING__( prepare ) );
            if ( isparpre == _invalid_int )
            {
                return false;
            }

            kfmember = kfmemberrecord->NextData();
        }

        return true;
    }

    uint32 KFMatchClientModule::ProcessStartMatch( KFEntity* player, uint32 matchid, bool allowgroup, uint64 battleserverid, const std::string& version )
    {
        // 判断匹配是否存在
        auto kfsetting = _kf_match_config->FindMatchSetting( matchid );
        if ( kfsetting == nullptr )
        {
            return KFMsg::MatchCanNotFindMatch;
        }

        auto kfobject = player->GetData();

        // 正在房间中
        auto roomid = kfobject->GetValue( __KF_STRING__( roomid ) );
        if ( roomid != _invalid_int )
        {
            __LOG_ERROR__( "player[{}] match already in battle[{}]", player->GetKeyID(), roomid );
            _kf_battle->QueryBattleRoom( player->GetKeyID(), roomid );
            return KFMsg::MatchRequestSuccess;
        }

        // 是否正在匹配中
        auto waitmatchid = kfobject->GetValue( __KF_STRING__( matchid ) );
        if ( waitmatchid != _invalid_int )
        {
            return KFMsg::MatchAlreadyWait;
        }

        auto kfgroup = kfobject->FindData( __KF_STRING__( group ) );
        auto kfmemberrecord = kfgroup->FindData( __KF_STRING__( groupmember ) );

        // 判断该模式的人数限定
        auto membercount = __MAX__( kfmemberrecord->Size(), 1 );
        if ( kfsetting->_min_group_player_count > membercount ||
                kfsetting->_max_group_player_count < membercount )
        {
            return KFMsg::MatchGroupPlayerLimit;
        }

        // 如果有队伍
        auto groupid = kfgroup->GetValue( __KF_STRING__( id ) );
        if ( groupid != _invalid_int )
        {
            // 队长才能匹配
            auto captainid = kfgroup->GetValue( __KF_STRING__( captainid ) );
            if ( captainid != player->GetKeyID() )
            {
                return KFMsg::GroupNotCaption;
            }

            // 判断是否都准备
            if ( !IsAllGroupMemberPrepare( kfmemberrecord ) )
            {
                return KFMsg::MatchMustPrepare;
            }
        }

        // 发送给匹配集群， 进行匹配
        KFMsg::S2SMatchToProxyReq req;
        req.set_matchid( matchid );
        req.set_allowgroup( allowgroup );
        req.set_playerid( player->GetKeyID() );
        req.set_serverid( KFGlobal::Instance()->_app_id._union._id );
        req.set_battleserverid( battleserverid );
        req.set_version( version );
        FormatMatchGroup( player, req.mutable_pbgroup() );
        auto ok = SendMessageToMatch( KFMsg::S2S_MATCH_TO_PROXY_REQ, &req );
        if ( !ok )
        {
            return KFMsg::MatchClusterBusy;
        }

        // 设置正在等待匹配中, 防止客户端请求多次
        kfobject->SetValue( __KF_STRING__( matchid ), matchid );
        return KFMsg::MatchRequestSuccess;
    }

    void KFMatchClientModule::FormatMatchGroup( KFEntity* player, KFMsg::PBMatchGroup* pbgroup )
    {
        auto kfobject = player->GetData();
        auto groupid = kfobject->GetValue( __KF_STRING__( group ), __KF_STRING__( id ) );
        if ( groupid == _invalid_int )
        {
            groupid = kfobject->GetKeyID();
        }
        pbgroup->set_groupid( groupid );

        // 自己的属性
        FormatMatchPlayer( kfobject, pbgroup->add_pbplayer() );

        // 队员的属性
        auto kfmemberrecord = kfobject->FindData( __KF_STRING__( group ), __KF_STRING__( groupmember ) );
        auto kfmember = kfmemberrecord->FirstData();
        while ( kfmember != nullptr )
        {
            FormatMatchPlayer( kfmember, pbgroup->add_pbplayer() );
            kfmember = kfmemberrecord->NextData();
        }
    }

    void KFMatchClientModule::FormatMatchPlayer( KFData* kfobject, KFMsg::PBBattlePlayer* pbplayer )
    {
        auto kfbasic = kfobject->FindData( __KF_STRING__( basic ) );
        pbplayer->set_serverid( kfbasic->GetValue( __KF_STRING__( serverid ) ) );
        pbplayer->set_playerid( kfbasic->GetValue( __KF_STRING__( id ) ) );

        pbplayer->set_sex( kfbasic->GetValue( __KF_STRING__( sex ) ) );
        pbplayer->set_grading( kfbasic->GetValue( __KF_STRING__( grading ) ) );
        pbplayer->set_groupid( kfbasic->GetValue( __KF_STRING__( groupid ) ) );
        pbplayer->set_name( kfbasic->GetValue< std::string >( __KF_STRING__( name ) ) );
        pbplayer->set_icon( kfbasic->GetValue< std::string >( __KF_STRING__( icon ) ) );
        pbplayer->set_iconbox( kfbasic->GetValue< std::string >( __KF_STRING__( iconbox ) ) );

        // 模型, 时装
        pbplayer->set_modelid( kfobject->GetValue( __KF_STRING__( modelid ) ) );
        pbplayer->set_clothesid( kfobject->GetValue( __KF_STRING__( clothesid ) ) );

        // 成就信息
        auto kfachieves = kfobject->FindData( __KF_STRING__( achieve ) );
        _kf_achieve->FormatBattleAchieve( kfachieves, pbplayer->mutable_achieve() );
    }

    __KF_MESSAGE_FUNCTION__( KFMatchClientModule::HandleCancelMatchReq )
    {
        __CLIENT_PROTO_PARSE__( KFMsg::MsgCancelMatchReq );

        auto kfobject = player->GetData();
        auto matchid = kfobject->GetValue( __KF_STRING__( matchid ) );
        if ( matchid == _invalid_int )
        {
            return _kf_display->SendToClient( player, KFMsg::MatchNotInMatch );
        }

        auto roomid = kfobject->GetValue( __KF_STRING__( roomid ) );
        if ( roomid != _invalid_int )
        {
            return _kf_display->SendToClient( player, KFMsg::MatchCancelInBattle );
        }

        UpdateMatchStateToGroup( player, _invalid_int );
        _kf_display->SendToGroup( player, KFMsg::MatchCancelSuccess );
        player->UpdateData( __KF_STRING__( matchid ), KFOperateEnum::Set, _invalid_int );

        // 发送取消到匹配集群中
        KFMsg::S2SCancelMatchToProxyReq req;
        req.set_matchid( matchid );
        req.set_playerid( playerid );
        SendMessageToMatch( KFMsg::S2S_CANCEL_MATCH_TO_PROXY_REQ, &req );

        __LOG_DEBUG__( "player[{}] cancel match[{}]!", player->GetKeyID(), matchid );
    }

    __KF_MESSAGE_FUNCTION__( KFMatchClientModule::HanldeMatchToClientAck )
    {
        __SERVER_PROTO_PARSE__( KFMsg::S2SMatchToClientAck );

        _kf_display->SendToGroup( player, kfmsg.result() );

        auto kfobject = player->GetData();
        kfobject->SetValue( __KF_STRING__( matchid ), _invalid_int );

        if ( kfmsg.result() == KFMsg::MatchRequestSuccess )
        {
            UpdateMatchStateToGroup( player, kfmsg.matchid() );
            player->UpdateData( __KF_STRING__( matchid ), KFOperateEnum::Set, kfmsg.matchid() );
        }
    }

    void KFMatchClientModule::UpdateMatchStateToGroup( KFEntity* player, uint32 matchid )
    {
        KFMsg::S2SNoticeMatchStateReq req;
        req.set_matchid( matchid );
        _kf_player->SendToGroup( player, KFMsg::S2S_NOTICE_MATCH_STATE_REQ, &req, false );
    }

    __KF_MESSAGE_FUNCTION__( KFMatchClientModule::HandleNoticeMatchStateAck )
    {
        __SERVER_PROTO_PARSE__( KFMsg::S2SNoticeMatchStateReq );

        player->UpdateData( __KF_STRING__( matchid ), KFOperateEnum::Set, kfmsg.matchid() );
        __LOG_DEBUG__( "player[{}] update match[{}]!", player->GetKeyID(), kfmsg.matchid() );
    }

    void KFMatchClientModule::OnEnterQueryMatchRoom( KFEntity* player )
    {
        auto kfobject = player->GetData();
        auto matchid = kfobject->GetValue( __KF_STRING__( matchid ) );
        if ( matchid == _invalid_int )
        {
            return;
        }

        // roomid不为0, 在battleclient里处理
        auto roomid = kfobject->GetValue( __KF_STRING__( roomid ) );
        if ( roomid != _invalid_int )
        {
            return;
        }

        KFMsg::S2SQueryMatchRoomReq req;
        req.set_matchid( matchid );
        req.set_playerid( player->GetKeyID() );
        req.set_serverid( KFGlobal::Instance()->_app_id._union._id );
        SendMessageToMatch( KFMsg::S2S_QUERY_MATCH_ROOM_REQ, &req );
    }

    void KFMatchClientModule::OnLeaveCannelMatch( KFEntity* player )
    {
        auto kfobject = player->GetData();

        // 不在匹配
        auto matchid = kfobject->GetValue( __KF_STRING__( matchid ) );
        if ( matchid == _invalid_int )
        {
            return;
        }

        // 已经在战场中, 不能取消
        auto roomid = kfobject->GetValue( __KF_STRING__( roomid ) );
        if ( roomid != _invalid_int )
        {
            return;
        }

        // 在队伍中也不能取消
        auto groupid = kfobject->GetValue( __KF_STRING__( group ), __KF_STRING__( id ) );
        if ( groupid != _invalid_int )
        {
            return;
        }

        // 发送取消到匹配集群中
        KFMsg::S2SCancelMatchToProxyReq req;
        req.set_matchid( matchid );
        req.set_playerid( player->GetKeyID() );
        SendMessageToMatch( KFMsg::S2S_CANCEL_MATCH_TO_PROXY_REQ, &req );

        player->UpdateData( __KF_STRING__( matchid ), KFOperateEnum::Set, _invalid_int );
        __LOG_DEBUG__( "player[{}] leave cancel match[{}]!", player->GetKeyID(), matchid );
    }

    __KF_MESSAGE_FUNCTION__( KFMatchClientModule::HandleQueryMatchRoomAck )
    {
        __SERVER_PROTO_PARSE__( KFMsg::S2SQueryMatchRoomAck );

        player->UpdateData( __KF_STRING__( matchid ), KFOperateEnum::Set, kfmsg.matchid() );
        __LOG_DEBUG__( "player[{}] query match[{}]!", player->GetKeyID(), kfmsg.matchid() );
    }

    __KF_UPDATE_DATA_FUNCTION__( KFMatchClientModule::OnMatchIdUpdateCallBack )
    {
        auto kfobject = player->GetData();
        auto kfbasic = kfobject->FindData( __KF_STRING__( basic ) );

        if ( newvalue != _invalid_int )
        {
            player->UpdateData( kfbasic, __KF_STRING__( status ), KFOperateEnum::Set, KFMsg::MatchStatus );
        }
        else
        {
            auto groupid = kfobject->GetValue( __KF_STRING__( group ), __KF_STRING__( id ) );
            if ( groupid != _invalid_int )
            {
                player->UpdateData( kfbasic, __KF_STRING__( status ), KFOperateEnum::Set, KFMsg::GroupStatus );
            }
            else
            {
                player->UpdateData( kfbasic, __KF_STRING__( status ), KFOperateEnum::Set, KFMsg::OnlineStatus );
            }
        }
    }
}