﻿#include "KFObject.h"

namespace KFrame
{
    KFObject::KFObject()
    {
        _key = 0;
        _class_setting = nullptr;
    }

    KFObject::~KFObject()
    {
        _class_setting = nullptr;
    }

    uint64 KFObject::GetKeyID()
    {
        if ( _key == 0 )
        {
            _key = GetValue< uint64 >( _data_setting->_key_name );
        }

        return _key;
    }

    void KFObject::SetKeyID( uint64 id )
    {
        _key = id;
        SetValue<uint64>( _data_setting->_key_name, id );
    }

    uint32 KFObject::GetType() const
    {
        return KFDataDefine::Type_Object;
    }

    void KFObject::Reset()
    {
        _key = _invalid_int;

        auto child = _data.First();
        while ( child != nullptr )
        {
            child->Reset();
            child = _data.Next();
        }
    }

    bool KFObject::IsValid()
    {
        return true;
    }

    uint32 KFObject::Size()
    {
        return _data.Size();
    }

    KFData* KFObject::FirstData()
    {
        return _data.First();
    }

    KFData* KFObject::NextData()
    {
        return _data.Next();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////
    void KFObject::CopyFrom( KFData* kfother )
    {
        SetKeyID( kfother->GetKeyID() );

        for ( auto iter : _data._objects )
        {
            auto& key = iter.first;
            auto kfdata = iter.second;

            auto finddata = kfother->FindData( key );
            if ( finddata != nullptr )
            {
                kfdata->CopyFrom( finddata );
            }
        }
    }

    void KFObject::SaveTo( KFData* kfother )
    {
        kfother->CopyFrom( this );
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    KFData* KFObject::FindData( const std::string& dataname )
    {
        return _data.Find( dataname );
    }

    KFData* KFObject::FindData( const std::string& dataname, uint64 key )
    {
        auto kfdata = FindData( dataname );
        if ( kfdata == nullptr )
        {
            return nullptr;
        }

        return kfdata->FindData( key );
    }

    KFData* KFObject::FindData( const std::string& parentname, const std::string& childname )
    {
        if ( parentname.empty() )
        {
            return FindData( childname );
        }

        auto kfdata = FindData( parentname );
        if ( kfdata == nullptr )
        {
            return nullptr;
        }

        return kfdata->FindData( childname );
    }

    KFData* KFObject::FindData( const std::string& parentname, uint64 key, const std::string& childname )
    {
        auto kfdata = FindData( parentname );
        if ( kfdata == nullptr )
        {
            return nullptr;
        }

        return kfdata->FindData( key, childname );
    }

    KFData* KFObject::FindData( const std::string& parentname, const std::string& childname, uint64 key )
    {
        auto kfdata = FindData( parentname );
        if ( kfdata == nullptr )
        {
            return nullptr;
        }

        return kfdata->FindData( childname, key );
    }

    KFData* KFObject::FindData( const std::string& parentname, const std::string& childname, const std::string& dataname )
    {
        auto kfdata = FindData( parentname );
        if ( kfdata == nullptr )
        {
            return nullptr;
        }

        return kfdata->FindData( childname, dataname );
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool KFObject::AddData( const std::string& dataname, KFData* data )
    {
        data->SetParent( this );
        _data.Insert( dataname, data );
        return true;
    }

    bool KFObject::AddData( const std::string& dataname, uint64 key, KFData* data )
    {
        auto kfdata = FindData( dataname );
        if ( kfdata == nullptr )
        {
            return false;
        }

        return kfdata->AddData( key, data );
    }


    bool KFObject::AddData( const std::string& parentname, const std::string& childname, KFData* data )
    {
        auto kfdata = FindData( parentname );
        if ( kfdata == nullptr )
        {
            return false;
        }

        return kfdata->AddData( childname, data );
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool KFObject::RemoveData( const std::string& dataname )
    {
        return _data.Remove( dataname );
    }

    bool KFObject::RemoveData( const std::string& dataname, uint64 key )
    {
        auto kfdata = FindData( dataname );
        if ( kfdata == nullptr )
        {
            return false;
        }

        return kfdata->RemoveData( key );
    }

    bool KFObject::RemoveData( const std::string& parentname, const std::string& childname )
    {
        auto kfdata = FindData( parentname );
        if ( kfdata == nullptr )
        {
            return false;
        }

        return kfdata->RemoveData( childname );
    }

    std::string KFObject::ToString()
    {
        // {money:10000:money,login:{day:1,reward:2}:login,item:[1={xxxx},2={xxxx}]:item}

        std::string result = "";
        result += "{";

        for ( auto& iter : _data._objects )
        {
            auto& key = iter.first;
            auto kfdata = iter.second;

            auto strdata = kfdata->ToString();
            auto temp = __FORMAT__( "{}:{}:{},", key, strdata, key );
            result += temp;
        }

        result += "}";

        return result;
    }

    void KFObject::FromString( const std::string& value )
    {
        auto temp = value;
        KFUtility::SplitString( temp, "{" );

        while ( !temp.empty() )
        {
            auto dataname = KFUtility::SplitString( temp, ":" );

            auto split = ":" + dataname;
            auto data = KFUtility::SplitString( temp, split );

            auto kfdata = FindData( dataname );
            if ( kfdata != nullptr )
            {
                kfdata->FromString( data );
            }

            KFUtility::SplitString( temp, "," );
        }
    }

    void KFObject::ToMap( MapString& values )
    {
        for ( auto& iter : _data._objects )
        {
            auto& key = iter.first;
            auto kfdata = iter.second;

            values[ key ] = kfdata->ToString();
        }
    }

    void KFObject::FromMap( const MapString& values )
    {
        for ( auto iter : values )
        {
            auto kfdata = FindData( iter.first );
            if ( kfdata == nullptr )
            {
                continue;
            }

            kfdata->FromMap( values );
        }
    }
}