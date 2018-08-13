﻿#include "KFNetClientEngine.h"
#include "KFNetClientServices.h"
#include "KFNetEvent.h"

namespace KFrame
{
    KFNetClientEngine::KFNetClientEngine()
    {
        _net_client_services = nullptr;
        _client_connect_function = nullptr;
        _client_disconnect_function = nullptr;
    }

    KFNetClientEngine::~KFNetClientEngine()
    {
        __KF_DELETE__( _net_client_services );
    }

    void KFNetClientEngine::InitEngine( uint32 queuesize )
    {
        // 网络服务
        _net_client_services = __KF_NEW__( KFNetClientServices );
        _net_client_services->InitServices( 100, queuesize );
        _net_client_services->StartServices( nullptr );

        // 注册网络时间
        _net_client_services->_net_event->BindDisconnectFunction( this, &KFNetClientEngine::OnClientDisconnect );
        _net_client_services->_net_event->BindShutFunction( this, &KFNetClientEngine::OnClientShutDown );
        _net_client_services->_net_event->BindConnectFunction( this, &KFNetClientEngine::OnClientConnected );
        _net_client_services->_net_event->BindFailedFunction( this, &KFNetClientEngine::OnClientFailed );
    }

    void KFNetClientEngine::ShutEngine()
    {
        auto kfclient = _kf_clients.First();
        while ( kfclient != nullptr )
        {
            kfclient->CloseClient();
            kfclient = _kf_clients.Next();
        }

        _net_client_services->ShutServices();
        _net_client_services->_net_event->ShutEvent();
    }

    void KFNetClientEngine::RunEngine( uint64 nowtime )
    {
        // 网络事件
        _net_client_services->_net_event->RunEvent();

        // 添加客户端
        RunWaitClient();

        // 处理所有客户端消息
        HandleClientMessage();
    }

    void KFNetClientEngine::HandleClientMessage()
    {
        // 每次取200个消息, 防止占用过多的cpu
        static const uint32 _max_message_count = 200;

        auto kfclient = _kf_clients.First();
        while ( kfclient != nullptr )
        {
            auto messagecount = 0u;
            auto message = kfclient->PopNetMessage();
            while ( message != nullptr )
            {
                _net_function( message->_guid, message->_msgid, message->_data, message->_length );

                // 每次处理200个消息
                ++messagecount;
                if ( messagecount >= _max_message_count )
                {
                    break;
                }
                message = kfclient->PopNetMessage();
            }

            kfclient = _kf_clients.Next();
        }
    }

    void KFNetClientEngine::OnClientConnected( const KFEventData* eventdata )
    {
        auto kfclient = _kf_clients.Find( eventdata->_id );
        if ( kfclient == nullptr )
        {
            return;
        }

        auto kfsetting = &kfclient->_net_setting;

        __LOG_DEBUG__( KFLogEnum::Net, "[{}:{}:{}|{}:{}] connect ok!",
                       kfsetting->_name, kfsetting->_type, KFAppID::ToString( kfsetting->_id ),
                       kfsetting->_ip, kfsetting->_port );

        // 上层回调
        if ( _client_connect_function != nullptr )
        {
            _client_connect_function( kfsetting->_id, kfsetting->_name, kfsetting->_type );
        }
    }

    void KFNetClientEngine::OnClientDisconnect( const KFEventData* eventdata )
    {
        auto kfclient = _kf_clients.Find( eventdata->_id );
        if ( kfclient == nullptr )
        {
            return;
        }

        auto kfsetting = &kfclient->_net_setting;
        __LOG_DEBUG__( KFLogEnum::Net, "[{}:{}:{}|{}:{}] disconnect!",
                       kfsetting->_name, kfsetting->_type, KFAppID::ToString( kfsetting->_id ),
                       kfsetting->_ip, kfsetting->_port );

        // 上层回调
        if ( _client_disconnect_function != nullptr )
        {
            _client_disconnect_function( kfsetting->_id, kfsetting->_name, kfsetting->_type );
        }
    }

    void KFNetClientEngine::OnClientShutDown( const KFEventData* eventdata )
    {
        auto kfclient = _kf_clients.Find( eventdata->_id );
        if ( kfclient == nullptr )
        {
            return;
        }

        auto kfsetting = &kfclient->_net_setting;
        __LOG_DEBUG__( KFLogEnum::Net, "[{}:{}:{}|{}:{}] client shutdown!",
                       kfsetting->_name, kfsetting->_type, KFAppID::ToString( kfsetting->_id ),
                       kfsetting->_ip, kfsetting->_port );

        _kf_clients.Remove( eventdata->_id );
    }

    void KFNetClientEngine::OnClientFailed( const KFEventData* eventdata )
    {
        auto kfclient = _kf_clients.Find( eventdata->_id );
        if ( kfclient == nullptr )
        {
            return;
        }

        auto kfsetting = &kfclient->_net_setting;
        __LOG_ERROR__( KFLogEnum::Net, "[{}:{}:{}|{}:{}] connect failed!",
                       kfsetting->_name, kfsetting->_type, KFAppID::ToString( kfsetting->_id ),
                       kfsetting->_ip, kfsetting->_port );
    }

    void KFNetClientEngine::StartClient( const std::string& name, const std::string& type, uint32 id, const std::string& ip, uint32 port )
    {
        auto kfclient = _kf_clients.Find( id );
        if ( kfclient != nullptr )
        {
            return;
        }

        KFNetSetting kfsetting;
        kfsetting._id = id;
        kfsetting._type = type;
        kfsetting._name = name;
        kfsetting._ip = ip;
        kfsetting._port = port;
        _wait_clients[ id ] = kfsetting;
    }

    void KFNetClientEngine::RunWaitClient()
    {
        if ( _net_client_services->_is_shutdown || _wait_clients.empty() )
        {
            return;
        }

        for ( auto& iter : _wait_clients )
        {
            auto& kfsetting = iter.second;
            auto kfclient = _kf_clients.Create( kfsetting._id );
            kfclient->StartClient( _net_client_services, kfsetting );
        }

        _wait_clients.clear();
    }

    KFNetClient* KFNetClientEngine::FindClient( uint32 id )
    {
        return _kf_clients.Find( id );
    }

    void KFNetClientEngine::CloseClient( uint32 id, const char* function, uint32 line )
    {
        auto kfclient = _kf_clients.Find( id );
        if ( kfclient == nullptr )
        {
            return;
        }

        auto kfsetting = &kfclient->_net_setting;
        kfclient->CloseClient();
    }

    void KFNetClientEngine::SendNetMessage( uint32 msgid, const char* data, uint32 length )
    {
        for ( auto& iter : _kf_clients._objects )
        {
            auto netclient = iter.second;
            netclient->SendNetMessage( iter.first, msgid, data, length );
        }
    }

    bool KFNetClientEngine::SendNetMessage( uint32 serverid, uint32 msgid, const char* data, uint32 length )
    {
        auto netclient = _kf_clients.Find( serverid );
        if ( netclient == nullptr )
        {
            __LOG_ERROR__( KFLogEnum::Net, "msgid[{}] can't find server[{}]!", msgid, KFAppID::ToString( serverid ) );
            return false;
        }

        return netclient->SendNetMessage( msgid, data, length );
    }

    bool KFNetClientEngine::SendNetMessage( uint32 serverid, uint32 objectid, uint32 msgid, const char* data, uint32 length )
    {
        auto netclient = _kf_clients.Find( serverid );
        if ( netclient == nullptr )
        {
            __LOG_ERROR__( KFLogEnum::Net, "msgid[{}] can't find server[{}]!", msgid, KFAppID::ToString( serverid ) );
            return false;
        }

        return netclient->SendNetMessage( objectid, msgid, data, length );
    }

    void KFNetClientEngine::SendMessageToType( const std::string& servertype, uint32 msgid, const char* data, uint32 length )
    {
        for ( auto& iter : _kf_clients._objects )
        {
            auto kfclient = iter.second;
            if ( kfclient->_net_setting._type == servertype )
            {
                kfclient->SendNetMessage( iter.first, msgid, data, length );
            }
        }
    }

    void KFNetClientEngine::SendMessageToName( const std::string& servername, uint32 msgid, const char* data, uint32 length )
    {
        for ( auto& iter : _kf_clients._objects )
        {
            auto kfclient = iter.second;
            if ( kfclient->_net_setting._name == servername )
            {
                kfclient->SendNetMessage( iter.first, msgid, data, length );
            }
        }
    }

    void KFNetClientEngine::SendMessageToServer( const std::string& servername, const std::string& servertype, uint32 msgid, const char* data, uint32 length )
    {
        for ( auto& iter : _kf_clients._objects )
        {
            auto netclient = iter.second;
            if ( netclient->_net_setting._name == servername
                    && netclient->_net_setting._type == servertype )
            {
                netclient->SendNetMessage( iter.first, msgid, data, length );
            }
        }
    }
}
