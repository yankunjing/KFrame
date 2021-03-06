﻿#ifndef __KF_SERVICES_H__
#define __KF_SERVICES_H__

#include "KFrame.h"

namespace KFrame
{
    class KFApplication;
    class KFServices : public KFSingleton< KFServices >
    {
    public:
        // 构造
        KFServices();
        ~KFServices();

        // run
        void Run();

        // 初始化服务
        bool InitService( KFApplication* application, MapString& params );

        // 自动更新
        void RunUpdate();

        // 关闭服务
        void ShutDown();

        // 是否已经关闭
        bool IsShutDown();

    protected:
        ////////////////////////////////////////////////////////////////////////////
        // 初始化定时器
        void InitLogMemoryTimer();

        // 打印内存信息
        void PrintLogMemory();

        // 解析appid
        void ParseAppId( std::string strappid );

    protected:
        // 进程
        KFApplication* _application;

        // 打印定时器
        KFClockTimer _memory_timer;
    };
}
#endif
