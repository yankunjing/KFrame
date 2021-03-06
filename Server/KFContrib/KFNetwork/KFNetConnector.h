﻿#ifndef __NET_CONNECTOR_H__
#define __NET_CONNECTOR_H__

#include "KFNetSession.h"

namespace KFrame
{
    class KFNetServices;
    class KFNetConnector : public KFNetSession
    {
    public:

        KFNetConnector();
        ~KFNetConnector();

        // 初始化
        void InitConnector( uint64 id, KFNetServices* netservices );

        // 弹出一个收到消息
        KFNetMessage* PopNetMessage();

        // 发送消息
        bool SendNetMessage( uint32 msgid, const char* data, uint32 length );
        bool SendNetMessage( uint64 objectid, uint32 msgid, const char* data, uint32 length );

        // 执行消息处理
        void RunUpdate( KFNetFunction& netfunction, uint32 maxcount );

    protected:
        // 弹出一个消息
        KFNetMessage* PopMessage();

        // 发送单一消息
        bool SendSingleMessage( uint64 objectid, uint32 msgid, const char* data, uint32 length );

        // 发送拆包消息
        bool SendMultiMessage( uint64 objectid, uint32 msgid, const char* data, uint32 length );
        //////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////
        // 发送ping逻辑
        void RunSendPing();

        // 处理消息
        void RunMessage( KFNetFunction& netfunction, uint32 maxcount );

    protected:
        // 网络服务
        KFNetServices* _net_services;

        // 上一次通讯时间
        uint64 _last_message_time;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif