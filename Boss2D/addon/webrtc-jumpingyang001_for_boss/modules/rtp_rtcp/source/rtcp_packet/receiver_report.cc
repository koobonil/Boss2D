// author BOSS

/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include BOSS_WEBRTC_U_modules__rtp_rtcp__source__rtcp_packet__receiver_report_h //original-code:"modules/rtp_rtcp/source/rtcp_packet/receiver_report.h"

#include <utility>

#include BOSS_WEBRTC_U_modules__rtp_rtcp__source__byte_io_h //original-code:"modules/rtp_rtcp/source/byte_io.h"
#include BOSS_WEBRTC_U_modules__rtp_rtcp__source__rtcp_packet__common_header_h //original-code:"modules/rtp_rtcp/source/rtcp_packet/common_header.h"
#include BOSS_WEBRTC_U_rtc_base__checks_h //original-code:"rtc_base/checks.h"
#include BOSS_WEBRTC_U_rtc_base__logging_h //original-code:"rtc_base/logging.h"

namespace webrtc {
namespace rtcp {
constexpr uint8_t ReceiverReport::kPacketType;
constexpr size_t ReceiverReport::kMaxNumberOfReportBlocks;
// RTCP receiver report (RFC 3550).
//
//   0                   1                   2                   3
//   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |V=2|P|    RC   |   PT=RR=201   |             length            |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                     SSRC of packet sender                     |
//  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//  |                         report block(s)                       |
//  |                            ....                               |

ReceiverReport::ReceiverReport() : sender_ssrc_(0) {}

ReceiverReport::~ReceiverReport() = default;

bool ReceiverReport::Parse(const CommonHeader& packet) {
  RTC_DCHECK_EQ(packet.type(), kPacketType);

  const uint8_t report_blocks_count = packet.count();

  if (packet.payload_size_bytes() <
      kRrBaseLength + report_blocks_count * ReportBlock_rtcp_BOSS::kLength) { // modified by BOSS, original-code: ReportBlock, replace-code: ReportBlock_rtcp_BOSS
    RTC_LOG(LS_WARNING) << "Packet is too small to contain all the data.";
    return false;
  }

  sender_ssrc_ = ByteReader<uint32_t>::ReadBigEndian(packet.payload());

  const uint8_t* next_report_block = packet.payload() + kRrBaseLength;

  report_blocks_.resize(report_blocks_count);
  for (ReportBlock_rtcp_BOSS& block : report_blocks_) { // modified by BOSS, original-code: ReportBlock, replace-code: ReportBlock_rtcp_BOSS
    block.Parse(next_report_block, ReportBlock_rtcp_BOSS::kLength); // modified by BOSS, original-code: ReportBlock, replace-code: ReportBlock_rtcp_BOSS
    next_report_block += ReportBlock_rtcp_BOSS::kLength; // modified by BOSS, original-code: ReportBlock, replace-code: ReportBlock_rtcp_BOSS
  }

  RTC_DCHECK_LE(next_report_block - packet.payload(),
                static_cast<ptrdiff_t>(packet.payload_size_bytes()));
  return true;
}

size_t ReceiverReport::BlockLength() const {
  return kHeaderLength + kRrBaseLength +
         report_blocks_.size() * ReportBlock_rtcp_BOSS::kLength; // modified by BOSS, original-code: ReportBlock, replace-code: ReportBlock_rtcp_BOSS
}

bool ReceiverReport::Create(uint8_t* packet,
                            size_t* index,
                            size_t max_length,
                            PacketReadyCallback callback) const {
  while (*index + BlockLength() > max_length) {
    if (!OnBufferFull(packet, index, callback))
      return false;
  }
  CreateHeader(report_blocks_.size(), kPacketType, HeaderLength(), packet,
               index);
  ByteWriter<uint32_t>::WriteBigEndian(packet + *index, sender_ssrc_);
  *index += kRrBaseLength;
  for (const ReportBlock_rtcp_BOSS& block : report_blocks_) { // modified by BOSS, original-code: ReportBlock, replace-code: ReportBlock_rtcp_BOSS
    block.Create(packet + *index);
    *index += ReportBlock_rtcp_BOSS::kLength; // modified by BOSS, original-code: ReportBlock, replace-code: ReportBlock_rtcp_BOSS
  }
  return true;
}

bool ReceiverReport::AddReportBlock(const ReportBlock_rtcp_BOSS& block) { // modified by BOSS, original-code: ReportBlock, replace-code: ReportBlock_rtcp_BOSS
  if (report_blocks_.size() >= kMaxNumberOfReportBlocks) {
    RTC_LOG(LS_WARNING) << "Max report blocks reached.";
    return false;
  }
  report_blocks_.push_back(block);
  return true;
}

bool ReceiverReport::SetReportBlocks(std::vector<ReportBlock_rtcp_BOSS> blocks) { // modified by BOSS, original-code: ReportBlock, replace-code: ReportBlock_rtcp_BOSS
  if (blocks.size() > kMaxNumberOfReportBlocks) {
    RTC_LOG(LS_WARNING) << "Too many report blocks (" << blocks.size()
                        << ") for receiver report.";
    return false;
  }
  report_blocks_ = std::move(blocks);
  return true;
}

}  // namespace rtcp
}  // namespace webrtc
