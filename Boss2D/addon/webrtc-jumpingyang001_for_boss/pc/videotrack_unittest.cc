/*
 *  Copyright 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <memory>
#include <string>

#include BOSS_WEBRTC_U_media__base__fakemediaengine_h //original-code:"media/base/fakemediaengine.h"
#include BOSS_WEBRTC_U_media__base__fakevideocapturer_h //original-code:"media/base/fakevideocapturer.h"
#include BOSS_WEBRTC_U_pc__test__fakevideotrackrenderer_h //original-code:"pc/test/fakevideotrackrenderer.h"
#include BOSS_WEBRTC_U_pc__videocapturertracksource_h //original-code:"pc/videocapturertracksource.h"
#include BOSS_WEBRTC_U_pc__videotrack_h //original-code:"pc/videotrack.h"
#include BOSS_WEBRTC_U_rtc_base__gunit_h //original-code:"rtc_base/gunit.h"

using webrtc::FakeVideoTrackRenderer;
using webrtc::MediaSourceInterface;
using webrtc::MediaStreamTrackInterface;
using webrtc::VideoTrackSource;
using webrtc::VideoTrack;
using webrtc::VideoTrackInterface;

class TestVideoTrackSource : public VideoTrackSource {
 public:
  TestVideoTrackSource() : VideoTrackSource(true /* remote */) {}
  rtc::VideoSourceInterface<webrtc::VideoFrame>* source() override {
    return &capturer_;
  }
  cricket::FakeVideoCapturerWithTaskQueue* capturer() { return &capturer_; }

 private:
  cricket::FakeVideoCapturerWithTaskQueue capturer_;
};
class VideoTrackTest : public testing::Test {
 public:
  VideoTrackTest() {
    static const char kVideoTrackId[] = "track_id";
    video_track_source_ = new rtc::RefCountedObject<TestVideoTrackSource>();
    video_track_ = VideoTrack::Create(kVideoTrackId, video_track_source_,
                                      rtc::Thread::Current());
    video_track_source_->capturer()->Start(
        cricket::VideoFormat(640, 480, cricket::VideoFormat::FpsToInterval(30),
                             cricket::FOURCC_I420));
  }

 protected:
  rtc::scoped_refptr<TestVideoTrackSource> video_track_source_;
  rtc::scoped_refptr<VideoTrackInterface> video_track_;
};

// Test changing the source state also changes the track state.
TEST_F(VideoTrackTest, SourceStateChangeTrackState) {
  EXPECT_EQ(MediaStreamTrackInterface::kLive, video_track_->state());
  video_track_source_->SetState(MediaSourceInterface::kEnded);
  EXPECT_EQ(MediaStreamTrackInterface::kEnded, video_track_->state());
}

// Test adding renderers to a video track and render to them by providing
// frames to the source.
TEST_F(VideoTrackTest, RenderVideo) {
  // FakeVideoTrackRenderer register itself to |video_track_|
  std::unique_ptr<FakeVideoTrackRenderer> renderer_1(
      new FakeVideoTrackRenderer(video_track_.get()));

  video_track_source_->capturer()->CaptureFrame();
  EXPECT_EQ(1, renderer_1->num_rendered_frames());

  // FakeVideoTrackRenderer register itself to |video_track_|
  std::unique_ptr<FakeVideoTrackRenderer> renderer_2(
      new FakeVideoTrackRenderer(video_track_.get()));
  video_track_source_->capturer()->CaptureFrame();
  EXPECT_EQ(2, renderer_1->num_rendered_frames());
  EXPECT_EQ(1, renderer_2->num_rendered_frames());

  renderer_1.reset(nullptr);
  video_track_source_->capturer()->CaptureFrame();
  EXPECT_EQ(2, renderer_2->num_rendered_frames());
}

// Test that disabling the track results in blacked out frames.
TEST_F(VideoTrackTest, DisableTrackBlackout) {
  std::unique_ptr<FakeVideoTrackRenderer> renderer(
      new FakeVideoTrackRenderer(video_track_.get()));

  video_track_source_->capturer()->CaptureFrame();
  EXPECT_EQ(1, renderer->num_rendered_frames());
  EXPECT_FALSE(renderer->black_frame());

  video_track_->set_enabled(false);
  video_track_source_->capturer()->CaptureFrame();
  EXPECT_EQ(2, renderer->num_rendered_frames());
  EXPECT_TRUE(renderer->black_frame());

  video_track_->set_enabled(true);
  video_track_source_->capturer()->CaptureFrame();
  EXPECT_EQ(3, renderer->num_rendered_frames());
  EXPECT_FALSE(renderer->black_frame());
}
