/* Copyright (c) 2016 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include BOSS_WEBRTC_U_modules__desktop_capture__mock_desktop_capturer_callback_h //original-code:"modules/desktop_capture/mock_desktop_capturer_callback.h"

namespace webrtc {

MockDesktopCapturerCallback::MockDesktopCapturerCallback() = default;
MockDesktopCapturerCallback::~MockDesktopCapturerCallback() = default;

void MockDesktopCapturerCallback::OnCaptureResult(
    DesktopCapturer::Result result,
    std::unique_ptr<DesktopFrame> frame) {
  OnCaptureResultPtr(result, &frame);
}

}  // namespace webrtc