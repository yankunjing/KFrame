﻿#ifndef __KF_DEPLOY_SERVER_MODULE_H__
#define __KF_DEPLOY_SERVER_MODULE_H__


/************************************************************************
//    @Module			:    部署Server
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2018-7-2
************************************************************************/

#include "KFDeployServerInterface.h"
#include "KFMySQL/KFMySQLInterface.h"
#include "KFConfig/KFConfigInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFSchedule/KFScheduleInterface.h"
#include "KFTcpServer/KFTcpServerInterface.h"
#include "KFHttpServer/KFHttpServerInterface.h"
#include "KFHttpClient/KFHttpClientInterface.h"

namespace KFrame
{
    class KFAgentData
    {
    public:
        KFAgentData()
        {
            _port = 0;
            status = 0;
        }

    public:
        // 服务器id
        std::string _agent_id;

        // 局域网地址
        std::string _local_ip;

        // 名字
        std::string _name;

        // 类型
        std::string _type;

        // 端口
        uint32 _port;

        // 状态
        uint32 status;
    };


    class KFDeployServerModule : public KFDeployServerInterface
    {
    public:
        KFDeployServerModule() = default;
        ~KFDeployServerModule() = default;

        // 逻辑
        virtual void BeforeRun();
        virtual void OnceRun();

        // 关闭
        virtual void ShutDown();

    protected:
        // 连接丢失
        __KF_SERVER_LOST_FUNCTION__( OnServerLostClient );

        // 启动服务器
        __KF_HTTP_FUNCTION__( HandleDeployCommand );

        // 部署命令
        __KF_SCHEDULE_FUNCTION__( OnDeployCommandToAgent );

    protected:
        // 注册Agent
        __KF_MESSAGE_FUNCTION__( HandleRegisterAgentToServerReq );

        // 请求分配Agent连接
        __KF_MESSAGE_FUNCTION__( HandleGetAgentIpAddressReq );

    protected:
        // 更新Agnet状态
        void UpdateAgentToDatabase( KFAgentData* kfagent, uint32 status );

        // 回发日志消息
        template<typename... P>
        void LogDeploy( const std::string& url, const char* myfmt, P&& ... args )
        {
            auto msg = __FORMAT__( myfmt, std::forward<P>( args )... );
            return SendLogMessage( url, msg );
        }

        void SendLogMessage( const std::string& url, const std::string& msg );

    private:
        KFMySQLDriver* _mysql_driver{ nullptr };

        // Agent列表
        KFMap< std::string, const std::string&, KFAgentData > _agent_list;
    };
}

#endif