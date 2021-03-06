﻿#ifndef __KF_PUBLIC_CLIENT_INTERFACE_H__
#define __KF_PUBLIC_CLIENT_INTERFACE_H__

#include "KFKernel/KFEntity.h"

namespace KFrame
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class KFPublicClientInterface : public KFModule
    {
    public:
        // 发送消息到Public
        virtual bool SendMessageToPublic( uint32 msgid, ::google::protobuf::Message* message ) = 0;

        // 更新公共数据
        virtual bool UpdatePublicData( uint64 playerid, const MapString& values ) = 0;
        virtual bool UpdatePublicData( KFEntity* player, const MapString& values ) = 0;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    __KF_INTERFACE__( _kf_public, KFPublicClientInterface );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}



#endif