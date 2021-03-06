﻿#include "KFMessageModule.h"

namespace KFrame
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void KFMessageModule::AddFunction( uint32 msgid, KFMessageFunction& function )
    {
        auto kffunction = _kf_message_function.Find( msgid );
        if ( kffunction == nullptr )
        {
            kffunction = _kf_message_function.Create( msgid );
            kffunction->_function = function;
        }
        else
        {
            __LOG_ERROR__( "msgid[{}] already register!", msgid );
        }
    }

    bool KFMessageModule::CallFunction( const KFId& kfid, uint32 msgid, const char* data, uint32 length )
    {
        // ping 消息不处理
        if ( msgid == 0 )
        {
            return true;
        }

        auto kffunction = _kf_message_function.Find( msgid );
        if ( kffunction == nullptr )
        {
            return false;
        }

        kffunction->_function( kfid, data, length );
        return true;
    }

    void KFMessageModule::UnRegisterFunction( uint32 msgid )
    {
        auto ok = _kf_message_function.Remove( msgid );
        if ( !ok )
        {
            __LOG_ERROR__( "msgid[{}] unregister failed!", msgid );
        }
    }

}