/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_AUDIO_PROCESSING_AEC3_MOCK_MOCK_ECHO_REMOVER_H_
#define MODULES_AUDIO_PROCESSING_AEC3_MOCK_MOCK_ECHO_REMOVER_H_

#include <vector>

#include BOSS_WEBRTC_U_api__optional_h //original-code:"api/optional.h"
#include "modules/audio_processing/aec3/echo_path_variability.h"
#include "modules/audio_processing/aec3/echo_remover.h"
#include "modules/audio_processing/aec3/render_buffer.h"
#include "test/gmock.h"

namespace webrtc {
namespace test {

class MockEchoRemover : public EchoRemover {
 public:
  virtual ~MockEchoRemover() = default;

  MOCK_METHOD4(ProcessCapture,
               void(const EchoPathVariability& echo_path_variability,
                    bool capture_signal_saturation,
                    RenderBuffer* render_buffer,
                    std::vector<std::vector<float>>* capture));

  MOCK_METHOD1(UpdateEchoLeakageStatus, void(bool leakage_detected));
  MOCK_CONST_METHOD1(GetMetrics, void(EchoControl::Metrics* metrics));
};

}  // namespace test
}  // namespace webrtc

#endif  // MODULES_AUDIO_PROCESSING_AEC3_MOCK_MOCK_ECHO_REMOVER_H_
