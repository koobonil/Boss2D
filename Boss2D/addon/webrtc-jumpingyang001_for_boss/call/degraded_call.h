/*
 *  Copyright (c) 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef CALL_DEGRADED_CALL_H_
#define CALL_DEGRADED_CALL_H_

#include <memory>

#include BOSS_ABSEILCPP_U_absl__types__optional_h //original-code:"absl/types/optional.h"
#include BOSS_WEBRTC_U_api__call__transport_h //original-code:"api/call/transport.h"
#include BOSS_WEBRTC_U_call__call_h //original-code:"call/call.h"
#include BOSS_WEBRTC_U_call__fake_network_pipe_h //original-code:"call/fake_network_pipe.h"
#include BOSS_WEBRTC_U_modules__utility__include__process_thread_h //original-code:"modules/utility/include/process_thread.h"
#include BOSS_WEBRTC_U_system_wrappers__include__clock_h //original-code:"system_wrappers/include/clock.h"

namespace webrtc {

class DegradedCall : public Call, private Transport, private PacketReceiver {
 public:
  explicit DegradedCall(std::unique_ptr<Call> call,
                        absl::optional<FakeNetworkPipe::Config> send_config,
                        absl::optional<FakeNetworkPipe::Config> receive_config);
  ~DegradedCall() override;

  // Implements Call.
  AudioSendStream* CreateAudioSendStream(
      const AudioSendStream::Config& config) override;
  void DestroyAudioSendStream(AudioSendStream* send_stream) override;

  AudioReceiveStream* CreateAudioReceiveStream(
      const AudioReceiveStream::Config& config) override;
  void DestroyAudioReceiveStream(AudioReceiveStream* receive_stream) override;

  VideoSendStream* CreateVideoSendStream(
      VideoSendStream::Config config,
      VideoEncoderConfig encoder_config) override;
  VideoSendStream* CreateVideoSendStream(
      VideoSendStream::Config config,
      VideoEncoderConfig encoder_config,
      std::unique_ptr<FecController> fec_controller) override;
  void DestroyVideoSendStream(VideoSendStream* send_stream) override;

  VideoReceiveStream* CreateVideoReceiveStream(
      VideoReceiveStream::Config configuration) override;
  void DestroyVideoReceiveStream(VideoReceiveStream* receive_stream) override;

  FlexfecReceiveStream* CreateFlexfecReceiveStream(
      const FlexfecReceiveStream::Config& config) override;
  void DestroyFlexfecReceiveStream(
      FlexfecReceiveStream* receive_stream) override;

  PacketReceiver* Receiver() override;

  RtpTransportControllerSendInterface* GetTransportControllerSend() override;

  Stats GetStats() const override;

  void SetBitrateAllocationStrategy(
      std::unique_ptr<rtc::BitrateAllocationStrategy>
          bitrate_allocation_strategy) override;

  void SignalChannelNetworkState(MediaType media, NetworkState state) override;

  void OnTransportOverheadChanged(MediaType media,
                                  int transport_overhead_per_packet) override;

  void OnSentPacket(const rtc::SentPacket& sent_packet) override;

 protected:
  // Implements Transport.
  bool SendRtp(const uint8_t* packet,
               size_t length,
               const PacketOptions& options) override;

  bool SendRtcp(const uint8_t* packet, size_t length) override;

  // Implements PacketReceiver.
  DeliveryStatus DeliverPacket(MediaType media_type,
                               rtc::CopyOnWriteBuffer packet,
                               int64_t packet_time_us) override;

 private:
  Clock* const clock_;
  const std::unique_ptr<Call> call_;

  const absl::optional<FakeNetworkPipe::Config> send_config_;
  const std::unique_ptr<ProcessThread> send_process_thread_;
  std::unique_ptr<FakeNetworkPipe> send_pipe_;
  size_t num_send_streams_;

  const absl::optional<FakeNetworkPipe::Config> receive_config_;
  std::unique_ptr<FakeNetworkPipe> receive_pipe_;
};

}  // namespace webrtc

#endif  // CALL_DEGRADED_CALL_H_
