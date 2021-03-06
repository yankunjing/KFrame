﻿#ifndef __KF_ENTITY_EX_H__
#define __KF_ENTITY_EX_H__

#include "KFEntity.h"
#include "KFCore/KFData.h"
#include "KFCore/KFAgent.h"
#include "KFProtocol/KFProtocol.h"

namespace KFrame
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class KFComponentEx;
    class KFEntityEx : public KFEntity
    {
    public:
        KFEntityEx();
        ~KFEntityEx();

        // 数据
        virtual KFData* GetData();

        // selfid
        virtual void SetKeyID( uint64 id );
        virtual uint64 GetKeyID();

        virtual const char* GetName();
        virtual void SetName( const std::string& name );

        // 初始化
        void InitData( KFComponentEx* kfcomponent, const std::string& dataname );

        // 是否初始化完成
        virtual bool IsInited();
        virtual void SetInited();

        // 是否需要保存数据库
        virtual bool IsNeedToSave();
        virtual void SetNeetToSave( bool needtosave );
        //////////////////////////////////////////////////////////////////////////////////////////
        // 创建子属性
        virtual KFData* CreateData( const std::string& dataname );
        virtual KFData* CreateData( const std::string& dataname, uint64 key );

        // 添加属性
        virtual bool AddData( KFData* kfparent, KFData* kfdata );
        virtual bool AddData( KFData* kfparent, uint64 key, KFData* kfdata );
        virtual bool AddData( const std::string& parentname, uint64 key, KFData* kfdata );

        // 删除属性
        virtual bool RemoveData( const std::string& parentname, uint64 key );
        virtual bool RemoveData( KFData* kfparent, uint64 key );
        virtual bool RemoveData( const std::string& parentname );
        virtual bool RemoveData( const std::string& parentname, const std::string& dataname );

        // 更新属性
        virtual void UpdateData( const std::string& dataname, const std::string& value );
        virtual void UpdateData( const std::string& parentname, const std::string& dataname, const std::string& value );
        virtual void UpdateData( const std::string& parentname, uint64 key, const std::string& dataname, const std::string& value );
        virtual void UpdateData( KFData* kfdata, const std::string& value );
        virtual void UpdateData( KFData* kfparent, const std::string& dataname, const std::string& value );

        virtual uint64 UpdateData( const std::string& dataname, uint32 operate, uint64 value );
        virtual uint64 UpdateData( const std::string& parentname, const std::string& dataname, uint32 operate, uint64 value );
        virtual uint64 UpdateData( const std::string& parentname, uint64 key, const std::string& dataname, uint32 operate, uint64 value );

        virtual uint64 UpdateData( KFData* kfdata, uint32 operate, uint64 value );
        virtual uint64 UpdateData( KFData* kfdata, uint64 key, uint32 operate, uint64 value );
        virtual uint64 UpdateData( KFData* kfparent, const std::string& dataname, uint32 operate, uint64 value );
        virtual uint64 UpdateData( KFData* kfparent, uint64 key, const std::string& dataname, uint32 operate, uint64 value );

        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////

        // 判断元数据是否满足条件
        virtual bool CheckAgentData( const KFAgents* kfagents, const char* function, uint32 line );
        bool CheckAgentData( const KFAgent* kfagent, const char* function, uint32 line );

        // 添加元数据
        virtual void AddAgentData( const KFAgents* kfagents, float multiple, bool showclient, const char* function, uint32 line );
        void AddAgentData( const KFAgent* kfagent, float multiple, const char* function, uint32 line );

        // 删除元数据
        virtual void RemoveAgentData( const KFAgents* kfagents, const char* function, uint32 line );
        void RemoveAgentData( const KFAgent* kfagent, const char* function, uint32 line );

        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////
        // 同步更新数据
        void SyncUpdateData( KFData* kfdata, uint64 key );

        // 同步添加数据
        void SyncAddData( KFData* kfdata, uint64 key );

        // 同步删除数据
        void SyncRemoveData( KFData* kfdata, uint64 key );

        // 同步数据到客户端
        void SyncEntityToClient();
        //////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////

    protected:
        // 自动创建对象
        KFData* AutoCreateData( KFData* kfparent, uint64 key );

        // 添加元数据
        void AddNormalAgentData( KFAgent* kfagent, float multiple, const char* function, uint32 line );
        void AddRecordAgentData( KFAgent* kfagent, float multiple, const char* function, uint32 line );
        void AddObjectAgentData( KFAgent* kfagent, float multiple, const char* function, uint32 line );

        // 判断元数据
        bool CheckNormalAgentData( KFData* kfdata, KFAgent* kfagent );
        bool CheckObjectAgentData( KFAgent* kfagent );
        bool CheckRecordAgentData( KFAgent* kfagent );

        // 删除元数据
        void RemoveNormalAgentData( KFAgent* kfagent, const char* function, uint32 line );
        void RemoveRecordAgentData( KFAgent* kfagent, const char* function, uint32 line );
        void RemoveObjectAgentData( KFAgent* kfagent, const char* function, uint32 line );

    protected:
        // 同步添加数据
        void SyncAddDataToClient();

        // 同步删除
        void SyncRemoveDataToClient();

        // 同步更新数据
        void SyncUpdateDataToClient();

        // 添加属性到pb中
        void AddSyncUpdateDataToPBObject( KFData* kfdata, KFMsg::PBObject* pbobject );

        // 查找pbobject
        KFMsg::PBObject* FindObjectFromPBObject( KFData* kfdata, KFMsg::PBObject* pbobject );
        KFMsg::PBRecord* FindRecordFromPBObject( KFData* kfdata, KFMsg::PBObject* pbobject );
        KFMsg::PBObject* FindObjectFromPBRecord( KFData* kfdata, KFMsg::PBRecord* pbrecord );



    public:
        // 组件
        KFComponentEx* _kf_component;
    protected:
        // 数据
        KFData* _kf_object;

        // 名字
        std::string _name;

        // 是否初始化
        bool _is_inited;

        // 是否需要保存数据库
        bool _is_need_to_save;

        // 添加的数据
        bool _have_add_pb_object;
        KFMsg::PBObject _add_pb_object;

        // 删除的数据
        bool _have_remove_pb_object;
        KFMsg::PBObject _remove_pb_object;

        // 更新的数据
        bool _have_update_pb_object;
        KFMsg::PBObject _update_pb_object;
    };
}

#endif