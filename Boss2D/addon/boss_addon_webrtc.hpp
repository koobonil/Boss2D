﻿#pragma once
#include <boss.h>

#include "boss_integration_webrtc-jumpingyang001.h"
#include <addon/webrtc-jumpingyang001_for_boss/api/audio_codecs/builtin_audio_decoder_factory.h>
#include <addon/webrtc-jumpingyang001_for_boss/api/audio_codecs/builtin_audio_encoder_factory.h>
#include <addon/webrtc-jumpingyang001_for_boss/api/mediastreaminterface.h>
#include <addon/webrtc-jumpingyang001_for_boss/api/peerconnectioninterface.h>
#include <addon/webrtc-jumpingyang001_for_boss/api/datachannelinterface.h>
#include <addon/webrtc-jumpingyang001_for_boss/examples/peerconnection/client/defaults.h>
#include <addon/webrtc-jumpingyang001_for_boss/examples/peerconnection/client/peer_connection_client.h>
#include <addon/webrtc-jumpingyang001_for_boss/media/engine/webrtcvideocapturerfactory.h>
#include <addon/webrtc-jumpingyang001_for_boss/modules/video_capture/video_capture_factory.h>
#include <addon/webrtc-jumpingyang001_for_boss/rtc_base/checks.h>
#include <addon/webrtc-jumpingyang001_for_boss/rtc_base/json.h>
#include <addon/webrtc-jumpingyang001_for_boss/rtc_base/logging.h>

#include <boss.hpp>

using namespace webrtc;

class WebRtcConnector
{
public:
    WebRtcConnector();
    ~WebRtcConnector();

public:
    void Release();

private:
    class PCO : public PeerConnectionObserver
    {
    public:
        PCO(WebRtcConnector& parent);
        ~PCO();
    private:
        WebRtcConnector& mParent;

    public:
        void OnSignalingChange(PeerConnectionInterface::SignalingState new_state) override;
        void OnAddTrack(rtc::scoped_refptr<RtpReceiverInterface> receiver, const std::vector<rtc::scoped_refptr<MediaStreamInterface>>& streams) override;
        void OnRemoveTrack(rtc::scoped_refptr<RtpReceiverInterface> receiver) override;
        void OnAddStream(rtc::scoped_refptr<MediaStreamInterface> stream) override;
        void OnRemoveStream(rtc::scoped_refptr<MediaStreamInterface> stream) override;
        void OnDataChannel(rtc::scoped_refptr<DataChannelInterface> data_channel) override;
        void OnRenegotiationNeeded() override;
        void OnIceConnectionChange(PeerConnectionInterface::IceConnectionState new_state) override;
        void OnIceGatheringChange(PeerConnectionInterface::IceGatheringState new_state) override;
        void OnIceCandidate(const IceCandidateInterface* candidate) override;
        void OnIceConnectionReceivingChange(bool receiving) override;
    };

private:
    class DCO : public DataChannelObserver
    {
    public:
        DCO(WebRtcConnector& parent);
        ~DCO();
    private:
        WebRtcConnector& mParent;

    public:
        void OnStateChange() override;
        void OnMessage(const DataBuffer& buffer) override;
    };

private:
    class CSDO : public CreateSessionDescriptionObserver
    {
    public:
        CSDO(WebRtcConnector& parent);
        ~CSDO();
    private:
        WebRtcConnector& mParent;

    public:
        void OnSuccess(SessionDescriptionInterface* desc) override;
        void OnFailure(const std::string& error) override;
    };

private:
    class SSDO : public SetSessionDescriptionObserver
    {
    public:
        SSDO(WebRtcConnector& parent);
        ~SSDO();
    private:
        WebRtcConnector& mParent;

    public:
        void OnSuccess() override;
        void OnFailure(const std::string& error) override;
    };

public:
    rtc::scoped_refptr<PeerConnectionInterface> mPeerConnection;
    rtc::scoped_refptr<DataChannelInterface> mDataChannel;
    Context mSessionInfo;

public:
    PCO mPCO;
    DCO mDCO;
    rtc::scoped_refptr<CSDO> mCSDO;
    rtc::scoped_refptr<SSDO> mSSDO;
};

class WebRtcManager
{
public:
    WebRtcManager();
    ~WebRtcManager();

public:
    bool CreateOffer();
    bool CreateAnswer(const Context& sdp);
    bool Bind(const Context& sdp);
    bool AddIce(const Context& sdp);
    void Send(bytes data, sint32 len);

private:
    bool CreateDataChannel(chars channel);
    bool SendToDataChannel(chars data);
    bool SetRemoteDescription(chars type, chars sdp);

private:
    class FactoryRunnable : public rtc::Runnable
    {
    public:
        FactoryRunnable(WebRtcManager& parent);
        ~FactoryRunnable();
    private:
        WebRtcManager& mParent;

    public:
        void Run(rtc::Thread* subthread) override;
    };

private:
    std::unique_ptr<rtc::Thread> mThread;
    rtc::PhysicalSocketServer mSocketServer;
    FactoryRunnable mFactory;

    rtc::scoped_refptr<PeerConnectionFactoryInterface> mConnectionFactory;
    PeerConnectionInterface::RTCConfiguration mConnectionConfig;
    WebRtcConnector mConnector;
};