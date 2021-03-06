﻿#include "KFMatchRoom.h"
#include "KFMatchQueue.h"
#include "KFRedis/KFRedisInterface.h"
#include "KFClusterClient/KFClusterClientInterface.h"

namespace KFrame
{
    KFMatchRoom::KFMatchRoom()
    {
        _room_id = _invalid_int;
        _battle_server_id = _invalid_int;
        _battle_shard_id = _invalid_int;
        _room_player_count = _invalid_int;
        _battle_start_time = _invalid_int;
        _is_stop_add_camp = false;
        _match_id = _invalid_int;
    }

    KFMatchRoom::~KFMatchRoom()
    {
        _camp_list.Clear();
    }

    void KFMatchRoom::Initialize( KFMatchQueue* kfmatchqueue, uint64 battleserverid, const std::string& version )
    {
        _kf_match_queue = kfmatchqueue;
        _match_id = _kf_match_queue->_match_id;
        _battle_server_id = battleserverid;
        _battle_version = version;
        _room_id = KFGlobal::Instance()->Make64Guid();

        // 开启创建定时器
        _create_timer.StartTimer( 1, 10000 );
    }

    void KFMatchRoom::ResetRoom()
    {
        __LOG_DEBUG__( "room[{}] reset!", _room_id );

        _battle_shard_id = _invalid_int;
        _create_timer.StartTimer( 1, 10000 );

        // 玩家进入重置
        for ( auto& campiter : _camp_list._objects )
        {
            auto kfcamp = campiter.second;
            kfcamp->ResetEnterBattleRoom();
        }
    }

    void KFMatchRoom::RunRoom()
    {
        // 分配房间
        CreateBatterRoomReq();

        // 进入战场房间
        EnterBattleRoomReq();
    }

    bool KFMatchRoom::SendToBattle( uint32 msgid, google::protobuf::Message* message )
    {
        return _kf_cluster->SendToShard( __KF_STRING__( battle ), _battle_shard_id, msgid, message );
    }

    void KFMatchRoom::SendToRoom( uint32 msgid, google::protobuf::Message* message )
    {
        for ( auto& campiter : _camp_list._objects )
        {
            auto kfcamp = campiter.second;
            for ( auto& groupiter : kfcamp->_group_list._objects )
            {
                auto kfgroup = groupiter.second;
                for ( auto& playeriter : kfgroup->_player_list )
                {
                    auto kfplayer = playeriter.second;
                    kfplayer->SendToGame( msgid, message );
                }
            }
        }
    }

    void KFMatchRoom::AddCamp( KFMatchCamp* kfcamp )
    {
        // 设置房间id
        for ( auto& giter : kfcamp->_group_list._objects )
        {
            auto kfgroup = giter.second;
            for ( auto& iter : kfgroup->_player_list )
            {
                auto kfplayer = iter.second;
                kfplayer->SetRoomID( _room_id );
            }
        }

        // 加入列表中
        _camp_list.Insert( kfcamp->_camp_id, kfcamp );

        // 玩家数量
        CalcRoomPlayerCount( KFOperateEnum::Add, kfcamp->PlayerCount(), __FUNC_LINE__ );
    }

    bool KFMatchRoom::IsWaitMatch( uint64 battleserverid, uint32 playercount, const std::string& version )
    {
        // 版本号
        if ( _battle_version != version )
        {
            return false;
        }

        // 判断是否指定战场id
        if ( _battle_server_id != battleserverid )
        {
            return false;
        }

        // 停止添加阵营, 判断人数
        if ( _is_stop_add_camp )
        {
            return false;
        }

        // 最大人数限制
        if ( _room_player_count + playercount > _kf_match_queue->_kf_setting->_max_player_count )
        {
            return false;
        }

        // 判断时间是否已经到了
        if ( _battle_start_time > _invalid_int )
        {
            if ( KFGlobal::Instance()->_game_time > _battle_start_time )
            {
                return false;
            }
        }

        return true;
    }

    void KFMatchRoom::CreateBatterRoomReq()
    {
        if ( !_create_timer.DoneTimer( KFGlobal::Instance()->_game_time, true ) )
        {
            return;
        }

        // 发送消息
        KFMsg::S2SCreateRoomToBattleProxyReq req;
        req.set_roomid( _room_id );
        req.set_battleserverid( _battle_server_id );
        req.set_matchid( _kf_match_queue->_match_id );
        req.set_maxplayercount( _kf_match_queue->_kf_setting->_max_player_count );
        req.set_version( _battle_version );
        auto ok = SendToBattle( KFMsg::S2S_CREATE_ROOM_TO_BATTLE_PROXY_REQ, &req );
        if ( ok )
        {
            __LOG_DEBUG__( "create battle room[{}] req!", _room_id );
        }
        else
        {
            __LOG_ERROR__( "create battle room[{}] failed!", _room_id );
        }
    }

    void KFMatchRoom::CreateBattleRoomAck( uint64 battleshardid )
    {
        _create_timer.StopTimer();
        _battle_shard_id = battleshardid;

        __LOG_DEBUG__( "room[{}] create battle[{}] ok!", _room_id, _battle_shard_id );
    }

    void KFMatchRoom::EnterBattleRoomReq()
    {
        // 还没有创建战场房间
        if ( _battle_shard_id == _invalid_int )
        {
            return;
        }

        // 请求把matchroom的阵营加入到battleroom中
        for ( auto& campiter : _camp_list._objects )
        {
            auto kfcamp = campiter.second;
            kfcamp->EnterBattleRoomReq( this );
        }
    }

    bool KFMatchRoom::EnterBattleRoomAck( uint32 campid, bool addok )
    {
        auto kfcamp = _camp_list.Find( campid );
        if ( kfcamp == nullptr )
        {
            return false;
        }

        if ( addok )
        {
            kfcamp->EnterBattleRoomAck( this );
        }
        else
        {
            // 删除阵营
            _is_stop_add_camp = true;
            _camp_list.Remove( campid, false );

            // 找到一个新的战场
            auto kfroom = _kf_match_queue->FindWaitMatchRoom( _battle_server_id, kfcamp->PlayerCount(), kfcamp->_battle_version );
            kfroom->AddCamp( kfcamp );
        }

        return true;
    }

    bool KFMatchRoom::TellRoomOpen( uint32 waittime )
    {
        // 设置战场开始的时间
        // 战场正式开始前10秒钟, 玩家不进入该战场了
        _battle_start_time = KFGlobal::Instance()->_game_time + ( waittime * KFTimeEnum::OneSecondMicSecond ) - 10000;

        // 回复消息
        KFMsg::S2SOpenRoomToBattleShardAck ack;
        ack.set_roomid( _room_id );
        auto ok = SendToBattle( KFMsg::S2S_OPEN_ROOM_TO_BATTLE_SHARD_ACK, &ack );
        if ( !ok )
        {
            __LOG_ERROR__( "match room[{}] open failed!", _room_id );
        }

        return ok;
    }

    void KFMatchRoom::TellRoomStart()
    {
        KFMsg::S2STellRoomStartToMatchShardAck ack;
        ack.set_roomid( _room_id );
        auto ok = SendToBattle( KFMsg::S2S_TELL_ROOM_START_TO_BATTLE_SHARD_ACK, &ack );
        if ( !ok )
        {
            __LOG_ERROR__( "match room[{}] start failed!", _room_id );
        }
    }

    bool KFMatchRoom::LeaveBattleRoom( uint32 campid, uint64 groupid, uint64 playerid )
    {
        auto kfcamp = _camp_list.Find( campid );
        if ( kfcamp == nullptr )
        {
            return false;
        }

        auto ok = kfcamp->RemovePlayer( groupid, playerid );
        if ( ok )
        {
            if ( kfcamp->PlayerCount() == 0 )
            {
                _camp_list.Remove( campid );
            }

            CalcRoomPlayerCount( KFOperateEnum::Dec, 1, __FUNC_LINE__ );
        }

        return ok;
    }

    bool KFMatchRoom::CancelMatch( uint32 campid, uint64 groupid )
    {
        {
            // 发送消息到战斗集群
            KFMsg::S2SCancelMatchToBattleShardReq req;
            req.set_roomid( _room_id );
            req.set_campid( campid );
            req.set_groupid( groupid );
            SendToBattle( KFMsg::S2S_CANCEL_MATCH_TO_BATTLE_SHARD_REQ, &req );
        }

        auto kfcamp = _camp_list.Find( campid );
        if ( kfcamp == nullptr )
        {
            return false;
        }

        auto campplayercount = kfcamp->PlayerCount();
        auto ok = kfcamp->RemoveGroup( groupid );
        if ( ok )
        {
            // 将剩余的队伍, 重新加入等待队伍中
            for ( auto& iter : kfcamp->_group_list._objects )
            {
                _kf_match_queue->AddWaitGroup( iter.second );
            }
            kfcamp->_group_list.Clear( false );

            // 删除阵营
            _camp_list.Remove( campid );
            CalcRoomPlayerCount( KFOperateEnum::Dec, campplayercount, __FUNC_LINE__ );
        }

        return ok;
    }

    void KFMatchRoom::CalcRoomPlayerCount( uint32 operate, uint32 count, const char* function, uint32 line )
    {
        _room_player_count = KFUtility::Operate( operate, _room_player_count, count );
        __LOG_DEBUG_FUNCTION__( function, line, "room[{}] playercount[{}]", _room_id, _room_player_count );

        // 房间还没有开启, 通知客户端
        if ( _battle_start_time == _invalid_int )
        {
            KFMsg::MsgTellMatchPlayerCount tell;
            tell.set_count( _room_player_count );
            SendToRoom( KFMsg::MSG_TELL_MATCH_PLAYER_COUNT, &tell );
        }
    }
}