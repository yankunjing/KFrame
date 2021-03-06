﻿#include "KFAttributePlugin.h"
#include "KFAttributeModule.h"
//////////////////////////////////////////////////////////////////////////

namespace KFrame
{
    void KFAttributePlugin::Install()
    {
        __REGISTER_MODULE__( KFAttribute );
    }

    void KFAttributePlugin::UnInstall()
    {
        __UNREGISTER_MODULE__( KFAttribute );
    }

    void KFAttributePlugin::LoadModule()
    {
        __FIND_MODULE__( _kf_debug, KFDebugInterface );
        __FIND_MODULE__( _kf_kernel, KFKernelInterface );
        __FIND_MODULE__( _kf_message, KFMessageInterface );
        __FIND_MODULE__( _kf_player, KFPlayerInterface );
        __FIND_MODULE__( _kf_display, KFDisplayInterface );
        __FIND_MODULE__( _kf_filter, KFFilterInterface );
        __FIND_MODULE__( _kf_data, KFDataClientInterface );
        __FIND_MODULE__( _kf_public, KFPublicClientInterface );
    }

}