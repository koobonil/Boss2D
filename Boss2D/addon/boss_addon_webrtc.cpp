#include <boss.h>

#if !defined(BOSS_NEED_ADDON_WEBRTC) || (BOSS_NEED_ADDON_WEBRTC != 0 && BOSS_NEED_ADDON_WEBRTC != 1)
    #error BOSS_NEED_ADDON_WEBRTC macro is invalid use
#endif
bool __LINK_ADDON_WEBRTC__() {return true;} // 링크옵션 /OPT:NOREF가 안되서 임시코드

#if BOSS_NEED_ADDON_WEBRTC

#include "boss_addon_webrtc.hpp"

#include <boss.hpp>
#include <platform/boss_platform.hpp>

// 등록과정
namespace BOSS
{
    BOSS_DECLARE_ADDON_FUNCTION(WebRtc, Open, id_webrtc, sint32)
    BOSS_DECLARE_ADDON_FUNCTION(WebRtc, Close, void, id_webrtc)

    static autorun Bind_AddOn_WebRtc()
    {
        Core_AddOn_WebRtc_Open() = Customized_AddOn_WebRtc_Open;
        Core_AddOn_WebRtc_Close() = Customized_AddOn_WebRtc_Close;
        return true;
    }
    static autorun _ = Bind_AddOn_WebRtc();
}

// 구현부
namespace BOSS
{
    id_webrtc Customized_AddOn_WebRtc_Open(sint32 port)
    {
        auto NewWebRtc = new Conductor();
        return (id_webrtc) NewWebRtc;
    }

    void Customized_AddOn_WebRtc_Close(id_webrtc webrtc)
    {
        auto OldWebRtc = (Conductor*) webrtc;
        delete OldWebRtc;
    }
}

Conductor::Conductor()
{
    RTC_DCHECK(mPeerConnectionFactory.get() == nullptr);
    RTC_DCHECK(mPeerConnection.get() == nullptr);

    rtc::InitializeSSL();
    rtc::InitRandom(rtc::Time());
    rtc::ThreadManager::Instance()->WrapCurrentThread();

    mPeerConnectionFactory = webrtc::CreatePeerConnectionFactory(
        webrtc::CreateBuiltinAudioEncoderFactory(),
        webrtc::CreateBuiltinAudioDecoderFactory());
}

Conductor::~Conductor()
{
}

#endif
