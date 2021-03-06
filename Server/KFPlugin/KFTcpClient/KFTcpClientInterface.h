﻿#ifndef __KF_CLIENT_INTERFACE_H__
#define __KF_CLIENT_INTERFACE_H__

#include "KFrame.h"

namespace KFrame
{
    class KFTcpClientInterface : public KFModule
    {
    public:
        // 添加连接
        virtual void StartClient( const std::string& name, const std::string& type, uint64 id, const std::string& ip, uint32 port ) = 0;

        // 断开连接
        virtual void CloseClient( uint64 serverid, const char* function, uint32 line ) = 0;
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // 发送消息
        virtual void SendNetMessage( uint32 msgid, google::protobuf::Message* message ) = 0;
        virtual bool SendNetMessage( uint64 serverid, uint32 msgid, google::protobuf::Message* message ) = 0;
        virtual bool SendNetMessage( uint64 serverid, uint64 objectid, uint32 msgid, google::protobuf::Message* message ) = 0;

        virtual void SendNetMessage( uint32 msgid, const char* data, uint32 length ) = 0;
        virtual bool SendNetMessage( uint64 serverid, uint32 msgid, const char* data, uint32 length ) = 0;
        virtual bool SendNetMessage( uint64 serverid, uint64 objectid, uint32 msgid, const char* data, uint32 length ) = 0;

        // 给某一类型服务器发送消息
        virtual void SendMessageToName( const std::string& servername, uint32 msgid, google::protobuf::Message* message ) = 0;
        virtual void SendMessageToName( const std::string& servername, uint32 msgid, const char* data, uint32 length ) = 0;

        virtual void SendMessageToType( const std::string& servertype, uint32 msgid, google::protobuf::Message* message ) = 0;
        virtual void SendMessageToType( const std::string& servertype, uint32 msgid, const char* data, uint32 length ) = 0;

        virtual void SendMessageToServer( const std::string& servername, const std::string& servertype, uint32 msgid, google::protobuf::Message* message ) = 0;
        virtual void SendMessageToServer( const std::string& servername, const std::string& servertype, uint32 msgid, const char* data, uint32 length ) = 0;


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 添加注册回调函数
        template< class T >
        void RegisterConnectionFunction( T* object, void ( T::*handle )( uint64 serverid, const std::string& servername, const std::string& servertype ) )
        {
            KFClientConnectionFunction function = std::bind( handle, object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
            AddConnectionFunction( typeid( T ).name(), function );
        }

        // 删除注册回调函数
        template< class T >
        void UnRegisterConnectionFunction( T* object )
        {
            RemoveConnectionFunction( typeid( T ).name() );
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 断线函数回调
        template< class T >
        void RegisterLostFunction( T* object, void ( T::*handle )( uint64 serverid, const std::string& servername, const std::string& servertype ) )
        {
            KFClientLostFunction function = std::bind( handle, object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
            AddLostFunction( typeid( T ).name(), function );
        }

        // 删除注册回调函数
        template< class T >
        void UnRegisterLostFunction( T* object )
        {
            RemoveLostFunction( typeid( T ).name() );
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // 注册转发
        template< class T >
        void RegisterTransmitFunction( T* object, bool ( T::*handle )( const KFId& kfid, uint32 msgid, const char* data, uint32 length ) )
        {
            KFTransmitFunction function = std::bind( handle, object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 );
            AddTransmitFunction( typeid( T ).name(), function );
        }

        template< class T >
        void UnRegisterTransmitFunction( T* object )
        {
            RemoveTransmitFunction( typeid( T ).name() );
        }

    protected:
        // 添加注册回调函数
        virtual void AddConnectionFunction( const char* name, KFClientConnectionFunction& function ) = 0;
        virtual void RemoveConnectionFunction( const char* name ) = 0;

        // 添加断线函数
        virtual void AddLostFunction( const char* name, KFClientLostFunction& function ) = 0;
        virtual void RemoveLostFunction( const char* name ) = 0;

        // 转发函数
        virtual void AddTransmitFunction( const char* name, KFTransmitFunction& function ) = 0;
        virtual void RemoveTransmitFunction( const char* name ) = 0;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    __KF_INTERFACE__( _kf_tcp_client, KFTcpClientInterface );

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __KF_CLIENT_LOST_FUNCTION__( function ) \
    void function( uint64 serverid, const std::string& servername, const std::string& servertype )

#define __KF_CLIENT_CONNECT_FUNCTION__( function ) \
    void function( uint64 serverid, const std::string& servername, const std::string& servertype )

#define __REGISTER_CLIENT_CONNECTION_FUNCTION__( function ) \
    _kf_tcp_client->RegisterConnectionFunction( this, function )

#define __UNREGISTER_CLIENT_CONNECTION_FUNCTION__() \
    _kf_tcp_client->UnRegisterConnectionFunction( this )

#define __REGISTER_CLIENT_LOST_FUNCTION__( function ) \
    _kf_tcp_client->RegisterLostFunction( this, function )

#define __UNREGISTER_CLIENT_LOST_FUNCTION__() \
    _kf_tcp_client->UnRegisterLostFunction( this )

#define __REGISTER_CLIENT_TRANSMIT_FUNCTION__( function ) \
    _kf_tcp_client->RegisterTransmitFunction( this, function )

#define __UNREGISTER_CLIENT_TRANSMIT_FUNCTION__() \
    _kf_tcp_client->UnRegisterTransmitFunction( this )
}

#endif