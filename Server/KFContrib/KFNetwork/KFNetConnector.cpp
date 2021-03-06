﻿#include "KFNetConnector.h"
#include "KFDefine.h"
#include "KFNetServices.h"

namespace KFrame
{
    KFNetConnector::KFNetConnector()
    {
        _net_services = nullptr;
    }

    KFNetConnector::~KFNetConnector()
    {
        _net_services = nullptr;
    }

    void KFNetConnector::InitConnector( uint64 id, KFNetServices* netservices )
    {
        _net_services = netservices;

        // 消息头长度
        uint32 headlength = ( netservices->_message_type == KFMessageEnum::Server ? sizeof( KFServerHead ) : sizeof( KFClientHead ) );
        InitSession( id, netservices->_queue_size, headlength );
    }

    // 弹出一个消息
    KFNetMessage* KFNetConnector::PopMessage()
    {
        auto message = _recv_queue.Front();
        if ( message == nullptr )
        {
            return nullptr;
        }

        KFNetMessage* retmessage = nullptr;
        switch ( message->_msgid )
        {
        case KFNetDefine::CUT_MSGCHILDBEGIN:	// 子消息头
        {
            // 这里的子消息没有包括消息头
            auto childcount = message->_child;
            auto queuesize = _recv_queue.Size();
            if ( queuesize >= ( childcount + 1u ) )
            {
                // 不能强转成KFNetMessage, headmessage->_data的长度只包括KFNetHead

                // 重新计算buff大小
                auto tempmessage = reinterpret_cast< KFNetHead* >( message->_data );
                auto totallength = tempmessage->_length + static_cast< uint32 >( sizeof( KFNetMessage ) );
                auto buffaddress = _net_services->GetBuffAddress( tempmessage->_msgid, totallength );

                if ( _net_services->_buff_length < totallength )
                {
                    // 长度异常, 直接丢弃
                    _recv_queue.PopRemove();
                }
                else
                {
                    // 先将消息头拷贝过去
                    memcpy( buffaddress, message->_data, message->_length );
                    _recv_queue.PopRemove();

                    retmessage = reinterpret_cast< KFNetMessage* >( buffaddress );
                    retmessage->_data = buffaddress + sizeof( KFNetMessage );

                    // 合并子消息
                    auto copylength = 0u;
                    auto leftlength = _net_services->_buff_length - sizeof( KFNetMessage );

                    for ( auto i = 0u; i < childcount; ++i )
                    {
                        auto childmessage = _recv_queue.Front();

                        // 不是子消息, 直接返回null
                        if ( childmessage == nullptr || childmessage->_msgid != KFNetDefine::CUT_MSGCHILD )
                        {
                            return nullptr;
                        }

                        // 长度不足, 返回null
                        if ( leftlength < childmessage->_length )
                        {
                            _recv_queue.PopRemove();
                            return nullptr;
                        }

                        memcpy( retmessage->_data + copylength, childmessage->_data, childmessage->_length );
                        copylength += childmessage->_length;
                        leftlength -= childmessage->_length;

                        _recv_queue.PopRemove();
                    }
                }

                retmessage->_kfid._head_id = _object_id;
            }
            else
            {
                // 如果超出了最大的队列长度
                if ( childcount >= _recv_queue.Capacity() )
                {
                    _recv_queue.PopRemove();
                }
            }
        }
        break;
        case KFNetDefine::CUT_MSGCHILD:		// 如果取到的是子消息, 直接丢掉
        {
            _recv_queue.PopRemove();
        }
        break;
        default:	// 不是拆包消息, 直接返回
        {
            if ( message->_length + sizeof( KFNetMessage ) <= _net_services->_buff_length )
            {
                retmessage = reinterpret_cast< KFNetMessage* >( _net_services->_buff_address );
                retmessage->_data = _net_services->_buff_address + sizeof( KFNetMessage );
                retmessage->CopyFrom( message );
            }

            _recv_queue.PopRemove();
        }
        break;
        }

        return retmessage;
    }

    // 添加一个发送消息
    bool KFNetConnector::SendSingleMessage( uint64 objectid, uint32 msgid, const char* data, uint32 length )
    {
        auto netmessage = KFNetMessage::Create( length );

        KFId kfid( 0, objectid );
        netmessage->CopyFrom( kfid, msgid, data, length );
        return AddSendMessage( netmessage );
    }

    bool KFNetConnector::SendMultiMessage( uint64 objectid, uint32 msgid, const char* data, uint32 length )
    {
        auto ok = true;

        // 子消息个数
        uint32 datalength = length;
        uint32 messagecount = ( datalength + KFNetDefine::MaxMessageLength - 1 ) / KFNetDefine::MaxMessageLength;

        // 消息头
        KFId kfid( 0, objectid );
        auto message = reinterpret_cast< KFNetMessage* >( _net_services->_buff_address );
        message->CopyFrom( kfid, msgid, nullptr, length );

        // 子消息头
        auto headmessage = KFNetMessage::Create( KFNetMessage::HeadLength() );
        headmessage->_child = messagecount;
        headmessage->CopyFrom( kfid, KFNetDefine::CUT_MSGCHILDBEGIN, _net_services->_buff_address, KFNetMessage::HeadLength() );
        if ( !AddSendMessage( headmessage ) )
        {
            ok = false;
        }

        // 子消息
        uint32 copydatalength = 0;
        for ( auto i = 0u; i < messagecount; ++i )
        {
            //  需要发送的数据长度
            auto sendlength = __MIN__( datalength, KFNetDefine::MaxMessageLength );

            // 消息拷贝
            auto childmessage = KFNetMessage::Create( sendlength );
            childmessage->_child = i + 1;
            childmessage->CopyFrom( kfid, KFNetDefine::CUT_MSGCHILD, data + copydatalength, sendlength );
            if ( !AddSendMessage( childmessage ) )
            {
                ok = false;
            }

            // 游标设置
            datalength -= sendlength;
            copydatalength += sendlength;
        }

        return ok;
    }

    KFNetMessage* KFNetConnector::PopNetMessage()
    {
        auto message = PopMessage();
        if ( message != nullptr )
        {
            // 解密
        }

        return message;
    }

    bool KFNetConnector::SendNetMessage( uint32 msgid, const char* data, uint32 length )
    {
        return SendNetMessage( 0, msgid, data, length );
    }

    bool KFNetConnector::SendNetMessage( uint64 objectid, uint32 msgid, const char* data, uint32 length )
    {
        bool ok = false;

        // 小于发送的最大buff, 直接添加消息
        if ( length <= KFNetDefine::MaxMessageLength )
        {
            ok = SendSingleMessage( objectid, msgid, data, length );
        }
        else
        {
            ok = SendMultiMessage( objectid, msgid, data, length );
        }

        // 发送消息
        _net_services->SendNetMessage( this );
        _last_message_time = _net_services->_now_time;
        return ok;
    }

    void KFNetConnector::RunUpdate( KFNetFunction& netfunction, uint32 maxcount )
    {
        // 处理消息
        RunMessage( netfunction, maxcount );

        // ping 消息
        RunSendPing();
    }

    void KFNetConnector::RunSendPing()
    {
        // 20秒没有消息通讯, 发送一次ping消息
        // keeplive 经常会失灵, 很久才检测到断开 问题待查
        if ( _net_services->_now_time < _last_message_time + 20000u )
        {
            return;
        }

        SendNetMessage( 0, nullptr, 0 );
    }

    void KFNetConnector::RunMessage( KFNetFunction& netfunction, uint32 maxcount )
    {
        auto messagecount = _invalid_int;
        auto message = PopNetMessage();
        while ( message != nullptr )
        {
            // 处理回调函数
            netfunction( message->_kfid, message->_msgid, message->_data, message->_length );

            // 每次处理200个消息
            ++messagecount;
            if ( messagecount >= maxcount )
            {
                break;
            }
            message = PopNetMessage();
        }

        // 设置处理消息时间
        if ( messagecount != _invalid_int )
        {
            _last_message_time = _net_services->_now_time;
        }
    }
}
