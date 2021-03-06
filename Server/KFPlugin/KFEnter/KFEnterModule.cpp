﻿#include "KFEnterModule.h"
#include "KFEnterConfig.h"

namespace KFrame
{
    void KFEnterModule::InitModule()
    {
        __KF_ADD_CONFIG__( _kf_enter_config, true );
    }

    void KFEnterModule::BeforeRun()
    {
        _kf_player->RegisterEnterFunction( this, &KFEnterModule::EnterGameWorld );
    }

    void KFEnterModule::BeforeShut()
    {
        __KF_REMOVE_CONFIG__( _kf_enter_config );
        _kf_player->UnRegisterEnterFunction( this );
    }

    void KFEnterModule::EnterGameWorld( KFEntity* player )
    {
        // 上线时间
        auto kfobject = player->GetData();
        player->UpdateData( __KF_STRING__( onlinetime ), KFOperateEnum::Set, KFGlobal::Instance()->_real_time );
        ////////////////////////////////////////////////////////////////////////////////
        auto playerid = player->GetKeyID();
        auto kfnoteparent = kfobject->FindData( __KF_STRING__( note ) );
        for ( auto& iter : _kf_enter_config->_kf_enter_setting )
        {
            auto kfsetting = &iter;
            if ( kfsetting->_note_id != _invalid_int )
            {
                auto notevalue = kfnoteparent->GetValue( kfsetting->_note_id, __KF_STRING__( value ) );
                if ( notevalue != _invalid_int )
                {
                    continue;
                }

                // 设置属性
                player->UpdateData( kfnoteparent, kfsetting->_note_id, __KF_STRING__( value ), KFOperateEnum::Set, 1 );
            }

            // 调用脚本
            _kf_lua->Call( kfsetting->_lua_file, kfsetting->_lua_function, playerid );
        }
    }
}