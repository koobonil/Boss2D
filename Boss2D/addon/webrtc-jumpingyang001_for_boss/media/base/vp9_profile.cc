/*
 *  Copyright (c) 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include BOSS_WEBRTC_U_media__base__vp9_profile_h //original-code:"media/base/vp9_profile.h"

#include BOSS_WEBRTC_U_rtc_base__string_to_number_h //original-code:"rtc_base/string_to_number.h"

namespace webrtc {

// Profile information for VP9 video.
const char kVP9FmtpProfileId[] = "x-google-profile-id";

std::string VP9ProfileToString(VP9Profile profile) {
  switch (profile) {
    case VP9Profile::kProfile0:
      return "0";
    case VP9Profile::kProfile2:
      return "2";
  }
  return "0";
}

absl::optional<VP9Profile> StringToVP9Profile(const std::string& str) {
  const absl::optional<int> i = rtc::StringToNumber<int>(str);
  if (!i.has_value())
    return absl::nullopt;

  switch (i.value()) {
    case 0:
      return VP9Profile::kProfile0;
    case 2:
      return VP9Profile::kProfile2;
    default:
      return absl::nullopt;
  }
  return absl::nullopt;
}

absl::optional<VP9Profile> ParseSdpForVP9Profile(
    const SdpVideoFormat::Parameters& params) {
  const auto profile_it = params.find(kVP9FmtpProfileId);
  if (profile_it == params.end())
    return VP9Profile::kProfile0;
  const std::string& profile_str = profile_it->second;
  return StringToVP9Profile(profile_str);
}

bool IsSameVP9Profile(const SdpVideoFormat::Parameters& params1,
                      const SdpVideoFormat::Parameters& params2) {
  const absl::optional<VP9Profile> profile = ParseSdpForVP9Profile(params1);
  const absl::optional<VP9Profile> other_profile =
      ParseSdpForVP9Profile(params2);
  return profile && other_profile && profile == other_profile;
}

}  // namespace webrtc
