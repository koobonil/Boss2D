/*
 *  Copyright (c) 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include BOSS_WEBRTC_U_media__base__mediachannel_h //original-code:"media/base/mediachannel.h"

namespace cricket {

VideoOptions::VideoOptions() = default;
VideoOptions::~VideoOptions() = default;

void MediaChannel::SetInterface(NetworkInterface* iface) {
  rtc::CritScope cs(&network_interface_crit_);
  network_interface_ = iface;
  SetDscp(enable_dscp_ ? PreferredDscp() : rtc::DSCP_DEFAULT);
}

rtc::DiffServCodePoint MediaChannel::PreferredDscp() const {
  return rtc::DSCP_DEFAULT;
}

int MediaChannel::GetRtpSendTimeExtnId() const {
  return -1;
}

MediaSenderInfo::MediaSenderInfo() = default;
MediaSenderInfo::~MediaSenderInfo() = default;

MediaReceiverInfo::MediaReceiverInfo() = default;
MediaReceiverInfo::~MediaReceiverInfo() = default;

VoiceSenderInfo::VoiceSenderInfo() = default;
VoiceSenderInfo::~VoiceSenderInfo() = default;

VoiceReceiverInfo::VoiceReceiverInfo() = default;
VoiceReceiverInfo::~VoiceReceiverInfo() = default;

VideoSenderInfo::VideoSenderInfo() = default;
VideoSenderInfo::~VideoSenderInfo() = default;

VideoReceiverInfo::VideoReceiverInfo() = default;
VideoReceiverInfo::~VideoReceiverInfo() = default;

VoiceMediaInfo::VoiceMediaInfo() = default;
VoiceMediaInfo::~VoiceMediaInfo() = default;

VideoMediaInfo::VideoMediaInfo() = default;
VideoMediaInfo::~VideoMediaInfo() = default;

DataMediaInfo::DataMediaInfo() = default;
DataMediaInfo::~DataMediaInfo() = default;

AudioSendParameters::AudioSendParameters() = default;
AudioSendParameters::~AudioSendParameters() = default;

std::map<std::string, std::string> AudioSendParameters::ToStringMap() const {
  auto params = RtpSendParameters<AudioCodec>::ToStringMap();
  params["options"] = options.ToString();
  return params;
}

VideoSendParameters::VideoSendParameters() = default;
VideoSendParameters::~VideoSendParameters() = default;

std::map<std::string, std::string> VideoSendParameters::ToStringMap() const {
  auto params = RtpSendParameters<VideoCodec>::ToStringMap();
  params["conference_mode"] = (conference_mode ? "yes" : "no");
  return params;
}

DataMediaChannel::DataMediaChannel() = default;
DataMediaChannel::DataMediaChannel(const MediaConfig& config)
    : MediaChannel(config) {}
DataMediaChannel::~DataMediaChannel() = default;

bool DataMediaChannel::GetStats(DataMediaInfo* info) {
  return true;
}

}  // namespace cricket
