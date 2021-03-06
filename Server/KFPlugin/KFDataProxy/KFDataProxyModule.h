﻿#ifndef __KF_DATA_PROXY_MODULE_H__
#define __KF_DATA_PROXY_MODULE_H__

/************************************************************************
//    @Module			:    数据功能模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2018-3-8
************************************************************************/

#include "KFrame.h"
#include "KFDataProxyInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFTcpClient/KFTcpClientInterface.h"
#include "KFClusterProxy/KFClusterProxyInterface.h"

namespace KFrame
{
    class KFDataProxyModule : public KFDataProxyInterface
    {
    public:
        KFDataProxyModule();
        ~KFDataProxyModule();

        // 初始化
        virtual void InitModule();

        // 刷新
        virtual void BeforeRun();

        // 关闭
        virtual void BeforeShut();

    protected:
        // 断开连接
        __KF_CLIENT_LOST_FUNCTION__( OnClientLostServer );

    protected:
        // 更新zone信息
        __KF_MESSAGE_FUNCTION__( HandleUpdateZoneToProxyReq );

        // 消息转发
        __KF_MESSAGE_FUNCTION__( HandleTransmitToDataShardReq );
    };
}

#endif