﻿#include "KFSchedulePlugin.h"
#include "KFScheduleModule.h"
//////////////////////////////////////////////////////////////////////////

namespace KFrame
{
    void KFSchedulePlugin::Install()
    {
        __REGISTER_MODULE__( KFSchedule );
    }

    void KFSchedulePlugin::UnInstall()
    {
        __UNREGISTER_MODULE__( KFSchedule );
    }

    void KFSchedulePlugin::LoadModule()
    {

    }
}