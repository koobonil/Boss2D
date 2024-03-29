/*
 *  Copyright (c) 2014 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef COMMON_AUDIO_WINDOW_GENERATOR_H_
#define COMMON_AUDIO_WINDOW_GENERATOR_H_

#include <stddef.h>

#include BOSS_WEBRTC_U_rtc_base__constructormagic_h //original-code:"rtc_base/constructormagic.h"

namespace webrtc {

// Helper class with generators for various signal transform windows.
class WindowGenerator {
 public:
  static void Hanning(int length, float* window);
  static void KaiserBesselDerived(float alpha, size_t length, float* window);

 private:
  RTC_DISALLOW_IMPLICIT_CONSTRUCTORS(WindowGenerator);
};

}  // namespace webrtc

#endif  // COMMON_AUDIO_WINDOW_GENERATOR_H_
