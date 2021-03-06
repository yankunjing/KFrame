﻿#include "KFPlugin.h"

namespace KFrame
{
    KFPlugin::KFPlugin()
    {
        _sort = 0;
        _kf_module = nullptr;
    }

    KFPlugin::~KFPlugin()
    {
        __DELETE_OBJECT__( _kf_module );
    }

    void KFPlugin::InitModule()
    {
        _kf_module->InitModule();
    }

    void KFPlugin::LoadConfig()
    {
        _kf_module->LoadConfig();
    }

    void KFPlugin::AfterLoad()
    {
        _kf_module->AfterLoad();
    }

    void KFPlugin::BeforeRun()
    {
        _kf_module->BeforeRun();
    }

    void KFPlugin::BeforeShut()
    {
        _kf_module->BeforeShut();
    }

    void KFPlugin::ShutDown()
    {
        _kf_module->ShutDown();
    }

    void KFPlugin::AfterShut()
    {
        _kf_module->AfterShut();
    }

    void KFPlugin::OnceRun()
    {
        _kf_module->OnceRun();
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    KFModule* KFPlugin::FindModule( const std::string& name )
    {
        if ( name == _kf_module->_class_name )
        {
            return _kf_module;
        }

        return nullptr;
    }

    void KFPlugin::BindModule( const std::string& name, KFModule* module )
    {
        _kf_module = module;
        _kf_module->_kf_plugin = this;
        _kf_module->_class_name = name;
        _kf_module->_plugin_name = _plugin_name;
        _kf_module->_kf_plugin_manage = _kf_plugin_manage;
    }

    void KFPlugin::UnBindModule()
    {
        _kf_module->BeforeShut();
        _kf_module->ShutDown();
        _kf_module->AfterShut();

        __DELETE_OBJECT__( _kf_module );
    }
}
