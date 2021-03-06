/*
 *  Copyright 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include BOSS_WEBRTC_U_media__engine__internaldecoderfactory_h //original-code:"media/engine/internaldecoderfactory.h"
#include BOSS_WEBRTC_U_media__engine__internalencoderfactory_h //original-code:"media/engine/internalencoderfactory.h"
#include BOSS_WEBRTC_U_modules__video_coding__codecs__h264__include__h264_h //original-code:"modules/video_coding/codecs/h264/include/h264.h"
#include BOSS_WEBRTC_U_modules__video_coding__codecs__multiplex__include__multiplex_decoder_adapter_h //original-code:"modules/video_coding/codecs/multiplex/include/multiplex_decoder_adapter.h"
#include BOSS_WEBRTC_U_modules__video_coding__codecs__multiplex__include__multiplex_encoder_adapter_h //original-code:"modules/video_coding/codecs/multiplex/include/multiplex_encoder_adapter.h"
#include BOSS_WEBRTC_U_modules__video_coding__codecs__vp8__include__vp8_h //original-code:"modules/video_coding/codecs/vp8/include/vp8.h"
#include BOSS_WEBRTC_U_modules__video_coding__codecs__vp9__include__vp9_h //original-code:"modules/video_coding/codecs/vp9/include/vp9.h"
#include "test/call_test.h"
#include "test/encoder_settings.h"
#include "test/field_trial.h"
#include "test/function_video_encoder_factory.h"
#include "test/gtest.h"

namespace webrtc {

class CodecEndToEndTest : public test::CallTest,
                          public testing::WithParamInterface<std::string> {
 public:
  CodecEndToEndTest() : field_trial_(GetParam()) {}

 private:
  test::ScopedFieldTrials field_trial_;
};

INSTANTIATE_TEST_CASE_P(RoundRobin,
                        CodecEndToEndTest,
                        ::testing::Values("WebRTC-RoundRobinPacing/Disabled/",
                                          "WebRTC-RoundRobinPacing/Enabled/"));

class CodecObserver : public test::EndToEndTest,
                      public rtc::VideoSinkInterface<VideoFrame> {
 public:
  CodecObserver(int no_frames_to_wait_for,
                VideoRotation rotation_to_test,
                const std::string& payload_name,
                VideoEncoderFactory* encoder_factory,
                std::unique_ptr<webrtc::VideoDecoder> decoder)
      : EndToEndTest(4 * CodecEndToEndTest::kDefaultTimeoutMs),
        // TODO(hta): This timeout (120 seconds) is excessive.
        // https://bugs.webrtc.org/6830
        no_frames_to_wait_for_(no_frames_to_wait_for),
        expected_rotation_(rotation_to_test),
        payload_name_(payload_name),
        encoder_factory_(encoder_factory),
        decoder_(std::move(decoder)),
        frame_counter_(0) {}

  void PerformTest() override {
    EXPECT_TRUE(Wait())
        << "Timed out while waiting for enough frames to be decoded.";
  }

  void ModifyVideoConfigs(
      VideoSendStream::Config* send_config,
      std::vector<VideoReceiveStream::Config>* receive_configs,
      VideoEncoderConfig* encoder_config) override {
    encoder_config->codec_type = PayloadStringToCodecType(payload_name_);
    send_config->encoder_settings.encoder_factory = encoder_factory_;
    send_config->rtp.payload_name = payload_name_;
    send_config->rtp.payload_type = test::CallTest::kVideoSendPayloadType;

    (*receive_configs)[0].renderer = this;
    (*receive_configs)[0].decoders.resize(1);
    (*receive_configs)[0].decoders[0].payload_type =
        send_config->rtp.payload_type;
    (*receive_configs)[0].decoders[0].payload_name =
        send_config->rtp.payload_name;
    (*receive_configs)[0].decoders[0].decoder = decoder_.get();
  }

  void OnFrame(const VideoFrame& video_frame) override {
    EXPECT_EQ(expected_rotation_, video_frame.rotation());
    if (++frame_counter_ == no_frames_to_wait_for_)
      observation_complete_.Set();
  }

  void OnFrameGeneratorCapturerCreated(
      test::FrameGeneratorCapturer* frame_generator_capturer) override {
    frame_generator_capturer->SetFakeRotation(expected_rotation_);
  }

 private:
  int no_frames_to_wait_for_;
  VideoRotation expected_rotation_;
  std::string payload_name_;
  VideoEncoderFactory* encoder_factory_;
  std::unique_ptr<webrtc::VideoDecoder> decoder_;
  int frame_counter_;
};

TEST_P(CodecEndToEndTest, SendsAndReceivesVP8) {
  test::FunctionVideoEncoderFactory encoder_factory(
      []() { return VP8Encoder::Create(); });
  CodecObserver test(5, kVideoRotation_0, "VP8", &encoder_factory,
                     VP8Decoder::Create());
  RunBaseTest(&test);
}

TEST_P(CodecEndToEndTest, SendsAndReceivesVP8Rotation90) {
  test::FunctionVideoEncoderFactory encoder_factory(
      []() { return VP8Encoder::Create(); });
  CodecObserver test(5, kVideoRotation_90, "VP8", &encoder_factory,
                     VP8Decoder::Create());
  RunBaseTest(&test);
}

#if !defined(RTC_DISABLE_VP9)
TEST_P(CodecEndToEndTest, SendsAndReceivesVP9) {
  test::FunctionVideoEncoderFactory encoder_factory(
      []() { return VP9Encoder::Create(); });
  CodecObserver test(500, kVideoRotation_0, "VP9", &encoder_factory,
                     VP9Decoder::Create());
  RunBaseTest(&test);
}

TEST_P(CodecEndToEndTest, SendsAndReceivesVP9VideoRotation90) {
  test::FunctionVideoEncoderFactory encoder_factory(
      []() { return VP9Encoder::Create(); });
  CodecObserver test(5, kVideoRotation_90, "VP9", &encoder_factory,
                     VP9Decoder::Create());
  RunBaseTest(&test);
}

// Mutiplex tests are using VP9 as the underlying implementation.
TEST_P(CodecEndToEndTest, SendsAndReceivesMultiplex) {
  InternalEncoderFactory internal_encoder_factory;
  InternalDecoderFactory decoder_factory;
  test::FunctionVideoEncoderFactory encoder_factory(
      [&internal_encoder_factory]() {
        return absl::make_unique<MultiplexEncoderAdapter>(
            &internal_encoder_factory, SdpVideoFormat(cricket::kVp9CodecName));
      });
  CodecObserver test(
      5, kVideoRotation_0, "multiplex", &encoder_factory,
      absl::make_unique<MultiplexDecoderAdapter>(
          &decoder_factory, SdpVideoFormat(cricket::kVp9CodecName)));
  RunBaseTest(&test);
}

TEST_P(CodecEndToEndTest, SendsAndReceivesMultiplexVideoRotation90) {
  InternalEncoderFactory internal_encoder_factory;
  InternalDecoderFactory decoder_factory;
  test::FunctionVideoEncoderFactory encoder_factory(
      [&internal_encoder_factory]() {
        return absl::make_unique<MultiplexEncoderAdapter>(
            &internal_encoder_factory, SdpVideoFormat(cricket::kVp9CodecName));
      });
  CodecObserver test(
      5, kVideoRotation_90, "multiplex", &encoder_factory,
      absl::make_unique<MultiplexDecoderAdapter>(
          &decoder_factory, SdpVideoFormat(cricket::kVp9CodecName)));
  RunBaseTest(&test);
}

#endif  // !defined(RTC_DISABLE_VP9)

#if defined(WEBRTC_USE_H264)
class EndToEndTestH264 : public CodecEndToEndTest {};

const auto h264_field_trial_combinations = ::testing::Values(
    "WebRTC-SpsPpsIdrIsH264Keyframe/Disabled/WebRTC-RoundRobinPacing/Disabled/",
    "WebRTC-SpsPpsIdrIsH264Keyframe/Enabled/WebRTC-RoundRobinPacing/Disabled/",
    "WebRTC-SpsPpsIdrIsH264Keyframe/Disabled/WebRTC-RoundRobinPacing/Enabled/",
    "WebRTC-SpsPpsIdrIsH264Keyframe/Enabled/WebRTC-RoundRobinPacing/Enabled/");
INSTANTIATE_TEST_CASE_P(SpsPpsIdrIsKeyframe,
                        EndToEndTestH264,
                        h264_field_trial_combinations);

TEST_P(EndToEndTestH264, SendsAndReceivesH264) {
  test::FunctionVideoEncoderFactory encoder_factory(
      []() { return H264Encoder::Create(cricket::VideoCodec("H264")); });
  CodecObserver test(500, kVideoRotation_0, "H264", &encoder_factory,
                     H264Decoder::Create());
  RunBaseTest(&test);
}

TEST_P(EndToEndTestH264, SendsAndReceivesH264VideoRotation90) {
  test::FunctionVideoEncoderFactory encoder_factory(
      []() { return H264Encoder::Create(cricket::VideoCodec("H264")); });
  CodecObserver test(5, kVideoRotation_90, "H264", &encoder_factory,
                     H264Decoder::Create());
  RunBaseTest(&test);
}

TEST_P(EndToEndTestH264, SendsAndReceivesH264PacketizationMode0) {
  cricket::VideoCodec codec = cricket::VideoCodec("H264");
  codec.SetParam(cricket::kH264FmtpPacketizationMode, "0");
  test::FunctionVideoEncoderFactory encoder_factory(
      [codec]() { return H264Encoder::Create(codec); });
  CodecObserver test(500, kVideoRotation_0, "H264", &encoder_factory,
                     H264Decoder::Create());
  RunBaseTest(&test);
}

TEST_P(EndToEndTestH264, SendsAndReceivesH264PacketizationMode1) {
  cricket::VideoCodec codec = cricket::VideoCodec("H264");
  codec.SetParam(cricket::kH264FmtpPacketizationMode, "1");
  test::FunctionVideoEncoderFactory encoder_factory(
      [codec]() { return H264Encoder::Create(codec); });
  CodecObserver test(500, kVideoRotation_0, "H264", &encoder_factory,
                     H264Decoder::Create());
  RunBaseTest(&test);
}
#endif  // defined(WEBRTC_USE_H264)

}  // namespace webrtc
