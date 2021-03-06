﻿#include "KFPublicProxyPlugin.h"
#include "KFPublicProxyModule.h"

namespace KFrame
{
    void KFPublicProxyPlugin::Install()
    {
        __REGISTER_MODULE__( KFPublicProxy );
    }

    void KFPublicProxyPlugin::UnInstall()
    {
        __UNREGISTER_MODULE__( KFPublicProxy );
    }

    void KFPublicProxyPlugin::LoadModule()
    {
    }

}