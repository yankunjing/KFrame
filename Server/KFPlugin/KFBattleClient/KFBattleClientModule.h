﻿#ifndef __KF_BATTLE_CLIENT_MODULE_H__
#define __KF_BATTLE_CLIENT_MODULE_H__

/************************************************************************
//    @Module			:    战斗服务器客户端模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2018-2-22
************************************************************************/

#include "KFrame.h"
#include "KFProtocol/KFProtocol.h"
#include "KFBattleClientInterface.h"
#include "KFMessage/KFMessageInterface.h"
#include "KFPlayer/KFPlayerInterface.h"
#include "KFKernel/KFKernelInterface.h"
#include "KFOption/KFOptionInterface.h"
#include "KFClusterClient/KFClusterClientInterface.h"
#include "KFRelationClient/KFRelationClientInterface.h"

namespace KFrame
{
    class KFBattleClientModule : public KFBattleClientInterface
    {
    public:
        KFBattleClientModule() = default;
        ~KFBattleClientModule() = default;

        // 初始化
        virtual void BeforeRun();

        // 关闭
        virtual void BeforeShut();
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        // 发送消息到Battle
        virtual bool SendMessageToBattle( uint32 msgid, ::google::protobuf::Message* message );
        virtual bool SendMessageToBattle( uint64 roomid, uint32 msgid, ::google::protobuf::Message* message );

        // 查询战场
        virtual void QueryBattleRoom( uint64 playerid, uint64 roomid );

    protected:

        // 匹配结果
        __KF_MESSAGE_FUNCTION__( HandleNoticeMatchRoomReq );

        // 查询匹配
        __KF_MESSAGE_FUNCTION__( HandleQueryBattleRoomAck );

        // 离开房间
        __KF_MESSAGE_FUNCTION__( HandleLeaveBattleRoomAck );

        // 结算战绩
        __KF_MESSAGE_FUNCTION__( HandlePlayerBattleScoreReq );

    protected:
        __KF_UPDATE_DATA_FUNCTION__( OnRoomIdUpdateCallBack );

    private:
        // 玩家上线回调
        void OnEnterQueryBattleRoom( KFEntity* player );

        // 结算战绩
        void BalanceBattleScore( KFEntity* player, KFData* kfscore, KFMsg::PBBattleScore* pbscore );

        // 计算总评分
        uint32 CalcTotalScore( KFEntity* player );

    private:
        KFComponent* _kf_component{ nullptr };
    };
}



#endif