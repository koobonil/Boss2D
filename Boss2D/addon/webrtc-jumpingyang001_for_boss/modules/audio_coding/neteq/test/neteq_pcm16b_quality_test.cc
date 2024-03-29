/*
 *  Copyright (c) 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <memory>

#include BOSS_WEBRTC_U_modules__audio_coding__codecs__pcm16b__audio_encoder_pcm16b_h //original-code:"modules/audio_coding/codecs/pcm16b/audio_encoder_pcm16b.h"

#include "modules/audio_coding/neteq/tools/neteq_quality_test.h"
#include BOSS_WEBRTC_U_rtc_base__checks_h //original-code:"rtc_base/checks.h"
#include BOSS_WEBRTC_U_rtc_base__flags_h //original-code:"rtc_base/flags.h"
#include BOSS_WEBRTC_U_rtc_base__numerics__safe_conversions_h //original-code:"rtc_base/numerics/safe_conversions.h"
#include "test/testsupport/fileutils.h"

using testing::InitGoogleTest;

namespace webrtc {
namespace test {
namespace {
static const int kInputSampleRateKhz = 48;
static const int kOutputSampleRateKhz = 48;

DEFINE_int(frame_size_ms, 20, "Codec frame size (milliseconds).");

}  // namespace

class NetEqPcm16bQualityTest : public NetEqQualityTest {
 protected:
  NetEqPcm16bQualityTest()
      : NetEqQualityTest(FLAG_frame_size_ms,
                         kInputSampleRateKhz,
                         kOutputSampleRateKhz,
                         NetEqDecoder::kDecoderPCM16Bswb48kHz) {
    // Flag validation
    RTC_CHECK(FLAG_frame_size_ms >= 10 && FLAG_frame_size_ms <= 60 &&
              (FLAG_frame_size_ms % 10) == 0)
        << "Invalid frame size, should be 10, 20, ..., 60 ms.";
  }

  void SetUp() override {
    AudioEncoderPcm16B::Config config;
    config.frame_size_ms = FLAG_frame_size_ms;
    config.sample_rate_hz = 48000;
    config.num_channels = channels_;
    encoder_.reset(new AudioEncoderPcm16B(config));
    NetEqQualityTest::SetUp();
  }

  int EncodeBlock(int16_t* in_data,
                  size_t block_size_samples,
                  rtc::Buffer* payload,
                  size_t max_bytes) override {
    const size_t kFrameSizeSamples = 480;  // Samples per 10 ms.
    size_t encoded_samples = 0;
    uint32_t dummy_timestamp = 0;
    AudioEncoder::EncodedInfo info;
    do {
      info = encoder_->Encode(dummy_timestamp,
                              rtc::ArrayView<const int16_t>(
                                  in_data + encoded_samples, kFrameSizeSamples),
                              payload);
      encoded_samples += kFrameSizeSamples;
    } while (info.encoded_bytes == 0);
    return rtc::checked_cast<int>(info.encoded_bytes);
  }

 private:
  std::unique_ptr<AudioEncoderPcm16B> encoder_;
};

TEST_F(NetEqPcm16bQualityTest, Test) {
  Simulate();
}

}  // namespace test
}  // namespace webrtc
