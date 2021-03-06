﻿#ifndef __KF_MAIL_SHARD_MODULE_H__
#define __KF_MAIL_SHARD_MODULE_H__

/************************************************************************
//    @Module			:    数据库模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-11-30
************************************************************************/

#include "KFrame.h"
#include "KFProtocol/KFProtocol.h"
#include "KFMailShardInterface.h"
#include "KFRedis/KFRedisInterface.h"
#include "KFWorker/KFWorkerInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFSchedule/KFScheduleInterface.h"
#include "KFOption/KFOptionInterface.h"
#include "KFSchedule/KFScheduleInterface.h"
#include "KFClusterShard/KFClusterShardInterface.h"

namespace KFrame
{
    class KFMailShardModule : public KFMailShardInterface
    {
    public:
        KFMailShardModule() = default;
        ~KFMailShardModule() = default;

        // 初始化
        virtual void BeforeRun();
        virtual void OnceRun();

        // 关闭
        virtual void BeforeShut ();
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////

    protected:
        // 删除邮件
        __KF_MESSAGE_FUNCTION__ ( HandleDeleteMailReq );

        // 查询邮件
        __KF_MESSAGE_FUNCTION__ ( HandleQueryMailReq );

        // 添加邮件
        __KF_MESSAGE_FUNCTION__( HandleAddMailReq );

        // 设置邮件状态
        __KF_MESSAGE_FUNCTION__( HandleUpdateMailFlagReq );

        // 新玩家登陆邮件处理
        __KF_MESSAGE_FUNCTION__( HandleNewPlayerLoginMailReq );

        ///////////////////////////////////////////////////////////////////////////////
        //GM增加邮件
        // 新玩家登陆邮件处理
        __KF_MESSAGE_FUNCTION__( HandleGMAddMailReq );

    protected:
        // 计划清理过期的全局邮件
        __KF_SCHEDULE_FUNCTION__( OnScheduleClearWholeOverdueMail );

        // 重载GM全局邮件
        void LoadWholeMailToPerson( uint32 playerid );

        // 根据邮件类型获取redis键名
        std::string FormatMailKeyName( uint32 playerid, uint32 mailtype, const char* function, uint32 line );

        // 设置邮件状态
        bool UpdateMailFlag( uint32 playerid, uint64 mailid, uint32 mailtype, uint32 flag );

        // 删除邮件
        bool RemoveMail( uint32 playerid, uint32 mailtype, uint64 mailid );

        // 添加邮件 当maillistkey == ""发送系统邮件
        bool AddMail( uint32 playerid, uint32 mailtype, MapString& maildata );

        // 通知有新邮件
        void NoticePlayerNewMail( uint32 playerid, uint32 mailtype );

    private:
        KFRedisDriver* _mail_redis_driver{ nullptr };
        KFRedisDriver* _public_redis_driver{ nullptr };
    };
}

#endif