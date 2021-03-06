﻿#include "KFDisplayPlugin.h"
#include "KFDisplayModule.h"

//////////////////////////////////////////////////////////////////////////

namespace KFrame
{
    void KFDisplayPlugin::Install()
    {
        __REGISTER_MODULE__( KFDisplay );
    }

    void KFDisplayPlugin::UnInstall()
    {
        __UNREGISTER_MODULE__( KFDisplay );
    }

    void KFDisplayPlugin::LoadModule()
    {
        __FIND_MODULE__( _kf_tcp_server, KFTcpServerInterface );
        __FIND_MODULE_NO_LOG__( _kf_player, KFPlayerInterface );
        __FIND_MODULE_NO_LOG__( _kf_route, KFRouteClientInterface );
        __FIND_MODULE_NO_LOG__( _kf_cluster_shard, KFClusterShardInterface );
    }
}