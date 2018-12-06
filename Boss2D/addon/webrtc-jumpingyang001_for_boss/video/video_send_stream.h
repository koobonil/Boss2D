/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef VIDEO_VIDEO_SEND_STREAM_H_
#define VIDEO_VIDEO_SEND_STREAM_H_

#include <map>
#include <memory>
#include <vector>

#include BOSS_WEBRTC_U_api__fec_controller_h //original-code:"api/fec_controller.h"
#include BOSS_WEBRTC_U_api__video__video_stream_encoder_interface_h //original-code:"api/video/video_stream_encoder_interface.h"
#include BOSS_WEBRTC_U_call__bitrate_allocator_h //original-code:"call/bitrate_allocator.h"
#include BOSS_WEBRTC_U_call__video_receive_stream_h //original-code:"call/video_receive_stream.h"
#include BOSS_WEBRTC_U_call__video_send_stream_h //original-code:"call/video_send_stream.h"
#include BOSS_WEBRTC_U_common_video__libyuv__include__webrtc_libyuv_h //original-code:"common_video/libyuv/include/webrtc_libyuv.h"
#include BOSS_WEBRTC_U_rtc_base__criticalsection_h //original-code:"rtc_base/criticalsection.h"
#include BOSS_WEBRTC_U_rtc_base__event_h //original-code:"rtc_base/event.h"
#include BOSS_WEBRTC_U_rtc_base__task_queue_h //original-code:"rtc_base/task_queue.h"
#include BOSS_WEBRTC_U_video__send_delay_stats_h //original-code:"video/send_delay_stats.h"
#include BOSS_WEBRTC_U_video__send_statistics_proxy_h //original-code:"video/send_statistics_proxy.h"

namespace webrtc {
namespace test {
class VideoSendStreamPeer;
}  // namespace test

class CallStats;
class SendSideCongestionController;
class IvfFileWriter;
class ProcessThread;
class RateLimiter;
class RtpRtcp;
class RtpTransportControllerSendInterface;
class RtcEventLog;

namespace internal {

class VideoSendStreamImpl;

// VideoSendStream implements webrtc::VideoSendStream.
// Internally, it delegates all public methods to VideoSendStreamImpl and / or
// VideoStreamEncoder. VideoSendStreamInternal is created and deleted on
// |worker_queue|.
class VideoSendStream : public webrtc::VideoSendStream {
 public:
  using RtpStateMap = std::map<uint32_t, RtpState>;
  using RtpPayloadStateMap = std::map<uint32_t, RtpPayloadState>;

  VideoSendStream(
      int num_cpu_cores,
      ProcessThread* module_process_thread,
      rtc::TaskQueue* worker_queue,
      CallStats* call_stats,
      RtpTransportControllerSendInterface* transport,
      BitrateAllocator* bitrate_allocator,
      SendDelayStats* send_delay_stats,
      RtcEventLog* event_log,
      VideoSendStream::Config config,
      VideoEncoderConfig encoder_config,
      const std::map<uint32_t, RtpState>& suspended_ssrcs,
      const std::map<uint32_t, RtpPayloadState>& suspended_payload_states,
      std::unique_ptr<FecController> fec_controller);

  ~VideoSendStream() override;

  bool DeliverRtcp(const uint8_t* packet, size_t length);

  // webrtc::VideoSendStream implementation.
  void UpdateActiveSimulcastLayers(
      const std::vector<bool> active_layers) override;
  void Start() override;
  void Stop() override;

  void SetSource(rtc::VideoSourceInterface<webrtc::VideoFrame>* source,
                 const DegradationPreference& degradation_preference) override;

  void ReconfigureVideoEncoder(VideoEncoderConfig) override;
  Stats GetStats() override;

  // Takes ownership of each file, is responsible for closing them later.
  // Calling this method will close and finalize any current logs.
  // Giving rtc::kInvalidPlatformFileValue in any position disables logging
  // for the corresponding stream.
  // If a frame to be written would make the log too large the write fails and
  // the log is closed and finalized. A |byte_limit| of 0 means no limit.
  void EnableEncodedFrameRecording(const std::vector<rtc::PlatformFile>& files,
                                   size_t byte_limit) override;

  void StopPermanentlyAndGetRtpStates(RtpStateMap* rtp_state_map,
                                      RtpPayloadStateMap* payload_state_map);

  void SetTransportOverhead(size_t transport_overhead_per_packet);

 private:
  friend class test::VideoSendStreamPeer;

  class ConstructionTask;

  absl::optional<float> GetPacingFactorOverride() const;

  rtc::ThreadChecker thread_checker_;
  rtc::TaskQueue* const worker_queue_;
  rtc::Event thread_sync_event_;

  SendStatisticsProxy stats_proxy_;
  const VideoSendStream::Config config_;
  const VideoEncoderConfig::ContentType content_type_;
  std::unique_ptr<VideoSendStreamImpl> send_stream_;
  std::unique_ptr<VideoStreamEncoderInterface> video_stream_encoder_;
};

}  // namespace internal
}  // namespace webrtc

#endif  // VIDEO_VIDEO_SEND_STREAM_H_