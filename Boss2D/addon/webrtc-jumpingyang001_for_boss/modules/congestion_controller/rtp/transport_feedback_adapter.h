/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_CONGESTION_CONTROLLER_RTP_TRANSPORT_FEEDBACK_ADAPTER_H_
#define MODULES_CONGESTION_CONTROLLER_RTP_TRANSPORT_FEEDBACK_ADAPTER_H_

#include <deque>
#include <vector>

#include BOSS_WEBRTC_U_modules__congestion_controller__rtp__send_time_history_h //original-code:"modules/congestion_controller/rtp/send_time_history.h"
#include BOSS_WEBRTC_U_rtc_base__criticalsection_h //original-code:"rtc_base/criticalsection.h"
#include BOSS_WEBRTC_U_rtc_base__thread_annotations_h //original-code:"rtc_base/thread_annotations.h"
#include BOSS_WEBRTC_U_rtc_base__thread_checker_h //original-code:"rtc_base/thread_checker.h"
#include BOSS_WEBRTC_U_system_wrappers__include__clock_h //original-code:"system_wrappers/include/clock.h"

namespace webrtc {

class PacketFeedbackObserver;

namespace rtcp {
class TransportFeedback;
}  // namespace rtcp

namespace webrtc_cc {
class TransportFeedbackAdapter {
 public:
  explicit TransportFeedbackAdapter(const Clock* clock);
  virtual ~TransportFeedbackAdapter();

  void RegisterPacketFeedbackObserver(PacketFeedbackObserver* observer);
  void DeRegisterPacketFeedbackObserver(PacketFeedbackObserver* observer);

  void AddPacket(uint32_t ssrc,
                 uint16_t sequence_number,
                 size_t length,
                 const PacedPacketInfo& pacing_info);
  void OnSentPacket(uint16_t sequence_number, int64_t send_time_ms);

  // TODO(holmer): This method should return DelayBasedBwe::Result so that we
  // can get rid of the dependency on BitrateController. Requires changes
  // to the CongestionController interface.
  void OnTransportFeedback(const rtcp::TransportFeedback& feedback);
  std::vector<PacketFeedback> GetTransportFeedbackVector() const;
  absl::optional<PacketFeedback> GetPacket(uint16_t sequence_number) const;

  void SetTransportOverhead(int transport_overhead_bytes_per_packet);

  void SetNetworkIds(uint16_t local_id, uint16_t remote_id);

  size_t GetOutstandingBytes() const;

 private:
  std::vector<PacketFeedback> GetPacketFeedbackVector(
      const rtcp::TransportFeedback& feedback);

  rtc::CriticalSection lock_;
  SendTimeHistory send_time_history_ RTC_GUARDED_BY(&lock_);
  const Clock* const clock_;
  int64_t current_offset_ms_;
  int64_t last_timestamp_us_;
  std::vector<PacketFeedback> last_packet_feedback_vector_;
  uint16_t local_net_id_ RTC_GUARDED_BY(&lock_);
  uint16_t remote_net_id_ RTC_GUARDED_BY(&lock_);

  rtc::CriticalSection observers_lock_;
  std::vector<PacketFeedbackObserver*> observers_
      RTC_GUARDED_BY(&observers_lock_);
};

}  // namespace webrtc_cc
}  // namespace webrtc

#endif  // MODULES_CONGESTION_CONTROLLER_RTP_TRANSPORT_FEEDBACK_ADAPTER_H_
