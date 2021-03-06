﻿#ifndef __KF_OPTION_MOUDLE_H__
#define __KF_OPTION_MOUDLE_H__

/************************************************************************
//    @Module			:    全局配置模块
//    @Author           :    __凌_痕__
//    @QQ				:    7969936
//    @Mail			    :    lori227@qq.com
//    @Date             :    2017-4-24
************************************************************************/

#include "KFrame.h"
#include "KFOptionInterface.h"
#include "KFConfig/KFConfigInterface.h"

namespace KFrame
{
    class KFOptionModule : public KFOptionInterface
    {
    public:
        KFOptionModule() = default;
        ~KFOptionModule() = default;

        // 初始化
        virtual void InitModule();

        // 关闭
        virtual void BeforeShut();
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // 获得配置属性
        virtual const KFOption* FindOption( const std::string& name );
        virtual const KFOption* FindOption( const std::string& name, uint32 key );
        virtual const KFOption* FindOption( const std::string& name, const std::string& key );
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        // uint32配置
        virtual uint32 GetUInt32( const std::string& name );
        virtual uint32 GetUInt32( const std::string& name, uint32 key );
        virtual uint32 GetUInt32( const std::string& name, const std::string& key );

        // double配置
        virtual double GetDouble( const std::string& name );
        virtual double GetDouble( const std::string& name, uint32 key );
        virtual double GetDouble( const std::string& name, const std::string& key );

        // string配置
        virtual const std::string& GetString( const std::string& name );
        virtual const std::string& GetString( const std::string& name, uint32 key );
        virtual const std::string& GetString( const std::string& name, const std::string& key );

    };
}



#endif