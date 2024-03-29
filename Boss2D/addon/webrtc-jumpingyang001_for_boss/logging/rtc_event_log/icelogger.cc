/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include BOSS_WEBRTC_U_logging__rtc_event_log__icelogger_h //original-code:"logging/rtc_event_log/icelogger.h"

#include BOSS_ABSEILCPP_U_absl__memory__memory_h //original-code:"absl/memory/memory.h"
#include BOSS_WEBRTC_U_logging__rtc_event_log__rtc_event_log_h //original-code:"logging/rtc_event_log/rtc_event_log.h"

namespace webrtc {

IceEventLog::IceEventLog() {}
IceEventLog::~IceEventLog() {}

void IceEventLog::LogCandidatePairConfig(
    IceCandidatePairConfigType type,
    uint32_t candidate_pair_id,
    const IceCandidatePairDescription& candidate_pair_desc) {
  if (event_log_ == nullptr) {
    return;
  }
  candidate_pair_desc_by_id_[candidate_pair_id] = candidate_pair_desc;
  event_log_->Log(absl::make_unique<RtcEventIceCandidatePairConfig>(
      type, candidate_pair_id, candidate_pair_desc));
}

void IceEventLog::LogCandidatePairEvent(IceCandidatePairEventType type,
                                        uint32_t candidate_pair_id) {
  if (event_log_ == nullptr) {
    return;
  }
  event_log_->Log(
      absl::make_unique<RtcEventIceCandidatePair>(type, candidate_pair_id));
}

void IceEventLog::DumpCandidatePairDescriptionToMemoryAsConfigEvents() const {
  for (const auto& desc_id_pair : candidate_pair_desc_by_id_) {
    event_log_->Log(absl::make_unique<RtcEventIceCandidatePairConfig>(
        IceCandidatePairConfigType::kUpdated, desc_id_pair.first,
        desc_id_pair.second));
  }
}

}  // namespace webrtc
