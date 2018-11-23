#pragma once
#include <boss.h>

#include "boss_integration_webrtc-jumpingyang001.h"
#include <addon/webrtc-jumpingyang001_for_boss/api/audio_codecs/builtin_audio_decoder_factory.h>
#include <addon/webrtc-jumpingyang001_for_boss/api/audio_codecs/builtin_audio_encoder_factory.h>
#include <addon/webrtc-jumpingyang001_for_boss/api/test/fakeconstraints.h>
#include <addon/webrtc-jumpingyang001_for_boss/examples/peerconnection/client/defaults.h>
#include <addon/webrtc-jumpingyang001_for_boss/media/engine/webrtcvideocapturerfactory.h>
#include <addon/webrtc-jumpingyang001_for_boss/modules/video_capture/video_capture_factory.h>
#include <addon/webrtc-jumpingyang001_for_boss/rtc_base/checks.h>
#include <addon/webrtc-jumpingyang001_for_boss/rtc_base/json.h>
#include <addon/webrtc-jumpingyang001_for_boss/rtc_base/logging.h>
#include <addon/webrtc-jumpingyang001_for_boss/rtc_base/ssladapter.h>

#include <boss.hpp>

class Conductor
{
public:
    Conductor();
    ~Conductor();

private:
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> mPeerConnection;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> mPeerConnectionFactory;
};
