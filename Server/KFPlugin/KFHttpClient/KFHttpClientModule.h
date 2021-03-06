﻿#ifndef __KF_HTTP_CLIENT_MODULE_H__
#define __KF_HTTP_CLIENT_MODULE_H__

/************************************************************************
//    @Module			:    http客户端
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-1-10
************************************************************************/

#include "KFrame.h"
#include "KFHttpClientInterface.h"
#include "KFHttp/KFHttpData.h"
#include "KFHttp/KFHttpClient.h"
#include "KFHttp/KFHttpsClient.h"

namespace KFrame
{
    class KFHttpClientModule : public KFHttpClientInterface
    {
    public:
        KFHttpClientModule();
        ~KFHttpClientModule();

        // 逻辑
        virtual void BeforeRun();
        virtual void Run();

        virtual void BeforeShut();
        /////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////

        virtual std::string StartSTHttpClient( const std::string& url, const std::string& data );
        virtual std::string StartSTHttpClient( const std::string& url, KFJson& json );

        virtual void StartMTHttpClient( const std::string& url, const std::string& data );
        virtual void StartMTHttpClient( const std::string& url, KFJson& json );

        /////////////////////////////////////////////////////////////////////
        // 返回错误
        virtual std::string SendResponseCode( uint32 code );

        // 获得错误码
        virtual uint32 GetResponseCode( KFJson& json );

        // 发送json
        virtual std::string SendResponse( KFJson& json );

    protected:
        // http
        virtual void StartMTHttpClient( KFHttpClientFunction& function, const std::string& url, const std::string& data, const std::string& callback );
        virtual void StartMTHttpClient( KFHttpClientFunction& function, const std::string& url, KFJson& json, const std::string& callback );

    private:
        // 添加异步请求
        void AddHttpData( KFHttpData* httpdata );

        // http请求
        void RunHttpRequest();

        // 判断是否是https
        bool IsHttpsClient( const std::string& url );

    private:

        volatile bool _thread_run;

        // 请求的数据队列
        KFMutex _kf_req_mutex;
        std::list< KFHttpData* > _req_http_data;

        // 完成的数据队列
        KFMutex _kf_ack_mutex;
        std::list< KFHttpData* > _ack_http_data;
    };
}



#endif