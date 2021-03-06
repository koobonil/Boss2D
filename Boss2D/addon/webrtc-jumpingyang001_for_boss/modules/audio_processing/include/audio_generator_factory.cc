/*
 *  Copyright (c) 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include BOSS_WEBRTC_U_modules__audio_processing__include__audio_generator_factory_h //original-code:"modules/audio_processing/include/audio_generator_factory.h"

#include BOSS_ABSEILCPP_U_absl__memory__memory_h //original-code:"absl/memory/memory.h"
#include BOSS_WEBRTC_U_common_audio__wav_file_h //original-code:"common_audio/wav_file.h"
#include BOSS_WEBRTC_U_modules__audio_processing__audio_generator__file_audio_generator_h //original-code:"modules/audio_processing/audio_generator/file_audio_generator.h"

namespace webrtc {

std::unique_ptr<AudioGenerator> AudioGeneratorFactory::Create(
    const std::string& file_name) {
  std::unique_ptr<WavReader> input_audio_file(new WavReader(file_name));
  return absl::make_unique<FileAudioGenerator>(std::move(input_audio_file));
}

}  // namespace webrtc
