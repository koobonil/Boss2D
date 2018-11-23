/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include BOSS_WEBRTC_U_modules__desktop_capture__window_finder_x11_h //original-code:"modules/desktop_capture/window_finder_x11.h"

#include "modules/desktop_capture/x11/window_list_utils.h"
#include BOSS_WEBRTC_U_rtc_base__checks_h //original-code:"rtc_base/checks.h"
#include BOSS_WEBRTC_U_rtc_base__ptr_util_h //original-code:"rtc_base/ptr_util.h"

namespace webrtc {

WindowFinderX11::WindowFinderX11(XAtomCache* cache)
    : cache_(cache) {
  RTC_DCHECK(cache_);
}

WindowFinderX11::~WindowFinderX11() = default;

WindowId WindowFinderX11::GetWindowUnderPoint(DesktopVector point) {
  WindowId id = kNullWindowId;
  GetWindowList(cache_,
                [&id, this, point](::Window window) {
                  DesktopRect rect;
                  if (GetWindowRect(this->cache_->display(), window, &rect) &&
                      rect.Contains(point)) {
                    id = window;
                    return false;
                  }
                  return true;
                });
  return id;
}

// static
std::unique_ptr<WindowFinder> WindowFinder::Create(
    const WindowFinder::Options& options) {
  if (options.cache == nullptr) {
    return nullptr;
  }

  return rtc::MakeUnique<WindowFinderX11>(options.cache);
}

}  // namespace webrtc
