﻿#include "KFBattleShardModule.h"
#include "KFProtocol/KFProtocol.h"
#include "KFBattleManage.h"
#include "KFBattleConfig.h"

namespace KFrame
{
    void KFBattleShardModule::InitModule()
    {
        __KF_ADD_CONFIG__( _kf_battle_config, true );
    }

    void KFBattleShardModule::BeforeRun()
    {
        __REGISTER_SERVER_DISCOVER_FUNCTION__( &KFBattleShardModule::OnServerDiscoverBattleProxy );
        ///////////////////////////////////////////////////////////////////////////////
        __REGISTER_MESSAGE__( KFMsg::S2S_REGISTER_SERVER_TO_BATTLE_SHARD_REQ, &KFBattleShardModule::HandleRegisterServerToBattleShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_TELL_BATTLE_REGISTER_TO_SHARD_REQ, &KFBattleShardModule::HandleTellBattleRegisterToShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_DISCONNECT_SERVER_TO_BATTLE_SHARD_REQ, &KFBattleShardModule::HandleDisconnectServerToBattleServerReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_CREATE_ROOM_TO_BATTLE_SHARD_REQ, &KFBattleShardModule::HandleCreateRoomToBattleShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_OPEN_BATTLE_ROOM_TO_SHARD_ACK, &KFBattleShardModule::HandleOpenBattleRoomToShardAck );
        __REGISTER_MESSAGE__( KFMsg::S2S_PLAYER_ENTER_ROOM_TO_BATTLE_SHARD_ACK, &KFBattleShardModule::HandlePlayerEnterRoomToBattleShardAck );
        __REGISTER_MESSAGE__( KFMsg::S2S_PLAYER_LEAVE_ROOM_TO_BATTLE_SHARD_REQ, &KFBattleShardModule::HandlePlayerLeaveRoomToBattleShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_TELL_ROOM_START_TO_BATTLE_SHARD_REQ, &KFBattleShardModule::HandleTellRoomStartToBattleShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_TELL_ROOM_FINISH_TO_BATTLE_SAHRD_REQ, &KFBattleShardModule::HandleTellRoomFinishToBattleShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_NOTICE_MATCH_ROOM_ACK, &KFBattleShardModule::HandleNoticeMatchRoomAck );
        __REGISTER_MESSAGE__( KFMsg::S2S_QUERY_ROOM_TO_BATTLE_SHARD_REQ, &KFBattleShardModule::HandleQueryRoomToBattleShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_ADD_CAMP_TO_BATTLE_SHARD_REQ, &KFBattleShardModule::HandleAddCampToBattleShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_CANCEL_MATCH_TO_BATTLE_SHARD_REQ, &KFBattleShardModule::HandleCancelMatchToBattleShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_OPEN_ROOM_TO_BATTLE_SHARD_ACK, &KFBattleShardModule::HandleOpenRoomToBattleShardAck );
        __REGISTER_MESSAGE__( KFMsg::S2S_PLAYER_LOGIN_ROOM_TO_BATTLE_SHARD_REQ, &KFBattleShardModule::HandlePlayerLoginRoomToBattleShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_TELL_ROOM_START_TO_BATTLE_SHARD_ACK, &KFBattleShardModule::HandleTellRoomStartToBattleShardAck );
        __REGISTER_MESSAGE__( KFMsg::S2S_PLAYER_ONLINE_TO_BATTLE_SHARD_REQ, &KFBattleShardModule::HandlePlayerOnlineToBattleShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_PLAYER_BATTLE_SCORE_ACK, &KFBattleShardModule::HandlePlayerBattleScoreAck );
        __REGISTER_MESSAGE__( KFMsg::S2S_BATTLE_SCORE_BALANCE_TO_SHARD_REQ, &KFBattleShardModule::HandleBattleScoreBalanceToShardReq );
        __REGISTER_MESSAGE__( KFMsg::S2S_ONLINE_QUERY_BATTLE_SCORE_REQ, &KFBattleShardModule::HandleOnlieQueryBattleScoreReq );
    }

    void KFBattleShardModule::BeforeShut()
    {
        __KF_REMOVE_CONFIG__( _kf_battle_config );
        __UNREGISTER_SERVER_DISCOVER_FUNCTION__();
        //////////////////////////////////////////////////////////////////////////////////////////
        __UNREGISTER_MESSAGE__( KFMsg::S2S_REGISTER_SERVER_TO_BATTLE_SHARD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_TELL_BATTLE_REGISTER_TO_SHARD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_DISCONNECT_SERVER_TO_BATTLE_SHARD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_CREATE_ROOM_TO_BATTLE_SHARD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_OPEN_BATTLE_ROOM_TO_SHARD_ACK );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_PLAYER_ENTER_ROOM_TO_BATTLE_SHARD_ACK );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_PLAYER_LEAVE_ROOM_TO_BATTLE_SHARD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_TELL_ROOM_START_TO_BATTLE_SHARD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_TELL_ROOM_FINISH_TO_BATTLE_SAHRD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_NOTICE_MATCH_ROOM_ACK );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_QUERY_ROOM_TO_BATTLE_SHARD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_ADD_CAMP_TO_BATTLE_SHARD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_CANCEL_MATCH_TO_BATTLE_SHARD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_OPEN_ROOM_TO_BATTLE_SHARD_ACK );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_PLAYER_LOGIN_ROOM_TO_BATTLE_SHARD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_TELL_ROOM_START_TO_BATTLE_SHARD_ACK );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_PLAYER_ONLINE_TO_BATTLE_SHARD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_PLAYER_BATTLE_SCORE_ACK );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_BATTLE_SCORE_BALANCE_TO_SHARD_REQ );
        __UNREGISTER_MESSAGE__( KFMsg::S2S_ONLINE_QUERY_BATTLE_SCORE_REQ );
    }

    void KFBattleShardModule::OnceRun()
    {
        _kf_battle_manage->Initialize();
        _battle_redis_driver = _kf_redis->CreateExecute( __KF_STRING__( battle ) );
    }

    void KFBattleShardModule::Run()
    {
        std::set< KFBattleRoom* > removelist;

        auto kfroom = _kf_room_list.First();

        while ( kfroom != nullptr )
        {
            kfroom->RunRoom();

            // 无效了
            if ( !kfroom->CheckValid() )
            {
                removelist.insert( kfroom );
            }

            kfroom = _kf_room_list.Next();
        }

        // 删除无效的战场
        for ( auto kfroom : removelist )
        {
            kfroom->FreeInValidRoom();
            _kf_room_list.Remove( kfroom->_battle_room_id );
        }
    }

    void KFBattleShardModule::ShutDown()
    {
        auto kfroom = _kf_room_list.First();

        while ( kfroom != nullptr )
        {
            kfroom->FreeInValidRoom();
            kfroom = _kf_room_list.Next();
        }
    }

    void KFBattleShardModule::RemoveBattleRoom( KFBattleRoom* kfroom )
    {
        // 通知proxy
        _kf_cluster_shard->RemoveObjectToProxy( kfroom->_battle_room_id );

        // 结束战场
        kfroom->FinishBattleRoom();

        // 删除房间
        _kf_room_list.Remove( kfroom->_battle_room_id );
    }

    KFBattleRoom* KFBattleShardModule::FindRoom( uint64 roomid, const char* function, uint32 line )
    {
        auto kfroom = _kf_room_list.Find( roomid );
        if ( kfroom == nullptr )
        {
            __LOG_ERROR_FUNCTION__( function, line, "can't find room[{}]!", roomid );
        }

        return kfroom;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    __KF_SERVER_DISCOVER_FUNCTION__( KFBattleShardModule::OnServerDiscoverBattleProxy )
    {
        // 把房间的列表同步到proxy服务器
        std::set< uint64 > roomlist;

        auto kfroom = _kf_room_list.First();
        while ( kfroom != nullptr )
        {
            roomlist.insert( kfroom->_battle_room_id );
            kfroom = _kf_room_list.Next();
        }

        _kf_cluster_shard->AddObjectToProxy( handleid, roomlist );
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleRegisterServerToBattleShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2SRegisterServerToBattleShardReq );
        auto proxyid = __KF_HEAD_ID__( kfid );
        auto strserverid = KFAppID::ToString( kfmsg.serverid() );

        __LOG_DEBUG__( "register version[{}] battle[{}|{}:{}|{}] req!", kfmsg.version(), strserverid, kfmsg.ip(), kfmsg.port(), kfmsg.roomid() );

        if ( kfmsg.serverid() == _invalid_int || kfmsg.ip().empty() || kfmsg.port() == _invalid_int || kfmsg.version().empty() )
        {
            return;
        }

        KFVersion kfversion( kfmsg.version() );
        _kf_battle_manage->RegisterBattleServer( kfmsg.serverid(), proxyid, kfmsg.ip(), kfmsg.port(), kfversion._battle_version );

        KFMsg::S2SRegisterBattleServerAck ack;
        ack.set_serverid( kfmsg.serverid() );
        ack.set_result( KFMsg::Success );
        _kf_cluster_shard->SendToClient( proxyid, kfmsg.serverid(), KFMsg::S2S_REGISTER_BATTLE_SERVER_ACK, &ack );

        __LOG_DEBUG__( "register version[{}] battle[{}|{}:{}|{}] ok!", kfmsg.version(), strserverid, kfmsg.ip(), kfmsg.port(), kfmsg.roomid() );
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleTellBattleRegisterToShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2STellBattleRegisterToShardReq );
        auto proxyid = __KF_HEAD_ID__( kfid );

        auto strserverid = KFAppID::ToString( kfmsg.serverid() );
        __LOG_DEBUG__( "battle[{}|{}:{}] update room[{}] req!",
                       strserverid, kfmsg.ip(), kfmsg.port(), kfmsg.roomid() );

        if ( kfmsg.roomid() != _invalid_int )
        {
            // roomid不为0 断线重连
            auto ok = false;

            auto kfroom = _kf_room_list.Find( kfmsg.roomid() );
            if ( kfroom != nullptr )
            {
                ok = kfroom->UpdateBattleRoom( proxyid, kfmsg.serverid(), kfmsg.ip(), kfmsg.port(), kfmsg.version() );
            }

            if ( !ok )
            {
                // 通知战场服务器重置战场
                KFMsg::S2SResetBattleRoomReq req;
                req.set_roomid( kfmsg.roomid() );
                _kf_cluster_shard->SendToClient( proxyid, kfmsg.serverid(), KFMsg::S2S_RESET_BATTLE_ROOM_REQ, &req );
            }
        }
        else
        {
            // roomid为0, 宕机重启
            auto kfroom = FindBattleRoomByServerId( kfmsg.serverid() );
            if ( kfroom != nullptr )
            {
                if ( kfroom->IsBattleRoomStart() )
                {
                    RemoveBattleRoom( kfroom );
                }
                else
                {
                    kfroom->UpdateBattleRoom( proxyid, kfmsg.serverid(), kfmsg.ip(), kfmsg.port(), kfmsg.version() );
                }
            }

        }

        __LOG_DEBUG__( "battle[{}|{}:{}] update room[{}] ok!", strserverid, kfmsg.ip().c_str(), kfmsg.port(), kfmsg.roomid() );
    }

    KFBattleRoom* KFBattleShardModule::FindBattleRoomByServerId( uint64 serverid )
    {
        if ( serverid == _invalid_int )
        {
            return nullptr;
        }

        auto kfroom = _kf_room_list.First();
        while ( kfroom != nullptr )
        {
            if ( kfroom->_battle_server._server_id == serverid )
            {
                return kfroom;
            }

            kfroom = _kf_room_list.Next();
        }

        return nullptr;
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleDisconnectServerToBattleServerReq )
    {
        __PROTO_PARSE__( KFMsg::S2SDisconnectServerToBattleShardReq );

        _kf_battle_manage->UnRegisterBattleServer( kfmsg.serverid() );

        auto kfroom = FindBattleRoomByServerId( kfmsg.serverid() );
        if ( kfroom != nullptr )
        {
            kfroom->DisconnectBattleRoom();
        }

        __LOG_DEBUG__( "battle[{}:{}] disconnect!", kfmsg.serverid(), KFAppID::ToString( kfmsg.serverid() ) );
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleCreateRoomToBattleShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2SCreateRoomToBattleShardReq );

        // 创建房间
        auto kfroom = _kf_room_list.Find( kfmsg.roomid() );
        if ( kfroom == nullptr )
        {
            kfroom = __KF_CREATE_BATCH__( KFBattleRoom, 100 );
            kfroom->InitRoom( kfmsg.matchid(), kfmsg.roomid(), kfmsg.battleserverid(), kfmsg.maxplayercount(), kfmsg.version() );
            _kf_room_list.Insert( kfmsg.roomid(), kfroom );
        }

        // 设置路由信息
        kfroom->_match_proxy_id = __KF_HEAD_ID__( kfid );
        kfroom->_match_shard_id = kfmsg.matchshardid();

        {
            // 回复确认消息
            KFMsg::S2SCreateRoomToMatchShardAck ack;
            ack.set_matchid( kfmsg.matchid() );
            ack.set_roomid( kfmsg.roomid() );
            ack.set_battleshardid( KFGlobal::Instance()->_app_id._union._id );
            kfroom->SendMessageToMatch( KFMsg::S2S_CREATE_ROOM_TO_MATCH_SHARD_ACK, &ack );
        }

        // 广播给所有的proxy
        _kf_cluster_shard->AddObjectToProxy( kfmsg.roomid() );

        __LOG_DEBUG__( "matchid[{}] room[{}] create ok!", kfmsg.matchid(), kfmsg.roomid() );
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleOpenBattleRoomToShardAck )
    {
        __PROTO_PARSE__( KFMsg::S2SOpenBattleRoomToShardAck );
        __LOG_DEBUG__( "open room[{}] waittime[{}] result[{}]!", kfmsg.roomid(), kfmsg.waittime(), kfmsg.result() );

        auto kfroom = FindRoom( kfmsg.roomid(), __FUNC_LINE__ );
        if ( kfroom == nullptr )
        {
            return;
        }

        if ( kfmsg.result() == KFMsg::BattleEnum::OpenSuccess )
        {
            kfroom->ConfirmOpenBattleRoom( true, kfmsg.waittime() );
        }
        else
        {
            kfroom->ConfirmOpenBattleRoom( false, kfmsg.waittime() );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleOpenRoomToBattleShardAck )
    {
        __PROTO_PARSE__( KFMsg::S2SOpenRoomToBattleShardAck );
        __LOG_DEBUG__( "open match room[{}] ack!", kfmsg.roomid() );

        auto kfroom = FindRoom( kfmsg.roomid(), __FUNC_LINE__ );
        if ( kfroom == nullptr )
        {
            return;
        }

        kfroom->ConfirmOpenMatchRoom();
        __LOG_DEBUG__( "open match room[{}] ok!", kfmsg.roomid() );
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleAddCampToBattleShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2SAddCampToBattleShardReq );

        auto pbcamp = &kfmsg.pbcamp();
        __LOG_DEBUG__( "room[{}] add camp[{}] req!", kfmsg.roomid(), pbcamp->campid() );

        auto kfroom = FindRoom( kfmsg.roomid(), __FUNC_LINE__ );
        if ( kfroom == nullptr )
        {
            // 找不到房间了, 可能是进程重启了, 通知match重置房间
            KFMsg::S2SResetMatchRoomReq req;
            req.set_matchid( kfmsg.matchid() );
            req.set_roomid( kfmsg.roomid() );
            _kf_cluster_shard->SendToClient( __KF_HEAD_ID__( kfid ), kfmsg.matchshardid(), KFMsg::S2S_RESET_MATCH_ROOM_REQ, &req );
            return;
        }

        auto kfcamp = kfroom->AddCamp( pbcamp );
        if ( kfcamp != nullptr )
        {
            __LOG_DEBUG__( "room[{}] add camp[{}] ok!", kfmsg.roomid(), pbcamp->campid() );
        }
        else
        {
            __LOG_ERROR__( "room[{}] add camp[{}] failed!", kfmsg.roomid(), pbcamp->campid() );
        }

        KFMsg::S2SAddCampToMatchShardAck ack;
        ack.set_matchid( kfroom->_match_id );
        ack.set_roomid( kfroom->_battle_room_id );
        ack.set_campid( kfmsg.pbcamp().campid() );
        ack.set_addok( kfcamp != nullptr );
        auto ok = kfroom->SendMessageToMatch( KFMsg::S2S_ADD_CAMP_TO_MATCH_SHARD_ACK, &ack );
        if ( ok )
        {
            __LOG_DEBUG__( "room[{}] add camp[{}] send ok!", kfmsg.roomid(), pbcamp->campid() );
        }
        else
        {
            __LOG_ERROR__( "room[{}] add camp[{}] send failed!", kfmsg.roomid(), pbcamp->campid() );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleCancelMatchToBattleShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2SCancelMatchToBattleShardReq );
        __LOG_DEBUG__( "room[{}] campid[{}] group[{}] cancel match req!", kfmsg.roomid(), kfmsg.campid(), kfmsg.groupid() );
        auto kfroom = _kf_room_list.Find( kfmsg.roomid() );
        if ( kfroom == nullptr )
        {
            return;
        }

        auto ok = kfroom->CancelMatch( kfmsg.campid(), kfmsg.groupid() );
        if ( ok )
        {
            __LOG_DEBUG__( "room[{}] campid[{}] group[{}] cancel match ok!", kfmsg.roomid(), kfmsg.campid(), kfmsg.groupid() );
        }
        else
        {
            __LOG_DEBUG__( "room[{}] campid[{}] group[{}] cancel match failed!", kfmsg.roomid(), kfmsg.campid(), kfmsg.groupid() );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandlePlayerEnterRoomToBattleShardAck )
    {
        __PROTO_PARSE__( KFMsg::S2SPlayerEnterRoomToBattleShardAck );
        __LOG_DEBUG__( "[{}:{}] enter room[{}] ack!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );

        auto kfroom = FindRoom( kfmsg.roomid(), __FUNC_LINE__ );
        if ( kfroom == nullptr )
        {
            return;
        }

        auto ok = kfroom->ConfirmEnterBattleRoom( kfmsg.campid(), kfmsg.playerid() );
        if ( ok )
        {
            __LOG_DEBUG__( "[{}:{}] enter room[{}] ok!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );
        }
        else
        {
            __LOG_ERROR__( "[{}:{}] enter room[{}] failed!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandlePlayerLeaveRoomToBattleShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2SPlayerLeaveRoomToBattleShardReq );
        __LOG_DEBUG__( "[{}:{}] leave room[{}] req!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );

        auto kfroom = FindRoom( kfmsg.roomid(), __FUNC_LINE__ );
        if ( kfroom == nullptr )
        {
            return;
        }

        auto ok = kfroom->LeaveBattleRoom( kfmsg.campid(), kfmsg.playerid() );
        if ( ok )
        {
            __LOG_DEBUG__( "[{}:{}] leave room[{}] ok!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );
        }
        else
        {
            __LOG_ERROR__( "[{}:{}] leave room[{}] failed!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );
        }

        // 回复给战斗服务器
        KFMsg::S2SPlayerLeaveBattleRoomAck ack;
        ack.set_roomid( kfmsg.roomid() );
        ack.set_campid( kfmsg.campid() );
        ack.set_playerid( kfmsg.playerid() );
        kfroom->SendMessageToBattle( KFMsg::S2S_PLAYER_LEAVE_BATTLE_ROOM_ACK, &ack );
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleNoticeMatchRoomAck )
    {
        __PROTO_PARSE__( KFMsg::S2SNoticeMatchRoomAck )
        __LOG_DEBUG__( "[{}:{}] notice battle room[{}] ack!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );

        auto kfroom = FindRoom( kfmsg.roomid(), __FUNC_LINE__ );
        if ( kfroom == nullptr )
        {
            return;
        }

        auto ok = kfroom->NoticeBattleRoom( kfmsg.campid(), kfmsg.playerid() );
        if ( ok )
        {
            __LOG_DEBUG__( "[{}:{}] notice battle room[{}] ok!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );
        }
        else
        {
            __LOG_ERROR__( "[{}:{}] notice battle room[{}] failed!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandlePlayerLoginRoomToBattleShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2SPlayerLoginRoomToBattleShardReq );
        __LOG_DEBUG__( "player[{}] login room[{}] req!", kfmsg.playerid(), kfmsg.roomid() );

        auto kfroom = FindRoom( kfmsg.roomid(), __FUNC_LINE__ );
        if ( kfroom == nullptr )
        {
            return;
        }

        auto ok = kfroom->LoginBattleRoom( kfmsg.campid(), kfmsg.playerid() );
        if ( ok )
        {
            __LOG_DEBUG__( "[{}:{}] login room[{}] ok!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );
        }
        else
        {
            __LOG_ERROR__( "[{}:{}] login room[{}] failed!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleQueryRoomToBattleShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2SQueryRoomToBattleShardReq );
        __LOG_DEBUG__( "player[{}] query room[{}] req!", kfmsg.playerid(), kfmsg.roomid() );

        auto kfroom = _kf_room_list.Find( kfmsg.roomid() );
        if ( kfroom != nullptr )
        {
            auto ok = kfroom->QueryBattleRoom( kfmsg.playerid(), kfmsg.serverid() );
            if ( ok )
            {
                return __LOG_DEBUG__( "player[{}] query room[{}] ok!", kfmsg.playerid(), kfmsg.roomid() );
            }
        }

        KFMsg::S2SQueryBattleRoomAck ack;
        ack.set_playerid( kfmsg.playerid() );
        ack.set_roomid( _invalid_int );
        ack.set_matchid( _invalid_int );
        _kf_cluster_shard->SendToClient( __KF_HEAD_ID__( kfid ), kfmsg.serverid(), KFMsg::S2S_QUERY_BATTLE_ROOM_ACK, &ack );
        __LOG_DEBUG__( "player[{}] not in room[{}]!", kfmsg.playerid(), kfmsg.roomid() );
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleTellRoomStartToBattleShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2STellRoomStartToBattleShardReq );
        __LOG_DEBUG__( "start room[{}] req!", kfmsg.roomid() );

        auto kfroom = FindRoom( kfmsg.roomid(), __FUNC_LINE__ );
        if ( kfroom == nullptr )
        {
            return;
        }

        kfroom->StartBattleRoom( kfmsg.maxtime() );

        // 回复给战斗服务器
        KFMsg::S2STellBattleRoomStartAck ack;
        ack.set_roomid( kfmsg.roomid() );
        kfroom->SendMessageToBattle( KFMsg::S2S_TELL_BATTLE_ROOM_START_ACK, &ack );

        __LOG_DEBUG__( "room[{}] start ok!", kfmsg.roomid() );
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleTellRoomStartToBattleShardAck )
    {
        __PROTO_PARSE__( KFMsg::S2STellRoomStartToMatchShardAck );
        __LOG_DEBUG__( "start room[{}] ack!", kfmsg.roomid() );

        auto kfroom = FindRoom( kfmsg.roomid(), __FUNC_LINE__ );
        if ( kfroom == nullptr )
        {
            return;
        }

        kfroom->ConfirmStartBattleRoom();

        __LOG_DEBUG__( "start room[{}] ok!", kfmsg.roomid() );
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleTellRoomFinishToBattleShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2STellRoomFinishToBattleShardReq );
        __LOG_DEBUG__( "room[{}] finish req!", kfmsg.roomid() );

        auto kfroom = FindRoom( kfmsg.roomid(), __FUNC_LINE__ );
        if ( kfroom != nullptr )
        {
            RemoveBattleRoom( kfroom );
        }

        KFMsg::S2STellBattleRoomFinishAck ack;
        ack.set_roomid( kfmsg.roomid() );
        auto ok = _kf_cluster_shard->SendToClient( kfmsg.serverid(), KFMsg::S2S_TELL_BATTLE_ROOM_FINISH_ACK, &ack );
        if ( ok )
        {
            __LOG_DEBUG__( "room[{}] serverid[{}] ip[{}] finish ok!", kfmsg.roomid(), KFAppID::ToString( kfmsg.serverid() ), kfmsg.ip() );
        }
        else
        {
            __LOG_ERROR__( "room[{}] serverid[{}] ip[{}] finish failed!", kfmsg.roomid(), KFAppID::ToString( kfmsg.serverid() ), kfmsg.ip() );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandlePlayerOnlineToBattleShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2SPlayerOnlineToBattleShardReq );
        __LOG_DEBUG__( "[{}:{}] online room[{}] req!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );

        auto kfroom = FindRoom( kfmsg.roomid(), __FUNC_LINE__ );
        if ( kfroom == nullptr )
        {
            return;
        }

        auto ok = kfroom->PlayerOnlineBattleRoom( kfmsg.campid(), kfmsg.playerid(), kfmsg.serverid() );
        if ( ok )
        {
            __LOG_DEBUG__( "[{}:{}] online room[{}] ok!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );
        }
        else
        {
            __LOG_ERROR__( "[{}:{}] online room[{}] failed!", kfmsg.campid(), kfmsg.playerid(), kfmsg.roomid() );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleBattleScoreBalanceToShardReq )
    {
        __PROTO_PARSE__( KFMsg::S2SBattleScoreBalanceToShardReq );
        __LOG_DEBUG__( "battle[{}] balance req!", kfmsg.roomid() );

        auto kfroom = FindRoom( kfmsg.roomid(), __FUNC_LINE__ );
        if ( kfroom == nullptr )
        {
            return;
        }

        auto pbscore = kfmsg.mutable_pbscore();
        auto ok = kfroom->BattleScoreBalance( pbscore );
        if ( ok )
        {
            // 保存到数据库
            auto strdata = KFProto::Serialize( pbscore, KFCompressEnum::Compress );
            _battle_redis_driver->Execute( "hset {}:{} {} {}", __KF_STRING__( battlescore ), pbscore->playerid(), kfmsg.roomid(), strdata );

            __LOG_DEBUG__( "battle[{}] balance ok!", kfmsg.roomid() );
        }
        else
        {
            __LOG_ERROR__( "battle[{}] balance failed!", kfmsg.roomid() );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandleOnlieQueryBattleScoreReq )
    {
        __PROTO_PARSE__( KFMsg::S2SOnlieQueryBattleScoreReq );

        auto queryscorelist = _battle_redis_driver->QueryMap( "hgetall {}:{}", __KF_STRING__( battlescore ), kfmsg.playerid() );
        for ( auto& iter : queryscorelist->_value )
        {
            KFMsg::S2SPlayerBattleScoreReq req;
            req.set_playerid( kfmsg.playerid() );
            req.set_roomid( KFUtility::ToValue< uint64 >( iter.first ) );
            KFProto::Parse( req.mutable_pbscore(), iter.second, KFCompressEnum::Compress );
            _kf_cluster_shard->SendToClient( kfid, KFMsg::S2S_PLAYER_BATTLE_SCORE_REQ, &req );
        }
    }

    __KF_MESSAGE_FUNCTION__( KFBattleShardModule::HandlePlayerBattleScoreAck )
    {
        __PROTO_PARSE__( KFMsg::S2SPlayerBattleScoreAck );
        __LOG_DEBUG__( "player[{}:{}] balance ack req!", kfmsg.playerid(), kfmsg.roomid() );

        auto kfresult = _battle_redis_driver->Execute( "hdel {}:{} {}", __KF_STRING__( battlescore ), kfmsg.playerid(), kfmsg.roomid() );
        if ( kfresult->IsOk() )
        {
            __LOG_DEBUG__( "player[{}:{}] balance ack ok!", kfmsg.playerid(), kfmsg.roomid() );
        }
        else
        {
            __LOG_ERROR__( "player[{}:{}] balance ack failed!", kfmsg.playerid(), kfmsg.roomid() );
        }
    }
}