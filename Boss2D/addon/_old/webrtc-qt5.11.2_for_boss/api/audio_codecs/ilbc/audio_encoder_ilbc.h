/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef API_AUDIO_CODECS_ILBC_AUDIO_ENCODER_ILBC_H_
#define API_AUDIO_CODECS_ILBC_AUDIO_ENCODER_ILBC_H_

#include <memory>
#include <vector>

#include BOSS_WEBRTC_U_api__audio_codecs__audio_encoder_h //original-code:"api/audio_codecs/audio_encoder.h"
#include BOSS_WEBRTC_U_api__audio_codecs__audio_format_h //original-code:"api/audio_codecs/audio_format.h"
#include BOSS_WEBRTC_U_api__audio_codecs__ilbc__audio_encoder_ilbc_config_h //original-code:"api/audio_codecs/ilbc/audio_encoder_ilbc_config.h"
#include BOSS_WEBRTC_U_api__optional_h //original-code:"api/optional.h"

namespace webrtc {

// ILBC encoder API for use as a template parameter to
// CreateAudioEncoderFactory<...>().
//
// NOTE: This struct is still under development and may change without notice.
struct AudioEncoderIlbc {
  using Config = AudioEncoderIlbcConfig;
  static rtc::Optional<AudioEncoderIlbcConfig> SdpToConfig(
      const SdpAudioFormat& audio_format);
  static void AppendSupportedEncoders(std::vector<AudioCodecSpec>* specs);
  static AudioCodecInfo QueryAudioEncoder(const AudioEncoderIlbcConfig& config);
  static std::unique_ptr<AudioEncoder> MakeAudioEncoder(
      const AudioEncoderIlbcConfig& config,
      int payload_type);
};

}  // namespace webrtc

#endif  // API_AUDIO_CODECS_ILBC_AUDIO_ENCODER_ILBC_H_
