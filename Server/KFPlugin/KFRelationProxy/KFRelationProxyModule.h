﻿#ifndef __KF_RELATION_PROXY_MODULE_H__
#define __KF_RELATION_PROXY_MODULE_H__

/************************************************************************
//    @Module			:	 好友逻辑模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2018-3-8
************************************************************************/

#include "KFrame.h"
#include "KFProtocol/KFProtocol.h"
#include "KFRelationProxyInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFTcpClient/KFTcpClientInterface.h"
#include "KFClusterProxy/KFClusterProxyInterface.h"

namespace KFrame
{
    class KFRelationProxyModule : public KFRelationProxyInterface
    {
    public:
        KFRelationProxyModule();
        ~KFRelationProxyModule();

        // 加载配置
        virtual void InitModule();

        // 初始化
        virtual void BeforeRun();

        // 关闭
        virtual void BeforeShut();
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

    };
}



#endif