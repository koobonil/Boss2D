/*
 *  Copyright (c) 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_AUDIO_PROCESSING_AGC2_ADAPTIVE_DIGITAL_GAIN_APPLIER_H_
#define MODULES_AUDIO_PROCESSING_AGC2_ADAPTIVE_DIGITAL_GAIN_APPLIER_H_

#include BOSS_WEBRTC_U_modules__audio_processing__agc2__agc2_common_h //original-code:"modules/audio_processing/agc2/agc2_common.h"
#include BOSS_WEBRTC_U_modules__audio_processing__agc2__gain_applier_h //original-code:"modules/audio_processing/agc2/gain_applier.h"
#include BOSS_WEBRTC_U_modules__audio_processing__agc2__vad_with_level_h //original-code:"modules/audio_processing/agc2/vad_with_level.h"
#include BOSS_WEBRTC_U_modules__audio_processing__include__audio_frame_view_h //original-code:"modules/audio_processing/include/audio_frame_view.h"

namespace webrtc {

class ApmDataDumper;

class AdaptiveDigitalGainApplier {
 public:
  explicit AdaptiveDigitalGainApplier(ApmDataDumper* apm_data_dumper);
  // Decide what gain to apply.
  void Process(float input_level_dbfs,
               float input_noise_level_dbfs,
               const VadWithLevel::LevelAndProbability vad_result,
               AudioFrameView<float> float_frame);

 private:
  float last_gain_db_ = kInitialAdaptiveDigitalGainDb;
  GainApplier gain_applier_;

  // For some combinations of noise and speech probability, increasing
  // the level is not allowed. Since we may get VAD results in bursts,
  // we keep track of this variable until the next VAD results come
  // in.
  bool gain_increase_allowed_ = true;
  ApmDataDumper* apm_data_dumper_ = nullptr;
};
}  // namespace webrtc

#endif  // MODULES_AUDIO_PROCESSING_AGC2_ADAPTIVE_DIGITAL_GAIN_APPLIER_H_
