﻿#ifndef __CLIENT_ENGINE_H__
#define __CLIENT_ENGINE_H__

#include "KFrame.h"
#include "KFNetClient.h"

namespace KFrame
{
    class KFNetClientServices;
    class KFNetClientEngine
    {
    public:

        KFNetClientEngine();
        ~KFNetClientEngine();

        // 初始化引擎
        void InitEngine( uint32 queuesize );

        // 关闭引擎
        void ShutEngine();

        // 执行逻辑
        void RunEngine( uint64 nowtime );

        // 开始一个客户端
        bool StartClient( const std::string& name, const std::string& type, uint64 id, const std::string& ip, uint32 port );

        // 关闭一个客户端
        void CloseClient( uint64 id, const char* function, uint32 line );

        // 查找客户端
        KFNetClient* FindClient( uint64 id );
        //////////////////////////////////////////////////////////////////////////////////////////////////

        // 发送消息
        void SendNetMessage( uint32 msgid, const char* data, uint32 length );
        bool SendNetMessage( uint64 serverid, uint32 msgid, const char* data, uint32 length );
        bool SendNetMessage( uint64 serverid, uint64 objectid, uint32 msgid, const char* data, uint32 length );

        // 指定发送消息
        void SendMessageToName( const std::string& servername, uint32 msgid, const char* data, uint32 length );
        void SendMessageToType( const std::string& servertype, uint32 msgid, const char* data, uint32 length );
        void SendMessageToServer( const std::string& servername, const std::string& servertype, uint32 msgid, const char* data, uint32 length );

    public:
        // 绑定消息事件
        template< class T >
        void BindNetFunction( T* object, void ( T::*handle )( const KFId& kfid, uint32 msgid, const char* data, uint32 length ) )
        {
            _net_function = std::bind( handle, object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 );
        }

        // 绑定连接事件
        template< class T >
        void BindConnectFunction( T* object, void ( T::*handle )( uint64 id, const std::string& name, const std::string& type ) )
        {
            _client_connect_function = std::bind( handle, object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
        }

        // 绑定断开事件
        template< class T >
        void BindDisconnectFunction( T* object, void ( T::*handle )( uint64 id, const std::string& name, const std::string& type ) )
        {
            _client_disconnect_function = std::bind( handle, object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
        }

    protected:
        // 等待添加的客户端
        void RunWaitClient();

        // 处理所有客户端消息
        void HandleClientMessage();

        // 客户端连接成功
        void OnClientConnected( const KFEventData* eventdata );

        // 客户端断开连接
        void OnClientDisconnect( const KFEventData* eventdata );

        // 客户端关闭
        void OnClientShutDown( const KFEventData* eventdata );

        // 连接失败
        void OnClientFailed( const KFEventData* eventdata );

    public:

        // 网络服务
        KFNetClientServices* _net_client_services;

    protected:
        // 等待启动的客户端
        std::map< uint64, KFNetSetting > _wait_clients;

        // 客户端列表
        KFMap< uint64, uint64, KFNetClient > _kf_clients;

    protected:
        // 消息函数
        KFNetFunction _net_function;

        // 客户端连接
        KFClientConnectionFunction _client_connect_function;

        // 客户端断开
        KFClientLostFunction _client_disconnect_function;
    };
}

#endif
