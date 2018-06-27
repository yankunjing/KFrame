﻿#ifndef __KF_MATCH_SHARD_CONFIG_H__
#define __KF_MATCH_SHARD_CONFIG_H__

#include "KFrame.h"
#include "KFConfig/KFConfigInterface.h"

namespace KFrame
{
	__ST_CLASS__( KFMatchSetting )
	{
	public:
		KFMatchSetting()
		{
			_match_id = 1;
			_max_player_count = 100;
			_min_group_player_count = 1;
			_max_group_player_count = 3;
			_max_room_wait_time = 0;
			_wait_confirm_time = 10000;
		}

		bool IsOpen( uint32 serverid ) const
		{
			return _open_server_list.find( serverid ) != _open_server_list.end();
		}

	public:
		// 匹配模式id
		uint32 _match_id;

		// 队伍的最小人数
		uint32 _min_group_player_count;

		// 队伍的最大人数
		uint32 _max_group_player_count;

		// 最大的玩家数量
		uint32 _max_player_count;

		// 等待时间( 毫秒 )
		uint32 _max_room_wait_time;
		
		// 重复请求间隔时间( 毫秒 )
		uint32 _wait_confirm_time;

		// 服务器列表
		std::set< uint32 > _open_server_list;
	};

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	class KFMatchShardConfig : public KFConfig, public KFSingleton< KFMatchShardConfig >
	{
	public:
		KFMatchShardConfig();
		~KFMatchShardConfig();

		// 加载配置
		bool LoadConfig( const char* file );

		// 查找匹配设置
		const KFMatchSetting* FindMatchSetting( uint32 matchid );

	public:
		// 匹配配置列表
		KFMap< uint32, uint32, KFMatchSetting > _kf_match_setting;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////
	static auto _kf_match_config = KFMatchShardConfig::Instance();
	//////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif