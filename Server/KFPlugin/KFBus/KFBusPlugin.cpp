﻿#include "KFBusPlugin.h"
#include "KFBusModule.h"

namespace KFrame
{
    void KFBusPlugin::Install()
    {
        __REGISTER_MODULE__( KFBus );
    }

    void KFBusPlugin::UnInstall()
    {
        __UNREGISTER_MODULE__( KFBus );
    }

    void KFBusPlugin::LoadModule()
    {
        __FIND_MODULE__( _kf_config, KFConfigInterface );
        __FIND_MODULE__( _kf_message, KFMessageInterface );
        __FIND_MODULE__( _kf_ip_address, KFIpAddressInterface );
        __FIND_MODULE__( _kf_tcp_client, KFTcpClientInterface );
    }

}