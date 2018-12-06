#include <boss.h>

#if !defined(BOSS_NEED_ADDON_WEBRTC) || (BOSS_NEED_ADDON_WEBRTC != 0 && BOSS_NEED_ADDON_WEBRTC != 1)
    #error BOSS_NEED_ADDON_WEBRTC macro is invalid use
#endif
bool __LINK_ADDON_WEBRTC__() {return true;} // 링크옵션 /OPT:NOREF가 안되서 임시코드

#if BOSS_NEED_ADDON_WEBRTC

#include "boss_addon_webrtc.hpp"

#include <addon/webrtc-jumpingyang001_for_boss/rtc_base/ssladapter.h>
#include <addon/webrtc-jumpingyang001_for_boss/rtc_base/win32socketserver.h>
#include <addon/webrtc-jumpingyang001_for_boss/api/test/fakeconstraints.h>

#include <boss.hpp>
#include <platform/boss_platform.hpp>

// 등록과정
namespace BOSS
{
    BOSS_DECLARE_ADDON_FUNCTION(WebRtc, OpenForOffer, id_webrtc, void)
    BOSS_DECLARE_ADDON_FUNCTION(WebRtc, OpenForAnswer, id_webrtc_read, chars)
    BOSS_DECLARE_ADDON_FUNCTION(WebRtc, Close, void, id_webrtc_read)
    BOSS_DECLARE_ADDON_FUNCTION(WebRtc, Bind, bool, id_webrtc, chars)
    BOSS_DECLARE_ADDON_FUNCTION(WebRtc, AddIce, bool, id_webrtc_read, chars)
    BOSS_DECLARE_ADDON_FUNCTION(WebRtc, Send, void, id_webrtc_read, bytes, sint32)

    static autorun Bind_AddOn_WebRtc()
    {
        Core_AddOn_WebRtc_OpenForOffer() = Customized_AddOn_WebRtc_OpenForOffer;
        Core_AddOn_WebRtc_OpenForAnswer() = Customized_AddOn_WebRtc_OpenForAnswer;
        Core_AddOn_WebRtc_Close() = Customized_AddOn_WebRtc_Close;
        Core_AddOn_WebRtc_Bind() = Customized_AddOn_WebRtc_Bind;
        Core_AddOn_WebRtc_AddIce() = Customized_AddOn_WebRtc_AddIce;
        Core_AddOn_WebRtc_Send() = Customized_AddOn_WebRtc_Send;
        return true;
    }
    static autorun _ = Bind_AddOn_WebRtc();
}

// 구현부
namespace BOSS
{
    id_webrtc Customized_AddOn_WebRtc_OpenForOffer()
    {
        auto NewWebRtcManager = new WebRtcManager();
        if(NewWebRtcManager)
        {
            if(NewWebRtcManager->CreateOffer())
                return (id_webrtc) NewWebRtcManager;
            delete NewWebRtcManager;
        }
        return (id_webrtc) nullptr;
    }

    id_webrtc_read Customized_AddOn_WebRtc_OpenForAnswer(chars offer_sdp)
    {
        auto NewWebRtcManager = new WebRtcManager();
        if(NewWebRtcManager)
        {
            const Context Sdp(ST_Json, SO_OnlyReference, offer_sdp);
            if(NewWebRtcManager->CreateAnswer(Sdp))
                return (id_webrtc_read) NewWebRtcManager;
            delete NewWebRtcManager;
        }
        return (id_webrtc_read) nullptr;
    }

    void Customized_AddOn_WebRtc_Close(id_webrtc_read webrtc)
    {
        auto OldWebRtcManager = (WebRtcManager*) webrtc;
        delete OldWebRtcManager;
    }

    bool Customized_AddOn_WebRtc_Bind(id_webrtc webrtc, chars answer_sdp)
    {
        if(auto CurWebRtcManager = (WebRtcManager*) webrtc)
        {
            const Context Sdp(ST_Json, SO_OnlyReference, answer_sdp);
            return CurWebRtcManager->Bind(Sdp);
        }
        return false;
    }

    bool Customized_AddOn_WebRtc_AddIce(id_webrtc_read webrtc, chars your_sdp)
    {
        if(auto CurWebRtcManager = (WebRtcManager*) webrtc)
        {
            const Context Sdp(ST_Json, SO_OnlyReference, your_sdp);
            return CurWebRtcManager->AddIce(Sdp);
        }
        return false;
    }

    void Customized_AddOn_WebRtc_Send(id_webrtc_read webrtc, bytes data, sint32 len)
    {
        if(auto CurWebRtcManager = (WebRtcManager*) webrtc)
            CurWebRtcManager->Send(data, len);
    }
}

////////////////////////////////////////////////////////////////////////////////
// WebRtcConnector
////////////////////////////////////////////////////////////////////////////////

WebRtcConnector::WebRtcConnector() :
    mPCO(*this),
    mDCO(*this),
    mCSDO(new rtc::RefCountedObject<CSDO>(*this)),
    mSSDO(new rtc::RefCountedObject<SSDO>(*this))
{
}

WebRtcConnector::~WebRtcConnector()
{
}

void WebRtcConnector::Release()
{
    if(mDataChannel.get())
        mDataChannel->Close();
    if(mPeerConnection.get())
        mPeerConnection->Close();
}

////////////////////////////////////////////////////////////////////////////////
// PCO
////////////////////////////////////////////////////////////////////////////////

WebRtcConnector::PCO::PCO(WebRtcConnector& parent) : mParent(parent)
{
}

WebRtcConnector::PCO::~PCO()
{
}

void WebRtcConnector::PCO::OnSignalingChange(PeerConnectionInterface::SignalingState new_state)
{
    BOSS_TRACE("WebRtcConnector::PCO::OnSignalingChange(%d)", (sint32) new_state);
}

void WebRtcConnector::PCO::OnAddTrack(rtc::scoped_refptr<RtpReceiverInterface> receiver, const std::vector<rtc::scoped_refptr<MediaStreamInterface>>& streams)
{
    BOSS_TRACE("WebRtcConnector::PCO::OnAddTrack()");
}

void WebRtcConnector::PCO::OnRemoveTrack(rtc::scoped_refptr<RtpReceiverInterface> receiver)
{
    BOSS_TRACE("WebRtcConnector::PCO::OnRemoveTrack()");
}

void WebRtcConnector::PCO::OnAddStream(rtc::scoped_refptr<MediaStreamInterface> stream)
{
    BOSS_TRACE("WebRtcConnector::PCO::OnAddStream()");
}

void WebRtcConnector::PCO::OnRemoveStream(rtc::scoped_refptr<MediaStreamInterface> stream)
{
    BOSS_TRACE("WebRtcConnector::PCO::OnRemoveStream()");
}

void WebRtcConnector::PCO::OnDataChannel(rtc::scoped_refptr<DataChannelInterface> data_channel)
{
    BOSS_TRACE("WebRtcConnector::PCO::OnDataChannel()");
    mParent.mDataChannel = data_channel;
    mParent.mDataChannel->RegisterObserver(&mParent.mDCO);
}

void WebRtcConnector::PCO::OnRenegotiationNeeded()
{
    BOSS_TRACE("WebRtcConnector::PCO::OnRenegotiationNeeded()");
}

void WebRtcConnector::PCO::OnIceConnectionChange(PeerConnectionInterface::IceConnectionState new_state)
{
    BOSS_TRACE("WebRtcConnector::PCO::OnIceConnectionChange(%d)", (sint32) new_state);
}

void WebRtcConnector::PCO::OnIceGatheringChange(PeerConnectionInterface::IceGatheringState new_state)
{
    BOSS_TRACE("WebRtcConnector::PCO::OnIceGatheringChange(%d)", (sint32) new_state);
}

void WebRtcConnector::PCO::OnIceCandidate(const IceCandidateInterface* candidate)
{
    BOSS_TRACE("WebRtcConnector::PCO::OnIceCandidate()");
    std::string SdpName;
    candidate->ToString(&SdpName);

    auto& NewCandidate = mParent.mSessionInfo.At("Candidate").AtAdding();
    NewCandidate.At("Sdp").Set(SdpName.c_str());
    NewCandidate.At("SdpMid").Set(candidate->sdp_mid().c_str());
    NewCandidate.At("SdpMLineIndex").Set(String::FromInteger(candidate->sdp_mline_index()));

    const String NewSDP = mParent.mSessionInfo.SaveJson();
    Platform::BroadcastNotify("WebRtc:CreateSdp", NewSDP, NT_AddOn);
    BOSS_TRACE("========================================");
    BOSS_TRACE("%s", (chars) NewSDP);
    BOSS_TRACE("========================================");
}

void WebRtcConnector::PCO::OnIceConnectionReceivingChange(bool receiving)
{
    BOSS_TRACE("WebRtcConnector::PCO::OnIceConnectionReceivingChange(%d)", (sint32) receiving);
}

////////////////////////////////////////////////////////////////////////////////
// DCO
////////////////////////////////////////////////////////////////////////////////

WebRtcConnector::DCO::DCO(WebRtcConnector& parent) : mParent(parent)
{
}

WebRtcConnector::DCO::~DCO()
{
}

void WebRtcConnector::DCO::OnStateChange()
{
    if(mParent.mDataChannel.get())
    {
        auto CurState = mParent.mDataChannel->state();
        BOSS_TRACE("WebRtcConnector::DCO::OnStateChange(%d)", CurState);

        if(CurState == DataChannelInterface::kOpen)
            Platform::BroadcastNotify("WebRtc:DataChannelOpen", nullptr, NT_AddOn);
    }
    else BOSS_TRACE("WebRtcConnector::DCO::OnStateChange()");
}

void WebRtcConnector::DCO::OnMessage(const DataBuffer& buffer)
{
    BOSS_TRACE("WebRtcConnector::DCO::OnMessage()");

    sint32 MessageSize = (sint32) buffer.data.size();
    const String NewMessage((chars) buffer.data.data(), MessageSize);
    Platform::BroadcastNotify("WebRtc:OnMessage", NewMessage, NT_AddOn);
}

////////////////////////////////////////////////////////////////////////////////
// CSDO
////////////////////////////////////////////////////////////////////////////////

WebRtcConnector::CSDO::CSDO(WebRtcConnector& parent) : mParent(parent)
{
}

WebRtcConnector::CSDO::~CSDO()
{
}

void WebRtcConnector::CSDO::OnSuccess(SessionDescriptionInterface* desc)
{
    BOSS_TRACE("WebRtcConnector::CSDO::OnSuccess()");
    mParent.mPeerConnection->SetLocalDescription(mParent.mSSDO, desc);

    std::string SdpName;
    desc->ToString(&SdpName);
    mParent.mSessionInfo.At("Sdp").At("Contents").Set(String(SdpName.c_str()).ToUrlString());

    const String NewSDP = mParent.mSessionInfo.SaveJson();
    Platform::BroadcastNotify("WebRtc:CreateSdp", NewSDP, NT_AddOn);
    BOSS_TRACE("========================================");
    BOSS_TRACE("%s", (chars) NewSDP);
    BOSS_TRACE("========================================");
}

void WebRtcConnector::CSDO::OnFailure(const std::string& error)
{
    BOSS_TRACE("WebRtcConnector::CSDO::OnFailure(%s)", error.c_str());
}

////////////////////////////////////////////////////////////////////////////////
// SSDO
////////////////////////////////////////////////////////////////////////////////

WebRtcConnector::SSDO::SSDO(WebRtcConnector& parent) : mParent(parent)
{
}

WebRtcConnector::SSDO::~SSDO()
{
}

void WebRtcConnector::SSDO::OnSuccess()
{
    BOSS_TRACE("WebRtcConnector::SSDO::OnSuccess()");
}

void WebRtcConnector::SSDO::OnFailure(const std::string& error)
{
    BOSS_TRACE("WebRtcConnector::SSDO::OnFailure(%s)", error.c_str());
}

////////////////////////////////////////////////////////////////////////////////
// WebRtcManager
////////////////////////////////////////////////////////////////////////////////

WebRtcManager::WebRtcManager() : mFactory(*this)
{
    PeerConnectionInterface::IceServer StunServer1;
    StunServer1.uri = "stun:stun.l.google.com:19302";
    mConnectionConfig.servers.push_back(StunServer1);

    PeerConnectionInterface::IceServer StunServer2;//////////////////////
    StunServer2.uri = "stun:stun01.sipphone.com";
    mConnectionConfig.servers.push_back(StunServer2);

    PeerConnectionInterface::IceServer StunServer3;//////////////////////
    StunServer3.uri = "stun:stun.ekiga.net";
    mConnectionConfig.servers.push_back(StunServer3);

    PeerConnectionInterface::IceServer TurnServer1;//////////////////////
    TurnServer1.urls.push_back("turn:192.158.29.39:3478?transport=udp");
    TurnServer1.username = "28224511:1379330808";
    TurnServer1.password = "JZEOEt2V3Qb0y27GRntt2u2PAYA=";
    mConnectionConfig.servers.push_back(TurnServer1);

    PeerConnectionInterface::IceServer TurnServer2;//////////////////////
    TurnServer2.urls.push_back("turn:192.158.29.39:3478?transport=tcp");
    TurnServer2.username = "28224511:1379330808";
    TurnServer2.password = "JZEOEt2V3Qb0y27GRntt2u2PAYA=";
    mConnectionConfig.servers.push_back(TurnServer2);

    rtc::InitializeSSL();
    mThread.reset(new rtc::Thread(&mSocketServer));
    mThread->Start(&mFactory);

    /*static std::unique_ptr<rtc::Thread> g_worker_thread;
    static std::unique_ptr<rtc::Thread> g_signaling_thread;
    g_worker_thread.reset(new rtc::Thread());
    g_worker_thread->Start();
    g_signaling_thread.reset(new rtc::Thread());
    g_signaling_thread->Start();
    mConnectionFactory = CreatePeerConnectionFactory(
        g_worker_thread.get(), g_worker_thread.get(), g_signaling_thread.get(),
        nullptr, CreateBuiltinAudioEncoderFactory(),
        CreateBuiltinAudioDecoderFactory(),
        nullptr, nullptr, nullptr, nullptr);*/
}

WebRtcManager::~WebRtcManager()
{
    mConnector.Release();
    //mThread.reset();
    rtc::CleanupSSL();
}

bool WebRtcManager::CreateOffer()
{
    while(!mConnectionFactory.get())
        Platform::Utility::Sleep(1, false, true);

    mConnector.mPeerConnection = mConnectionFactory->CreatePeerConnection(
        mConnectionConfig, nullptr, nullptr, &mConnector.mPCO);

    if(mConnector.mPeerConnection.get())
    {
        DataChannelInit Config;
        mConnector.mDataChannel = mConnector.mPeerConnection->CreateDataChannel("DataChannel", &Config);
        mConnector.mDataChannel->RegisterObserver(&mConnector.mDCO);

        /*auto NewAudioSource = mConnectionFactory->CreateAudioSource(cricket::AudioOptions());
        auto NewAudioTrack = mConnectionFactory->CreateAudioTrack("label", NewAudioSource);
        if(!mConnector.mPeerConnection->AddTrack(NewAudioTrack, {"streamid"}).ok())
            BOSS_TRACE("AddTrack이 실패하였습니다");*/

        mConnector.mSessionInfo.At("Sdp").At("Type").Set("offer");
        mConnector.mPeerConnection->CreateOffer(mConnector.mCSDO, nullptr);
        return true;
    }
    else BOSS_ASSERT("CreatePeerConnection이 실패하였습니다", false);
    return false;
}

bool WebRtcManager::CreateAnswer(const Context& sdp)
{
    while(!mConnectionFactory.get())
        Platform::Utility::Sleep(1, false, true);

    if(!String::Compare("offer", sdp("Sdp")("Type").GetString()))
    {
        mConnector.mPeerConnection = mConnectionFactory->CreatePeerConnection(
            mConnectionConfig, nullptr, nullptr, &mConnector.mPCO);

        if(mConnector.mPeerConnection.get())
        {
            const String SdpText = String::FromUrlString(sdp("Sdp")("Contents").GetString());
            SdpParseError SdpError;
            if(auto NewSessionDesc = CreateSessionDescription("offer", (chars) SdpText, &SdpError))
            {
                mConnector.mPeerConnection->SetRemoteDescription(mConnector.mSSDO, NewSessionDesc);

                mConnector.mSessionInfo.At("Sdp").At("Type").Set("answer");
                mConnector.mPeerConnection->CreateAnswer(mConnector.mCSDO, nullptr);
                return true;
            }
            else BOSS_ASSERT("CreateSessionDescription이 실패하였습니다", false);
        }
        else BOSS_ASSERT("CreatePeerConnection이 실패하였습니다", false);
    }
    else BOSS_ASSERT("상대방의 sdp가 offer가 아닙니다", false);
    return false;
}

bool WebRtcManager::Bind(const Context& sdp)
{
    if(!String::Compare("answer", sdp("Sdp")("Type").GetString()))
    {
        const String SdpText = String::FromUrlString(sdp("Sdp")("Contents").GetString());
        SdpParseError SdpError;
        if(auto NewSessionDesc = CreateSessionDescription("answer", (chars) SdpText, &SdpError))
        {
            mConnector.mPeerConnection->SetRemoteDescription(mConnector.mSSDO, NewSessionDesc);
            return true;
        }
        else BOSS_ASSERT("CreateSessionDescription이 실패하였습니다", false);
    }
    else BOSS_ASSERT("상대방의 sdp가 answer가 아닙니다", false);
    return false;
}

bool WebRtcManager::AddIce(const Context& sdp)
{
    hook(sdp("Candidate"))
    for(sint32 i = 0, iend = fish.LengthOfIndexable(); i < iend; ++i)
    {
        hook(fish[i])
        {
            SdpParseError SdpError;
            if(auto NewIceCandidate = CreateIceCandidate(fish("SdpMid").GetString(),
                fish("SdpMLineIndex").GetInt(), fish("Sdp").GetString(), &SdpError))
            {
                mConnector.mPeerConnection->AddIceCandidate(NewIceCandidate);
            }
            else
            {
                BOSS_ASSERT("CreateIceCandidate가 실패하였습니다", false);
                return false;
            }
        }
    }
    return true;
}

void WebRtcManager::Send(bytes data, sint32 len)
{
    if(mConnector.mDataChannel.get())
    {
        DataBuffer NewBuffer(rtc::CopyOnWriteBuffer(data, len), true);
        mConnector.mDataChannel->Send(NewBuffer);
    }
}

bool WebRtcManager::CreateDataChannel(chars channel)
{
    /*DataChannelInit NewInit;
    NewInit.ordered = true;
    NewInit.reliable = true;
    mDataChannel = mPeerConnection->CreateDataChannel(channel, &NewInit);
    if(mDataChannel.get())
    {
        mDataChannel->RegisterObserver(&mObserver);
        BOSS_TRACE("WebRtc:CreateDataChannel(%s) - true", channel);
        return true;
    }
    BOSS_TRACE("WebRtc:CreateDataChannel(%s) - false", channel);*/
    return false;
}

bool WebRtcManager::SendToDataChannel(chars data)
{
    /*if(!mDataChannel.get())
        CreateDataChannel("Channel");

    if(mDataChannel.get())
    if(mDataChannel->Send(DataBuffer(data)))
    {
        BOSS_TRACE("WebRtc:SendToDataChannel(%s) - true", data);
        return true;
    }

    BOSS_TRACE("WebRtc:SendToDataChannel(%s) - false", data);*/
    return false;
}

bool WebRtcManager::SetRemoteDescription(chars type, chars sdp)
{
    /*SdpParseError SdpError;
    if(auto NewSessionDesc = CreateSessionDescription(type, sdp, &SdpError))
    {
        mPeerConnection->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), NewSessionDesc);
        if(!String::Compare(type, "offer"))
            CreateAnswer();
        BOSS_TRACE("WebRtc:SetRemoteDescription(%s, sdp) - true", type);
        return true;
    }
    BOSS_TRACE("WebRtc:SetRemoteDescription(%s, sdp) - false", type);*/
    return false;
}

////////////////////////////////////////////////////////////////////////////////
// WebRtcManager
////////////////////////////////////////////////////////////////////////////////

WebRtcManager::FactoryRunnable::FactoryRunnable(WebRtcManager& parent) : mParent(parent)
{
}

WebRtcManager::FactoryRunnable::~FactoryRunnable()
{
}

void WebRtcManager::FactoryRunnable::Run(rtc::Thread* subthread)
{
    mParent.mConnectionFactory = CreatePeerConnectionFactory(
        CreateBuiltinAudioEncoderFactory(),
        CreateBuiltinAudioDecoderFactory());

    if(mParent.mConnectionFactory.get())
        subthread->Run();
    else BOSS_ASSERT("CreatePeerConnectionFactory가 실패하였습니다", false);
}

#endif
