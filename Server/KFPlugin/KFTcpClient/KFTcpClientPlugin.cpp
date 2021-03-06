﻿#include "KFTcpClientPlugin.h"
#include "KFTcpClientModule.h"

//////////////////////////////////////////////////////////////////////////

namespace KFrame
{
    void KFTcpClientPlugin::Install()
    {
        __REGISTER_MODULE__( KFTcpClient );
    }

    void KFTcpClientPlugin::UnInstall()
    {
        __UNREGISTER_MODULE__( KFTcpClient );
    }

    void KFTcpClientPlugin::LoadModule()
    {
        __FIND_MODULE__( _kf_config, KFConfigInterface );
        __FIND_MODULE__( _kf_message, KFMessageInterface );
    }
}