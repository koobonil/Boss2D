/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// This file contains the class RtpFormatVp8TestHelper. The class is
// responsible for setting up a fake VP8 bitstream according to the
// RTPVideoHeaderVP8 header, and partition information. After initialization,
// an RTPFragmentationHeader is provided so that the tester can create a
// packetizer. The packetizer can then be provided to this helper class, which
// will then extract all packets and compare to the expected outcome.

#ifndef MODULES_RTP_RTCP_SOURCE_RTP_FORMAT_VP8_TEST_HELPER_H_
#define MODULES_RTP_RTCP_SOURCE_RTP_FORMAT_VP8_TEST_HELPER_H_

#include BOSS_WEBRTC_U_modules__include__module_common_types_h //original-code:"modules/include/module_common_types.h"
#include BOSS_WEBRTC_U_modules__rtp_rtcp__source__rtp_format_vp8_h //original-code:"modules/rtp_rtcp/source/rtp_format_vp8.h"
#include BOSS_WEBRTC_U_modules__rtp_rtcp__source__rtp_packet_to_send_h //original-code:"modules/rtp_rtcp/source/rtp_packet_to_send.h"
#include BOSS_WEBRTC_U_rtc_base__constructormagic_h //original-code:"rtc_base/constructormagic.h"

namespace webrtc {

namespace test {

class RtpFormatVp8TestHelper {
 public:
  explicit RtpFormatVp8TestHelper(const RTPVideoHeaderVP8* hdr);
  ~RtpFormatVp8TestHelper();
  bool Init(const size_t* partition_sizes, size_t num_partitions);
  void GetAllPacketsAndCheck(RtpPacketizerVp8* packetizer,
                             const size_t* expected_sizes,
                             const int* expected_part,
                             const bool* expected_frag_start,
                             size_t expected_num_packets);

  uint8_t* payload_data() const { return payload_data_; }
  size_t payload_size() const { return payload_size_; }
  RTPFragmentationHeader* fragmentation() const { return fragmentation_; }
  size_t buffer_size() const {
    static constexpr size_t kVp8PayloadDescriptorMaxSize = 6;
    return payload_size_ + kVp8PayloadDescriptorMaxSize;
  }
  void set_sloppy_partitioning(bool value) { sloppy_partitioning_ = value; }

 private:
  void CheckHeader(bool frag_start);
  void CheckPictureID();
  void CheckTl0PicIdx();
  void CheckTIDAndKeyIdx();
  void CheckPayload();
  void CheckLast(bool last) const;
  void CheckPacket(size_t expect_bytes, bool last, bool frag_start);

  RtpPacketToSend packet_;
  uint8_t* payload_data_;
  uint8_t* data_ptr_;
  RTPFragmentationHeader* fragmentation_;
  const RTPVideoHeaderVP8* hdr_info_;
  int payload_start_;
  size_t payload_size_;
  bool sloppy_partitioning_;
  bool inited_;

  RTC_DISALLOW_COPY_AND_ASSIGN(RtpFormatVp8TestHelper);
};

}  // namespace test

}  // namespace webrtc

#endif  // MODULES_RTP_RTCP_SOURCE_RTP_FORMAT_VP8_TEST_HELPER_H_
