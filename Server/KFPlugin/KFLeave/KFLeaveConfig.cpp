﻿#include "KFLeaveConfig.h"

namespace KFrame
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    bool KFLeaveConfig::LoadConfig()
    {
        _kf_leave_setting.clear();
        //////////////////////////////////////////////////////////////////
        KFXml kfxml( _file );
        auto config = kfxml.RootNode();
        auto node = config.FindNode( "Setting" );
        while ( node.IsValid() )
        {
            auto channelid = node.GetUInt32( "ChannelId" );
            auto service = node.GetUInt32( "Service" );
            if ( KFGlobal::Instance()->CheckChannelService( channelid, service ) )
            {
                KFLeaveSetting setting;
                setting._note_id = node.GetUInt32( "NoteId" );
                setting._lua_file = node.GetString( "LuaFile" );
                setting._lua_function = node.GetString( "LuaFunction" );

                _kf_leave_setting.push_back( setting );
            }

            node.NextNode();
        }
        //////////////////////////////////////////////////////////////////

        return true;
    }
}