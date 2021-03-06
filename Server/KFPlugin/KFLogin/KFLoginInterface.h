﻿#ifndef __KF_LOGIN_INTERFACE_H__
#define __KF_LOGIN_INTERFACE_H__

#include "KFrame.h"

namespace KFrame
{
    /////////////////////////////////////////////////////////////////////////////////////////////
    class KFLoginInterface : public KFModule
    {
    public:
        // 是否连接到World
        virtual bool IsWorldConnected() = 0;

        // 发送消息到世界服务器
        virtual bool SendToWorld( uint32 msgid, ::google::protobuf::Message* message ) = 0;

        // 发送消息到Gate服务器
        virtual bool SendToGate( uint64 gateid, uint32 msgid, ::google::protobuf::Message* message ) = 0;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    __KF_INTERFACE__( _kf_login, KFLoginInterface );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}



#endif