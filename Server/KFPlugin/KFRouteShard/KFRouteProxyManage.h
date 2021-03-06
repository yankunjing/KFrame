﻿#ifndef __KF_ROUTE_PROXY_MANAGE_H__
#define __KF_ROUTE_PROXY_MANAGE_H__

#include "KFrame.h"

namespace KFrame
{
    class KFRouteProxy
    {
    public:
        KFRouteProxy()
        {
            _zone_id = 0;
            _server_id = 0;
            _handle_id = 0;
        }

        uint32 _zone_id;
        uint64 _server_id;
        uint64 _handle_id;
    };

    // 匹配服务器管理器
    class KFRouteProxyManage : public KFSingleton< KFRouteProxyManage >
    {
    public:
        KFRouteProxyManage();
        ~KFRouteProxyManage();

        // 添加游戏区
        KFRouteProxy* AddRouteProxy( uint32 zoneid, uint64 serverid, uint64 handleid );

        // 删除游戏区
        void RemoveRouteProxy( uint64 serverid );

        // 查找登录节点
        KFRouteProxy* FindRouteProxy( uint64 serverid );

    public:
        // 登录节点列表
        KFMap< uint64, uint64, KFRouteProxy > _kf_route_proxy;
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////
    static auto _kf_proxy_manage = KFRouteProxyManage::Instance();
    ///////////////////////////////////////////////////////////////////////////////////////////////
}



#endif