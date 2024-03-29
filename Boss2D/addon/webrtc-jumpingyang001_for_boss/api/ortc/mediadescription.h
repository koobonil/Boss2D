/*
 *  Copyright 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef API_ORTC_MEDIADESCRIPTION_H_
#define API_ORTC_MEDIADESCRIPTION_H_

#include <string>
#include <utility>
#include <vector>

#include BOSS_ABSEILCPP_U_absl__types__optional_h //original-code:"absl/types/optional.h"
#include BOSS_WEBRTC_U_api__cryptoparams_h //original-code:"api/cryptoparams.h"

namespace webrtc {

// A structured representation of a media description within an SDP session
// description.
class MediaDescription {
 public:
  explicit MediaDescription(std::string mid) : mid_(std::move(mid)) {}

  ~MediaDescription() {}

  // The mid(media stream identification) is used for identifying media streams
  // within a session description.
  // https://tools.ietf.org/html/rfc5888#section-6
  absl::optional<std::string> mid() const { return mid_; }
  void set_mid(std::string mid) { mid_.emplace(std::move(mid)); }

  // Security keys and parameters for this media stream. Can be used to
  // negotiate parameters for SRTP.
  // https://tools.ietf.org/html/rfc4568#page-5
  std::vector<cricket::CryptoParams>& sdes_params() { return sdes_params_; }
  const std::vector<cricket::CryptoParams>& sdes_params() const {
    return sdes_params_;
  }

 private:
  absl::optional<std::string> mid_;

  std::vector<cricket::CryptoParams> sdes_params_;
};

}  // namespace webrtc

#endif  // API_ORTC_MEDIADESCRIPTION_H_
