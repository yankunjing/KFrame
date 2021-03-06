﻿#ifndef __KF_GUILD_CLIENT_INTERFACE_H__
#define __KF_GUILD_CLIENT_INTERFACE_H__

#include "KFKernel/KFEntity.h"

namespace KFrame
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class KFGuildClientInterface : public KFModule
    {
    public:
        // 发送消息到Guild
        virtual bool SendMessageToGuild( uint64 guildid, uint32 msgid, ::google::protobuf::Message* message ) = 0;
        virtual bool SendMessageToGuild( uint32 msgid, ::google::protobuf::Message* message ) = 0;

        // 更新成员的基础信息
        virtual void UpdateMemberBasic( uint32 playerid, uint64 guildid, MapString& basics ) = 0;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    __KF_INTERFACE__( _kf_guild, KFGuildClientInterface );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}



#endif