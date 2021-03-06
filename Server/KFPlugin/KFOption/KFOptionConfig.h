﻿#ifndef __KF_OPTION_CONFIG_H__
#define __KF_OPTION_CONFIG_H__

#include "KFrame.h"
#include "KFOptionInterface.h"
#include "KFConfig/KFConfigInterface.h"

namespace KFrame
{
    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    class KFOptionConfig : public KFConfig, public KFSingleton< KFOptionConfig >
    {
    public:
        KFOptionConfig() = default;
        ~KFOptionConfig() = default;

        // 加载配置
        bool LoadConfig();

        // 查找配置选项
        const KFOption* FindOption( const std::string& name, const std::string& key ) const;

    public:
        // 选项配置列表
        typedef std::pair< std::string, std::string > OptionKey;
        KFMap< OptionKey, const OptionKey&, KFOption > _option_list;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////
    static auto _kf_option_config = KFOptionConfig::Instance();
    //////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif