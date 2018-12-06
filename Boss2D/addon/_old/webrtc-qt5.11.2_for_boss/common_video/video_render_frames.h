/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef COMMON_VIDEO_VIDEO_RENDER_FRAMES_H_
#define COMMON_VIDEO_VIDEO_RENDER_FRAMES_H_

#include BOSS_FAKEWIN_V_stdint_h //original-code:<stdint.h>

#include <list>

#include BOSS_WEBRTC_U_api__optional_h //original-code:"api/optional.h"
#include BOSS_WEBRTC_U_api__video__video_frame_h //original-code:"api/video/video_frame.h"

namespace webrtc {

// Class definitions
class VideoRenderFrames {
 public:
  explicit VideoRenderFrames(uint32_t render_delay_ms);
  VideoRenderFrames(const VideoRenderFrames&) = delete;

  // Add a frame to the render queue
  int32_t AddFrame(VideoFrame&& new_frame);

  // Get a frame for rendering, or false if it's not time to render.
  rtc::Optional<VideoFrame> FrameToRender();

  // Returns the number of ms to next frame to render
  uint32_t TimeToNextFrameRelease();

  bool HasPendingFrames() const;

 private:
  // Sorted list with framed to be rendered, oldest first.
  std::list<VideoFrame> incoming_frames_;

  // Estimated delay from a frame is released until it's rendered.
  const uint32_t render_delay_ms_;

  int64_t last_render_time_ms_ = 0;
};

}  // namespace webrtc

#endif  // COMMON_VIDEO_VIDEO_RENDER_FRAMES_H_