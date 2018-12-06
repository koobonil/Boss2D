/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 *
 */

#ifndef MODULES_RTP_RTCP_SOURCE_RTCP_PACKET_RRTR_H_
#define MODULES_RTP_RTCP_SOURCE_RTCP_PACKET_RRTR_H_

#include BOSS_WEBRTC_U_rtc_base__basictypes_h //original-code:"rtc_base/basictypes.h"
#include BOSS_WEBRTC_U_system_wrappers__include__ntp_time_h //original-code:"system_wrappers/include/ntp_time.h"

namespace webrtc {
namespace rtcp {

class Rrtr {
 public:
  static const uint8_t kBlockType = 4;
  static const uint16_t kBlockLength = 2;
  static const size_t kLength = 4 * (kBlockLength + 1);  // 12

  Rrtr() {}
  Rrtr(const Rrtr&) = default;
  ~Rrtr() {}

  Rrtr& operator=(const Rrtr&) = default;

  void Parse(const uint8_t* buffer);

  // Fills buffer with the Rrtr.
  // Consumes Rrtr::kLength bytes.
  void Create(uint8_t* buffer) const;

  void SetNtp(NtpTime ntp) { ntp_ = ntp; }

  NtpTime ntp() const { return ntp_; }

 private:
  NtpTime ntp_;
};

}  // namespace rtcp
}  // namespace webrtc
#endif  // MODULES_RTP_RTCP_SOURCE_RTCP_PACKET_RRTR_H_