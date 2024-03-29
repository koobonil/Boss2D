/*
 *  Copyright (c) 2016 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "sdk/android/src/jni/androidvideotracksource.h"

#include <utility>

#include BOSS_WEBRTC_U_api__videosourceproxy_h //original-code:"api/videosourceproxy.h"
#include BOSS_WEBRTC_U_rtc_base__logging_h //original-code:"rtc_base/logging.h"
#include "sdk/android/generated_video_jni/jni/AndroidVideoTrackSourceObserver_jni.h"
#include "sdk/android/generated_video_jni/jni/VideoSource_jni.h"

namespace webrtc {
namespace jni {

namespace {
// MediaCodec wants resolution to be divisible by 2.
const int kRequiredResolutionAlignment = 2;

VideoRotation jintToVideoRotation(jint rotation) {
  RTC_DCHECK(rotation == 0 || rotation == 90 || rotation == 180 ||
             rotation == 270);
  return static_cast<VideoRotation>(rotation);
}

AndroidVideoTrackSource* AndroidVideoTrackSourceFromJavaProxy(jlong j_proxy) {
  auto proxy_source = reinterpret_cast<VideoTrackSourceProxy*>(j_proxy);
  return reinterpret_cast<AndroidVideoTrackSource*>(proxy_source->internal());
}

}  // namespace

AndroidVideoTrackSource::AndroidVideoTrackSource(
    rtc::Thread* signaling_thread,
    JNIEnv* jni,
    const JavaRef<jobject>& j_surface_texture_helper,
    bool is_screencast)
    : AdaptedVideoTrackSource(kRequiredResolutionAlignment),
      signaling_thread_(signaling_thread),
      surface_texture_helper_(new rtc::RefCountedObject<SurfaceTextureHelper>(
          jni,
          j_surface_texture_helper)),
      is_screencast_(is_screencast) {
  RTC_LOG(LS_INFO) << "AndroidVideoTrackSource ctor";
  camera_thread_checker_.DetachFromThread();
}

void AndroidVideoTrackSource::SetState(SourceState state) {
  if (rtc::Thread::Current() != signaling_thread_) {
    invoker_.AsyncInvoke<void>(
        RTC_FROM_HERE, signaling_thread_,
        rtc::Bind(&AndroidVideoTrackSource::SetState, this, state));
    return;
  }

  if (state_ != state) {
    state_ = state;
    FireOnChanged();
  }
}

void AndroidVideoTrackSource::OnByteBufferFrameCaptured(const void* frame_data,
                                                        int length,
                                                        int width,
                                                        int height,
                                                        VideoRotation rotation,
                                                        int64_t timestamp_ns) {
  RTC_DCHECK(camera_thread_checker_.CalledOnValidThread());

  int64_t camera_time_us = timestamp_ns / rtc::kNumNanosecsPerMicrosec;
  int64_t translated_camera_time_us =
      timestamp_aligner_.TranslateTimestamp(camera_time_us, rtc::TimeMicros());

  int adapted_width;
  int adapted_height;
  int crop_width;
  int crop_height;
  int crop_x;
  int crop_y;

  if (!AdaptFrame(width, height, camera_time_us, &adapted_width,
                  &adapted_height, &crop_width, &crop_height, &crop_x,
                  &crop_y)) {
    return;
  }

  const uint8_t* y_plane = static_cast<const uint8_t*>(frame_data);
  const uint8_t* uv_plane = y_plane + width * height;
  const int uv_width = (width + 1) / 2;

  RTC_CHECK_GE(length, width * height + 2 * uv_width * ((height + 1) / 2));

  // Can only crop at even pixels.
  crop_x &= ~1;
  crop_y &= ~1;
  // Crop just by modifying pointers.
  y_plane += width * crop_y + crop_x;
  uv_plane += uv_width * crop_y + crop_x;

  rtc::scoped_refptr<I420Buffer> buffer =
      buffer_pool_.CreateBuffer(adapted_width, adapted_height);

  nv12toi420_scaler_.NV12ToI420Scale(
      y_plane, width, uv_plane, uv_width * 2, crop_width, crop_height,
      buffer->MutableDataY(), buffer->StrideY(),
      // Swap U and V, since we have NV21, not NV12.
      buffer->MutableDataV(), buffer->StrideV(), buffer->MutableDataU(),
      buffer->StrideU(), buffer->width(), buffer->height());

  OnFrame(VideoFrame(buffer, rotation, translated_camera_time_us));
}

void AndroidVideoTrackSource::OnTextureFrameCaptured(
    int width,
    int height,
    VideoRotation rotation,
    int64_t timestamp_ns,
    const NativeHandleImpl& handle) {
  RTC_DCHECK(camera_thread_checker_.CalledOnValidThread());

  int64_t camera_time_us = timestamp_ns / rtc::kNumNanosecsPerMicrosec;
  int64_t translated_camera_time_us =
      timestamp_aligner_.TranslateTimestamp(camera_time_us, rtc::TimeMicros());

  int adapted_width;
  int adapted_height;
  int crop_width;
  int crop_height;
  int crop_x;
  int crop_y;

  if (!AdaptFrame(width, height, camera_time_us, &adapted_width,
                  &adapted_height, &crop_width, &crop_height, &crop_x,
                  &crop_y)) {
    surface_texture_helper_->ReturnTextureFrame();
    return;
  }

  Matrix matrix = handle.sampling_matrix;

  matrix.Crop(crop_width / static_cast<float>(width),
              crop_height / static_cast<float>(height),
              crop_x / static_cast<float>(width),
              crop_y / static_cast<float>(height));

  // Note that apply_rotation() may change under our feet, so we should only
  // check once.
  if (apply_rotation()) {
    if (rotation == kVideoRotation_90 || rotation == kVideoRotation_270) {
      std::swap(adapted_width, adapted_height);
    }
    matrix.Rotate(rotation);
    rotation = kVideoRotation_0;
  }

  OnFrame(VideoFrame(surface_texture_helper_->CreateTextureFrame(
                         adapted_width, adapted_height,
                         NativeHandleImpl(handle.oes_texture_id, matrix)),
                     rotation, translated_camera_time_us));
}

void AndroidVideoTrackSource::OnFrameCaptured(
    JNIEnv* jni,
    int width,
    int height,
    int64_t timestamp_ns,
    VideoRotation rotation,
    const JavaRef<jobject>& j_video_frame_buffer) {
  RTC_DCHECK(camera_thread_checker_.CalledOnValidThread());

  int64_t camera_time_us = timestamp_ns / rtc::kNumNanosecsPerMicrosec;
  int64_t translated_camera_time_us =
      timestamp_aligner_.TranslateTimestamp(camera_time_us, rtc::TimeMicros());

  int adapted_width;
  int adapted_height;
  int crop_width;
  int crop_height;
  int crop_x;
  int crop_y;

  if (!AdaptFrame(width, height, camera_time_us, &adapted_width,
                  &adapted_height, &crop_width, &crop_height, &crop_x,
                  &crop_y)) {
    return;
  }

  rtc::scoped_refptr<VideoFrameBuffer> buffer =
      AndroidVideoBuffer::Create(jni, j_video_frame_buffer)
          ->CropAndScale(jni, crop_x, crop_y, crop_width, crop_height,
                         adapted_width, adapted_height);

  // AdaptedVideoTrackSource handles applying rotation for I420 frames.
  if (apply_rotation() && rotation != kVideoRotation_0) {
    buffer = buffer->ToI420();
  }

  OnFrame(VideoFrame(buffer, rotation, translated_camera_time_us));
}

void AndroidVideoTrackSource::OnOutputFormatRequest(int width,
                                                    int height,
                                                    int fps) {
  cricket::VideoFormat format(width, height,
                              cricket::VideoFormat::FpsToInterval(fps), 0);
  video_adapter()->OnOutputFormatRequest(format);
}

static void JNI_AndroidVideoTrackSourceObserver_OnByteBufferFrameCaptured(
    JNIEnv* jni,
    const JavaParamRef<jclass>&,
    jlong j_source,
    const JavaParamRef<jbyteArray>& j_frame,
    jint length,
    jint width,
    jint height,
    jint rotation,
    jlong timestamp) {
  AndroidVideoTrackSource* source =
      AndroidVideoTrackSourceFromJavaProxy(j_source);
  jbyte* bytes = jni->GetByteArrayElements(j_frame.obj(), nullptr);
  source->OnByteBufferFrameCaptured(bytes, length, width, height,
                                    jintToVideoRotation(rotation), timestamp);
  jni->ReleaseByteArrayElements(j_frame.obj(), bytes, JNI_ABORT);
}

static void JNI_AndroidVideoTrackSourceObserver_OnTextureFrameCaptured(
    JNIEnv* jni,
    const JavaParamRef<jclass>&,
    jlong j_source,
    jint j_width,
    jint j_height,
    jint j_oes_texture_id,
    const JavaParamRef<jfloatArray>& j_transform_matrix,
    jint j_rotation,
    jlong j_timestamp) {
  AndroidVideoTrackSource* source =
      AndroidVideoTrackSourceFromJavaProxy(j_source);
  source->OnTextureFrameCaptured(
      j_width, j_height, jintToVideoRotation(j_rotation), j_timestamp,
      NativeHandleImpl(jni, j_oes_texture_id, j_transform_matrix));
}

static void JNI_AndroidVideoTrackSourceObserver_OnFrameCaptured(
    JNIEnv* jni,
    const JavaParamRef<jclass>&,
    jlong j_source,
    jint j_width,
    jint j_height,
    jint j_rotation,
    jlong j_timestamp_ns,
    const JavaParamRef<jobject>& j_video_frame_buffer) {
  AndroidVideoTrackSource* source =
      AndroidVideoTrackSourceFromJavaProxy(j_source);
  source->OnFrameCaptured(jni, j_width, j_height, j_timestamp_ns,
                          jintToVideoRotation(j_rotation),
                          j_video_frame_buffer);
}

static void JNI_AndroidVideoTrackSourceObserver_CapturerStarted(
    JNIEnv* jni,
    const JavaParamRef<jclass>&,
    jlong j_source,
    jboolean j_success) {
  RTC_LOG(LS_INFO) << "AndroidVideoTrackSourceObserve_nativeCapturerStarted";
  AndroidVideoTrackSource* source =
      AndroidVideoTrackSourceFromJavaProxy(j_source);
  source->SetState(j_success ? AndroidVideoTrackSource::SourceState::kLive
                             : AndroidVideoTrackSource::SourceState::kEnded);
}

static void JNI_AndroidVideoTrackSourceObserver_CapturerStopped(
    JNIEnv* jni,
    const JavaParamRef<jclass>&,
    jlong j_source) {
  RTC_LOG(LS_INFO) << "AndroidVideoTrackSourceObserve_nativeCapturerStopped";
  AndroidVideoTrackSource* source =
      AndroidVideoTrackSourceFromJavaProxy(j_source);
  source->SetState(AndroidVideoTrackSource::SourceState::kEnded);
}

static void JNI_VideoSource_AdaptOutputFormat(JNIEnv* jni,
                                              const JavaParamRef<jclass>&,
                                              jlong j_source,
                                              jint j_width,
                                              jint j_height,
                                              jint j_fps) {
  RTC_LOG(LS_INFO) << "VideoSource_nativeAdaptOutputFormat";
  AndroidVideoTrackSource* source =
      AndroidVideoTrackSourceFromJavaProxy(j_source);
  source->OnOutputFormatRequest(j_width, j_height, j_fps);
}

}  // namespace jni
}  // namespace webrtc
