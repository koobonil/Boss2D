#include <boss.h>
#if BOSS_NEED_ADDON_WEBSOCKET

#include "boss_integration_websocketpp-0.8.1.hpp"
#include <platform/boss_platform.hpp>

#define BOOST_ALL_NO_LIB

#include <addon/websocketpp-0.8.1_for_boss/websocketpp/config/asio_client.hpp>
#include <addon/websocketpp-0.8.1_for_boss/websocketpp/config/asio_no_tls_client.hpp>
#include <addon/websocketpp-0.8.1_for_boss/websocketpp/client.hpp>

using namespace websocketpp;

namespace BOSS
{
    typedef client<config::asio_client> ClientType;
    typedef client<config::asio_tls_client> TlsClientType;
    typedef lib::shared_ptr<lib::asio::ssl::context> ContextPtr;

    static bool verify_subject_alternative_name(const char* hostname, X509* cert)
    {
        STACK_OF(GENERAL_NAME) * san_names = NULL;
        san_names = (STACK_OF(GENERAL_NAME) *) X509_get_ext_d2i(cert, NID_subject_alt_name, NULL, NULL);
        if(san_names == NULL) return false;

        int san_names_count = sk_GENERAL_NAME_num(san_names);
        bool result = false;
        for(int i = 0; i < san_names_count; i++)
        {
            const GENERAL_NAME * current_name = sk_GENERAL_NAME_value(san_names, i);
            if(current_name->type != GEN_DNS)
                continue;
            char* dns_name = (char*) ASN1_STRING_data(current_name->d.dNSName);
            if(ASN1_STRING_length(current_name->d.dNSName) != strlen(dns_name))
                break;
            result = (boss_stricmp(hostname, dns_name) == 0);
        }
        sk_GENERAL_NAME_pop_free(san_names, GENERAL_NAME_free);
        return result;
    }

    static bool verify_common_name(const char * hostname, X509 * cert)
    {
        int common_name_loc = X509_NAME_get_index_by_NID(X509_get_subject_name(cert), NID_commonName, -1);
        if(common_name_loc < 0) return false;

        X509_NAME_ENTRY * common_name_entry = X509_NAME_get_entry(X509_get_subject_name(cert), common_name_loc);
        if(common_name_entry == NULL) return false;

        ASN1_STRING * common_name_asn1 = X509_NAME_ENTRY_get_data(common_name_entry);
        if(common_name_asn1 == NULL)
            return false;

        char* common_name_str = (char*) ASN1_STRING_data(common_name_asn1);
        if(ASN1_STRING_length(common_name_asn1) != strlen(common_name_str))
            return false;
        return (boss_stricmp(hostname, common_name_str) == 0);
    }

    static bool verify_certificate(const char* hostname, bool preverified, boost::asio::ssl::verify_context& ctx)
    {
        int depth = X509_STORE_CTX_get_error_depth(ctx.native_handle());
        if(depth == 0 && preverified)
        {
            X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
            if(verify_subject_alternative_name(hostname, cert))
                return true;
            else if(verify_common_name(hostname, cert))
                return true;
            return false;
        }
        return preverified;
    }

    class HdlClass
    {
    public:
        connection_hdl mHdl;
    };

    static void OnMessage(WebSocketPrivate* c, connection_hdl hdl, ClientType::message_ptr msg)
    {
        const String* NewMessage = new String(msg->get_payload().c_str());
        Platform::BroadcastNotify("WebSocket:OnMessage", *NewMessage, NT_AddOn);

        ((HdlClass*) c->mHdlClass)->mHdl = hdl;
        c->mRecvStrings.Enqueue(NewMessage);
    }

    static void OnMessageTls(WebSocketPrivate* c, connection_hdl hdl, TlsClientType::message_ptr msg)
    {
        const String* NewMessage = new String(msg->get_payload().c_str());
        Platform::BroadcastNotify("WebSocket:OnMessage", *NewMessage, NT_AddOn);

        ((HdlClass*) c->mHdlClass)->mHdl = hdl;
        c->mRecvStrings.Enqueue(NewMessage);
    }

    static void OnOpen(WebSocketPrivate* c, connection_hdl hdl)
    {
        c->mState = WebSocketPrivate::State_Connected;
        ((HdlClass*) c->mHdlClass)->mHdl = hdl;
    }

    static void OnFail(WebSocketPrivate* c, connection_hdl hdl)
    {
        c->mState = WebSocketPrivate::State_Disconnected;
    }

    static void OnClose(WebSocketPrivate* c, connection_hdl hdl)
    {
        c->mState = WebSocketPrivate::State_Disconnected;
    }

    static ContextPtr OnInitTls(const char* hostname, connection_hdl hdl)
    {
        ContextPtr NewCtx = lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
        try
        {
            NewCtx->set_options(boost::asio::ssl::context::default_workarounds |
                boost::asio::ssl::context::no_sslv2 |
                boost::asio::ssl::context::no_sslv3 |
                boost::asio::ssl::context::single_dh_use);

            // 인증서 방식
            // NewCtx->set_verify_mode(boost::asio::ssl::verify_peer);
            // NewCtx->set_verify_callback(bind(&verify_certificate, hostname, lib::placeholders::_1, lib::placeholders::_2));
            // NewCtx->load_verify_file("ca-chain.cert.pem");

            // 비인증서 방식
            NewCtx->set_verify_mode(boost::asio::ssl::verify_none);
        }
        catch (std::exception& e)
        {
            BOSS_ASSERT(String::Format("OnInitTls가 실패하였습니다(%s)", e.what()), false);
        }
        return NewCtx;
    }

    WebSocketPrivate::WebSocketPrivate()
    {
        mState = State_Null;
        mIsTls = false;
        mClient = nullptr;
        mHdlClass = new HdlClass();
    }

    WebSocketPrivate::~WebSocketPrivate()
    {
        if(mIsTls) delete (TlsClientType*) mClient;
        else delete (ClientType*) mClient;
        delete (HdlClass*) mHdlClass;
        while(0 < mRecvStrings.Count())
            delete mRecvStrings.Dequeue();
    }

    bool WebSocketPrivate::Connect(chars url)
    {
        try
        {
            if(!String::CompareNoCase("wss://", url, 6))
            {
                auto CurClient = new TlsClientType();
                CurClient->set_access_channels(log::alevel::all);
                CurClient->clear_access_channels(log::alevel::frame_payload);
                CurClient->set_error_channels(log::elevel::all);
                CurClient->init_asio();

                CurClient->set_open_handler(lib::bind(&OnOpen, this, lib::placeholders::_1));
                CurClient->set_fail_handler(lib::bind(&OnFail, this, lib::placeholders::_1));
                CurClient->set_close_handler(lib::bind(&OnClose, this, lib::placeholders::_1));
                CurClient->set_message_handler(lib::bind(&OnMessageTls, this, lib::placeholders::_1, lib::placeholders::_2));

                String HostName(url + 6);
                const sint32 FindPos1 = HostName.Find(0, ":");
                if(0 <= FindPos1) HostName = HostName.Left(FindPos1);
                const sint32 FindPos2 = HostName.Find(0, "/");
                if(0 <= FindPos2) HostName = HostName.Left(FindPos2);
                CurClient->set_tls_init_handler(bind(&OnInitTls, (chars) HostName, lib::placeholders::_1));

                lib::error_code ErrorCode;
                TlsClientType::connection_ptr Connection = CurClient->get_connection(url, ErrorCode);
                if(ErrorCode)
                {
                    BOSS_ASSERT(String::Format("Connect이 실패하였습니다(%s)", ErrorCode.message().c_str()), false);
                    delete CurClient;
                    return false;
                }

                mState = State_Connection;
                mIsTls = true;
                mClient = CurClient;

                CurClient->connect(Connection);
                Platform::Utility::Threading(
                    [](void* data)->void
                    {
                        ((TlsClientType*) data)->run();
                    },
                    CurClient);
            }
            else
            {
                auto CurClient = new ClientType();
                CurClient->set_access_channels(log::alevel::all);
                CurClient->clear_access_channels(log::alevel::frame_payload);
                CurClient->init_asio();

                CurClient->set_open_handler(lib::bind(&OnOpen, this, lib::placeholders::_1));
                CurClient->set_fail_handler(lib::bind(&OnFail, this, lib::placeholders::_1));
                CurClient->set_close_handler(lib::bind(&OnClose, this, lib::placeholders::_1));
                CurClient->set_message_handler(lib::bind(&OnMessage, this, lib::placeholders::_1, lib::placeholders::_2));

                lib::error_code ErrorCode;
                ClientType::connection_ptr Connection = CurClient->get_connection(url, ErrorCode);
                if(ErrorCode)
                {
                    BOSS_ASSERT(String::Format("Connect이 실패하였습니다(%s)", ErrorCode.message().c_str()), false);
                    delete CurClient;
                    return false;
                }

                mState = State_Connection;
                mIsTls = true;
                mClient = CurClient;

                CurClient->connect(Connection);
                Platform::Utility::Threading(
                    [](void* data)->void
                    {
                        ((ClientType*) data)->run();
                    },
                    CurClient);
            }
        }
        catch (exception const& e)
        {
            BOSS_ASSERT(String::Format("Connect가 실패하였습니다(%s)", e.what()), false);
            if(mIsTls) delete (TlsClientType*) mClient;
            else delete (ClientType*) mClient;
            mClient = nullptr;
            return false;
        }
        return true;
    }

    bool WebSocketPrivate::IsConnected() const
    {
        return (mState == State_Connected);
    }

    void WebSocketPrivate::SendString(chars text)
    {
        if(mIsTls)
            ((TlsClientType*) mClient)->send(((HdlClass*) mHdlClass)->mHdl, text, frame::opcode::text);
        else ((ClientType*) mClient)->send(((HdlClass*) mHdlClass)->mHdl, text, frame::opcode::text);
    }

    void WebSocketPrivate::SendBinary(bytes data, sint32 len)
    {
        if(mIsTls)
            ((TlsClientType*) mClient)->send(((HdlClass*) mHdlClass)->mHdl, data, len, frame::opcode::binary);
        else ((ClientType*) mClient)->send(((HdlClass*) mHdlClass)->mHdl, data, len, frame::opcode::binary);
    }

    sint32 WebSocketPrivate::GetRecvCount() const
    {
        return mRecvStrings.Count();
    }

    const String* WebSocketPrivate::RecvStringOnce()
    {
        return mRecvStrings.Dequeue();
    }
}

namespace boost
{
    namespace random
    {
        random_device::random_device(void)
        {
        }

        random_device::~random_device(void)
        {
        }

        unsigned int random_device::operator()(void)
        {
            return Platform::Utility::Random();
        }
    }

    namespace system
    {
        class fake_error_category : public error_category
        {
        public:
            const char* name() const noexcept override {return nullptr;}
	        std::string message(int _Errval) const override {return "";}
        };

        error_category const& detail::system_category_ncx(void) noexcept
        {
            static fake_error_category _;
            return _;
        }

        error_category const& detail::generic_category_ncx(void) noexcept
        {
            static fake_error_category _;
            return _;
        }
    }
}

#endif
