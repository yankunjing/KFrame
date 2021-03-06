﻿#ifndef __KF_PUBLIC_CLIENT_MODULE_H__
#define __KF_PUBLIC_CLIENT_MODULE_H__

/************************************************************************
//    @Module			:    公共数据模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2018-3-8
************************************************************************/

#include "KFrame.h"
#include "KFPublicClientInterface.h"
#include "KFPlayer/KFPlayerInterface.h"
#include "KFKernel/KFKernelInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFZone/KFZoneInterface.h"
#include "KFClusterClient/KFClusterClientInterface.h"

namespace KFrame
{
    class KFPublicClientModule : public KFPublicClientInterface
    {
    public:
        KFPublicClientModule() = default;
        ~KFPublicClientModule() = default;

        // 刷新
        virtual void BeforeRun();
        virtual void OnceRun();

        // 关闭
        virtual void BeforeShut();
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // 更新公共数据
        virtual bool UpdatePublicData( uint64 playerid, const MapString& values );
        virtual bool UpdatePublicData( KFEntity* player, const MapString& values );

        // 发送消息到Public
        virtual bool SendMessageToPublic( uint32 msgid, ::google::protobuf::Message* message );
    protected:
        // 更新属性回调
        __KF_UPDATE_DATA_FUNCTION__( OnUpdateDataCallBack );
        __KF_UPDATE_STRING_FUNCTION__( OnUpdateStringCallBack );

        void OnUpdateDataToPublic( KFEntity* player, KFData* kfdata );

        // 上线更新公共数据
        void OnEnterUpdatePublicData( KFEntity* player );

        // 离线更新公共数据
        void OnLeaveUpdatePublicData( KFEntity* player );

    protected:
        // 查询玩家基本数据
        __KF_MESSAGE_FUNCTION__( HandleQueryBasicReq );

        // 查询玩家基本数据
        __KF_MESSAGE_FUNCTION__( HandleQueryBasicAck );
    private:
        // 组件
        KFComponent* _kf_component{ nullptr };

        // 基础模块
        KFData* _kf_basic{ nullptr };
    };
}

#endif