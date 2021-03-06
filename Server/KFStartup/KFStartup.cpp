﻿#include "KFStartup.h"
#include "KFLibrary.h"
#include "KFAppConfig.h"

namespace KFrame
{
    bool KFStartup::InitStartup( const std::string& file )
    {
        // 加载配置
        if ( !_app_config->LoadStartupConfig( file ) )
        {
            return false;
        }

        auto kfglobal = KFGlobal::Instance();
        kfglobal->_app_name = _app_config->_app_name;
        kfglobal->_app_type = _app_config->_app_type;

        return true;
    }

    bool KFStartup::LoadPlugin()
    {
        auto kfglobal = KFGlobal::Instance();

        for ( auto& iter : _app_config->_startups )
        {
            auto kfsetting = &iter.second;
            auto loadplguin = kfsetting->_name;
#ifdef __KF_DEBUG__
            loadplguin += "d";
#endif
            bool result = LoadPluginLibrary( loadplguin, kfsetting );
            if ( !result )
            {
                return false;
            }
        }

        return true;
    }

    void KFStartup::ShutDown()
    {
        _kf_library.Clear();
    }

    typedef KFPlugin* ( *PluginEntryFunction )( KFPluginManage* manage, KFGlobal* kfglobal, KFMalloc* kfmalloc );
    bool KFStartup::LoadPluginLibrary( const std::string& file, const KFAppSetting* appsetting )
    {
        auto kfglobal = KFGlobal::Instance();

        auto library = __KF_CREATE__( KFLibrary );
        if ( !library->Load( _app_config->_plugin_path, file ) )
        {
            __LOG_LOCAL__( "load [{}] failed!", library->_path );

            __KF_DESTROY__( KFLibrary, library );
            return false;
        }

        PluginEntryFunction function = ( PluginEntryFunction )library->GetFunction( "DllPluginEntry" );
        if ( function == nullptr )
        {
            __LOG_LOCAL__( "entry [{}] failed!", library->_path );

            __KF_DESTROY__( KFLibrary, library );
            return false;
        }

        // 添加library
        _kf_library.Insert( appsetting->_name, library );

        // 设置插件信息
        auto plugin = function( KFPluginManage::Instance(), KFGlobal::Instance(), KFMalloc::Instance() );
        plugin->_sort = appsetting->_sort;
        plugin->_plugin_name = appsetting->_name;
        plugin->_config = appsetting->_config_file;

        return true;
    }

    typedef void( *PluginLeaveFunction )( KFPluginManage* manage );

    bool KFStartup::UnLoadPluginLibrary( const std::string& pluginname )
    {
        auto library = _kf_library.Find( pluginname );
        if ( library == nullptr )
        {
            return false;
        }

        PluginLeaveFunction function = ( PluginLeaveFunction )library->GetFunction( "DllPluginLeave" );
        function( KFPluginManage::Instance() );

        library->UnLoad();
        _kf_library.Remove( pluginname );
        return true;
    }
}