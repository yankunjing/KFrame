﻿#ifndef __KF_RELATION_CLIENT_MODULE_H__
#define __KF_RELATION_CLIENT_MODULE_H__

/************************************************************************
//    @Module			:    好友客户端模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2018-3-8
************************************************************************/

#include "KFrame.h"
#include "KFProtocol/KFProtocol.h"
#include "KFRelationClientInterface.h"
#include "KFOption/KFOptionInterface.h"
#include "KFPlayer/KFPlayerInterface.h"
#include "KFConfig/KFConfigInterface.h"
#include "KFKernel/KFKernelInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFDisplay/KFDisplayInterface.h"
#include "KFMailClient/KFMailClientInterface.h"
#include "KFRouteClient/KFRouteClientInterface.h"
#include "KFPublicClient/KFPublicClientInterface.h"
#include "KFClusterClient/KFClusterClientInterface.h"

namespace KFrame
{
    class KFComponent;
    class KFRelationClientModule : public KFRelationClientInterface
    {
    public:
        KFRelationClientModule() = default;
        ~KFRelationClientModule() = default;

        // 初始化
        virtual void BeforeRun();
        virtual void OnceRun();

        // 关闭
        virtual void BeforeShut();
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // 添加好友度 （目前只支持增加）
        virtual void AddFriendLiness( KFEntity* player, uint64 friendid, uint32 type, uint32 value );
        virtual void AddFriendLinessOnce( KFEntity* player, uint64 friendid, uint32 type, uint32 value );

        // 发送消息到关系属性
        virtual bool SendMessageToRelation( KFData* kfrelation, uint32 msgid, google::protobuf::Message* message );

        // 发送消息到关系集群
        virtual bool SendMessageToRelation( uint32 msgid, ::google::protobuf::Message* message );

        // 计算战场成绩, 好友度/ 最近的玩家战绩
        virtual void BalanceBattleRelation( KFEntity* player, uint64 roomid, const KFMsg::PBBattleScore* pbscore );

    protected:
        // 上线查询好友数据
        void OnEnterQueryFriend( KFEntity* player );

        // 下线通知
        void OnLeaveUpdateFriend( KFEntity* player );
    protected:
        // 查询好友回馈
        __KF_MESSAGE_FUNCTION__( HandleQueryFriendAck );

        // 查询好友申请回馈
        __KF_MESSAGE_FUNCTION__( HandleQueryInviteAck );

        // 申请添加好友请求
        __KF_MESSAGE_FUNCTION__( HandleAddFriendInviteReq );

        // 申请添加好友请求
        __KF_MESSAGE_FUNCTION__( HandleAddFriendInviteAck );

        // 删除好友请求
        __KF_MESSAGE_FUNCTION__( HandleDelFriendReq );

        // 删除好友回馈
        __KF_MESSAGE_FUNCTION__( HandleDelFriendAck );

        // 好友申请回复
        __KF_MESSAGE_FUNCTION__( HandleReplyInviteReq );

        // 添加好友
        __KF_MESSAGE_FUNCTION__( HandleAddFriendAck );

        // 更新好友
        __KF_MESSAGE_FUNCTION__( HandleUpdateFriendReq );

        // 拒绝好友申请设置
        __KF_MESSAGE_FUNCTION__( HandleSetRefuseFriendInviteReq );

        // 更新好感度
        __KF_MESSAGE_FUNCTION__( HandleUpdateFriendLinessAck );

        // 查询最近游戏列表请求
        __KF_MESSAGE_FUNCTION__( HandleQueryRecentListReq );

        // 更新最近游戏列表
        __KF_MESSAGE_FUNCTION__( HandleQueryRecentListAck );

        // 处理玩家敬酒请求
        __KF_MESSAGE_FUNCTION__( HandlePlayerToastReq );

        // 处理玩家敬酒回馈
        __KF_MESSAGE_FUNCTION__( HandlePlayerToastAck );

        // 查询总的被敬酒次数
        __KF_MESSAGE_FUNCTION__( HandleQueryToastCountReq );

    protected:

        // 属性更新回调
        __KF_UPDATE_DATA_FUNCTION__( OnRelationValueUpdate );
        __KF_UPDATE_STRING_FUNCTION__( OnRelationStringUpdate );

    private:
        // 解析好友信息
        void PBRelationToKFData( const KFMsg::PBRelation* pbfriend, KFData* kffriend );

        // 好友申请操作
        void ReplyFriendInvite( KFEntity* player, uint32 operate );
        void ReplyFriendInvite( KFEntity* player, uint64 playerid, uint32 operate );
        uint64 ReplyFriendInvite( KFEntity* player, KFData* kfinvite, uint32 operate );

        // 添加好友
        void AddFriend( KFEntity* player, KFData* kfinvite );

        // 发送好友更新消息
        void SendUpdateToFriend( KFEntity* player, MapString& values );

        // 结算添加好友度
        void BalanceFriendLiness( KFEntity* player, const KFMsg::PBBattleScore* pbscore );

        // 添加最近游戏的人
        void AddRecentPlayer( KFEntity* player, uint64 roomid, const KFMsg::PBBattleScore* pbscore );
    private:
        // 玩家组件
        KFComponent* _kf_component{ nullptr };
    };
}



#endif