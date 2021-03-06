﻿#ifndef __KF_MATCH_SHARD_MODULE_H__
#define __KF_MATCH_SHARD_MODULE_H__

/************************************************************************
//    @Module			:    匹配房间逻辑模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-12-20
************************************************************************/

#include "KFMatchQueue.h"
#include "KFMatchShardInterface.h"
#include "KFConfig/KFConfigInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFTcpClient/KFTcpClientInterface.h"
#include "KFTcpServer/KFTcpServerInterface.h"
#include "KFClusterShard/KFClusterShardInterface.h"
#include "KFClusterClient/KFClusterClientInterface.h"
#include "KFTimer/KFTimerInterface.h"
#include "KFRedis/KFRedisInterface.h"

namespace KFrame
{
    class KFMatchShardModule : public KFMatchShardInterface
    {
    public:
        KFMatchShardModule() = default;
        ~KFMatchShardModule() = default;

        // 加载配置
        virtual void InitModule();

        // 初始化
        virtual void BeforeRun();
        virtual void OnceRun();
        virtual void Run();

        // 关闭
        virtual void BeforeShut();
        ////////////////////////////////////////////////////////////////////////////////
    protected:
        // 处理匹配请求
        __KF_MESSAGE_FUNCTION__( HandleMatchToShardReq );

        // 查询匹配
        __KF_MESSAGE_FUNCTION__( HandleQueryRoomToMatchShardReq );

        // 取消匹配
        __KF_MESSAGE_FUNCTION__( HandleCancelMatchToShardReq );

        // 创建房间回馈
        __KF_MESSAGE_FUNCTION__( HandleCreateRoomToMatchShardAck );

        // 战场开启
        __KF_MESSAGE_FUNCTION__( HandleOpenRoomToMatchShardReq );

        // 战场房间正式开始
        __KF_MESSAGE_FUNCTION__( HandleTellRoomStartToMatchShardReq );

        // 阵营加入房间
        __KF_MESSAGE_FUNCTION__( HandleAddCampToMatchShardAck );

        // 玩家离开战场
        __KF_MESSAGE_FUNCTION__( HandlePlayerLeaveRoomToMatchShardReq );

        // 战场关闭
        __KF_MESSAGE_FUNCTION__( HandleTellRoomCloseToMatchShardReq );

        // 重置匹配房间
        __KF_MESSAGE_FUNCTION__( HandleResetMatchRoomReq );

        // 查询房间版本号
        __KF_MESSAGE_FUNCTION__( HandleQueryBattleVersionReq );
    protected:
        // Match Master连接成功
        __KF_CLIENT_CONNECT_FUNCTION__( OnClientConnectMatchMaster );

        // 定时器查询版本信息
        __KF_TIMER_FUNCTION__( OnTimerQueryBattleVersion );
    protected:
        KFMatchQueue* FindMatchQueue( uint32 matchid, const char* function, uint32 line );

    private:
        KFRedisDriver* _kf_battle_driver{ nullptr };

        // 匹配模式列表
        KFMap< uint32, uint32, KFMatchQueue > _kf_match_queue;

        // 战斗服的版本号列表
        std::set< std::string > _battle_version_list;

    };
}



#endif