﻿#ifndef __KF_LUA_MODULE_H__
#define __KF_LUA_MODULE_H__

/************************************************************************
//    @Module			:    Lua模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-1-22
************************************************************************/

#include "KFrame.h"
#include "KFLuaScript.h"
#include "KFLuaInterface.h"
#include "KFKernel/KFKernelInterface.h"
#include "KFOption/KFOptionInterface.h"
#include "KFPlatform/KFPlatformInterface.h"
#include "KFHttpClient/KFHttpClientInterface.h"
#include "KFDeployCommand/KFDeployCommandInterface.h"

namespace KFrame
{
    class KFLuaModule : public KFLuaInterface
    {
    public:
        KFLuaModule() = default;
        ~KFLuaModule() = default;

        // 初始化
        virtual void BeforeRun();

        // 关闭
        virtual void ShutDown();
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // 调用lua函数
        virtual void Call( const std::string& luafile, const std::string& function, uint64 objectid );
        virtual void Call( const std::string& luafile, const std::string& function, uint64 objectid,
                           uint64 param1 );
        virtual void Call( const std::string& luafile, const std::string& function, uint64 objectid,
                           uint64 param1, uint64 param2 );
        virtual void Call( const std::string& luafile, const std::string& function, uint64 objectid,
                           uint64 param1, uint64 param2, uint64 param3 );
        virtual void Call( const std::string& luafile, const std::string& function, uint64 objectid,
                           uint64 param1, uint64 param2, uint64 param3, uint64 param4 );
        virtual void Call( const std::string& luafile, const std::string& function, uint64 objectid,
                           uint64 param1, uint64 param2, uint64 param3, uint64 param4, uint64 param5 );
        virtual void Call( const std::string& luafile, const std::string& function, uint64 objectid,
                           uint64 param1, uint64 param2, uint64 param3, uint64 param4, uint64 param5, uint64 param6 );
        ////////////////////////////////////////////////////////////////////////////////

        virtual void Call( const std::string& luafile, const std::string& function, uint64 objectid,
                           const std::string& param1 );
        virtual void Call( const std::string& luafile, const std::string& function, uint64 objectid,
                           const std::string& param1, const std::string& param2 );
        virtual void Call( const std::string& luafile, const std::string& function, uint64 objectid,
                           const std::string& param1, const std::string& param2, const std::string& param3 );


        virtual void Call( const std::string& luafile, const std::string& function, uint64 objectid,
                           const std::string& param1, const std::string& param2, const std::string& param3,
                           const std::string& param4 );

        virtual void Call( const std::string& luafile, const std::string& function, uint64 objectid,
                           const std::string& param1, const std::string& param2, const std::string& param3,
                           const std::string& param4, const std::string& param5 );

        virtual void Call( const std::string& luafile, const std::string& function, uint64 objectid,
                           const std::string& param1, const std::string& param2, const std::string& param3,
                           const std::string& param4, const std::string& param5, const std::string& param6 );
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        // md5
        const char* LuaMd5Encode( const char* data );

        // 日志记录
        void LuaLogDebug( const char* data );
        void LuaLogInfo( const char* data );
        void LuaLogWarn( const char* data );
        void LuaLogError( const char* data );
        void LuaLogCritical( const char* data );

        // 获得基础属性
        uint64 LuaGetValue( uint64 objectid, const char* dataname );

        // 操作属性
        uint64 LuaOperateValue( uint64 objectid, const char* dataname, uint32 operate, uint64 value );

        // 获得对象属性
        uint64 LuaGetObjectValue( uint64 objectid, const char* parentname, const char* dataname );

        // 操作对象属性
        uint64 LuaOperateObjectValue( uint64 objectid, const char* parentname, const char* dataname, uint32 operate, uint64 value );

        // 获得数组属性
        uint64 LuaGetRecordValue( uint64 objectid, const char* parentname, uint64 key, const char* dataname );

        // 操作数组属性
        uint64 LuaOperateRecordValue( uint64 objectid, const char* parentname, uint64 key, const char* dataname, uint32 operate, uint64 value );

        // 获得运行时变量
        uint32 LuaGetVariable( uint32 varid );

        // 操作运行时变量
        uint32 LuaOperateVariable( uint32 varid, uint32 operate, uint32 value );

        // 获得游戏时间
        uint64 LuaGetGameTime();

        // 获得现实时间
        uint64 LuaGetRealTime();

        // 添加数据
        void LuaAddData( uint64 objectid, const char* stragent, bool showclient );

        // 设置数据
        void LuaSetData( uint64 objectid, const char* stragent, bool showclient );

        // 减少数据
        void LuaDecData( uint64 objectid, const char* stragent );

        // http访问
        const char* LuaSTHttpClient( const char* url, const char* data );
        void LuaMTHttpClient( const char* url, const char* data, const char* callback );

        // 获得配置变量
        const char* LuaGetOptionString( const char* name, const char* logicid );
        uint32 LuaGetOptionUint32( const char* name, const char* logicid );

        // 创建平台地址
        const char* LuaMakePlatformUrl( const char* path );

    protected:
        // 重新加载脚本文件
        __KF_COMMAND_FUNCTION__( LoadScript );
        ////////////////////////////////////////////////////////////////////////////////
        // 查找lua脚本
        KFLuaScript* FindLuaScript( const std::string& luafile );

        // 注册lua导出函数
        void RegisterLuaFunction( KFLuaScript* luascript );

        // lua http回调函数
        __KF_HTTP_CALL_BACK_FUNCTION__( OnLuaHttpCallBack );

    private:
        // lua 脚本
        KFMap< std::string, const std::string&, KFLuaScript > _lua_script;

        // 运行时的一些变量
        std::map< uint32, uint32 > _run_variable;

        // 玩家组件
        KFComponent* _kf_player;
    };
}



#endif