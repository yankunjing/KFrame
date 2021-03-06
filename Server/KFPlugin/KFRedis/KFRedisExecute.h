﻿#ifndef __KF_REDIS_EXECUTE_H__
#define __KF_REDIS_EXECUTE_H__

#include "KFRedisInterface.h"
#include "hiredis/hiredis.h"

namespace KFrame
{
    class KFRedisExecute : public KFRedisDriver
    {
    public:
        /////////////////////////////////////////////////////////////////////////////////////////////
        KFRedisExecute();
        ~KFRedisExecute();

        // 初始化
        int32 Initialize( const std::string& ip, uint32 port, const std::string& password );

        // 关闭
        void ShutDown();

        // 切换索引
        void SelectIndex( uint32 index );

        // 逻辑
        void Run();

    protected:

        // 连接
        int32 TryConnect();

        // 是否断开连接
        bool IsDisconnected();
        ////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////

        // 执行更新
        virtual KFResult< voidptr >* UpdateExecute( const std::string& strsql );

        // 查询数据库
        virtual KFResult< uint32 >* UInt32Execute( const std::string& strsql );
        virtual KFResult< uint64 >* UInt64Execute( const std::string& strsql );
        virtual KFResult< std::string >* StringExecute( const std::string& strsql );
        virtual KFResult< MapString >* MapExecute( const std::string& strsql );
        virtual KFResult< ListString >* ListExecute( const std::string& strsql );
        virtual KFResult< VectorString >* VectorExecute( const std::string& strsql );

        // 添加执行命令
        virtual void AppendCommand( const std::string& strsql );
        virtual KFResult< voidptr >* Pipeline();
        virtual KFResult< ListString >* ListPipelineExecute();
        virtual KFResult< std::list< MapString > >* ListMapPipelineExecute();

    private:
        // 执行语句
        redisReply* Execute( const std::string& strsql );
        redisReply* TryExecute( KFBaseResult* kfresult, const std::string& strsql );

    private:
        // ip
        std::string _ip;

        // 端口
        uint32 _port;

        // 密码
        std::string _password;

        // redis环境
        redisContext* _redis_context;

        // 数据库索引
        uint32 _index;

        // 需要执行的命令集
        ListString _commands;

        //////////////////////////////////////////////////////////////

        // 返回结果
        KFResultQueue< voidptr > _void_result_queue;
        KFResultQueue< uint32 > _uint32_result_queue;
        KFResultQueue< std::string > _string_result_queue;
        KFResultQueue< uint64 > _uint64_result_queue;
        KFResultQueue< MapString > _map_result_queue;
        KFResultQueue< VectorString > _vector_result_queue;
        KFResultQueue< ListString > _list_result_queue;
        KFResultQueue< std::list< MapString > > _list_map_result_queue;

        std::vector< KFBaseResultQueue* > _result_queue_list;
    };
}
#endif