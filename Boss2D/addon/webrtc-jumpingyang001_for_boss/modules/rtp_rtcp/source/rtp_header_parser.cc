/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#include BOSS_WEBRTC_U_modules__rtp_rtcp__include__rtp_header_parser_h //original-code:"modules/rtp_rtcp/include/rtp_header_parser.h"

#include BOSS_WEBRTC_U_modules__rtp_rtcp__include__rtp_header_extension_map_h //original-code:"modules/rtp_rtcp/include/rtp_header_extension_map.h"
#include BOSS_WEBRTC_U_modules__rtp_rtcp__source__rtp_utility_h //original-code:"modules/rtp_rtcp/source/rtp_utility.h"
#include BOSS_WEBRTC_U_rtc_base__criticalsection_h //original-code:"rtc_base/criticalsection.h"

namespace webrtc {

class RtpHeaderParserImpl : public RtpHeaderParser {
 public:
  RtpHeaderParserImpl();
  ~RtpHeaderParserImpl() override = default;

  bool Parse(const uint8_t* packet,
             size_t length,
             RTPHeader* header) const override;

  bool RegisterRtpHeaderExtension(RTPExtensionType type, uint8_t id) override;

  bool DeregisterRtpHeaderExtension(RTPExtensionType type) override;

 private:
  rtc::CriticalSection critical_section_;
  RtpHeaderExtensionMap rtp_header_extension_map_
      RTC_GUARDED_BY(critical_section_);
};

RtpHeaderParser* RtpHeaderParser::Create() {
  return new RtpHeaderParserImpl;
}

RtpHeaderParserImpl::RtpHeaderParserImpl() {}

bool RtpHeaderParser::IsRtcp(const uint8_t* packet, size_t length) {
  RtpUtility::RtpHeaderParser rtp_parser(packet, length);
  return rtp_parser.RTCP();
}

bool RtpHeaderParserImpl::Parse(const uint8_t* packet,
                                size_t length,
                                RTPHeader* header) const {
  RtpUtility::RtpHeaderParser rtp_parser(packet, length);
  memset(header, 0, sizeof(*header));

  RtpHeaderExtensionMap map;
  {
    rtc::CritScope cs(&critical_section_);
    map = rtp_header_extension_map_;
  }

  const bool valid_rtpheader = rtp_parser.Parse(header, &map);
  if (!valid_rtpheader) {
    return false;
  }
  return true;
}

bool RtpHeaderParserImpl::RegisterRtpHeaderExtension(RTPExtensionType type,
                                                     uint8_t id) {
  rtc::CritScope cs(&critical_section_);
  return rtp_header_extension_map_.RegisterByType(id, type);
}

bool RtpHeaderParserImpl::DeregisterRtpHeaderExtension(RTPExtensionType type) {
  rtc::CritScope cs(&critical_section_);
  return rtp_header_extension_map_.Deregister(type) == 0;
}
}  // namespace webrtc
