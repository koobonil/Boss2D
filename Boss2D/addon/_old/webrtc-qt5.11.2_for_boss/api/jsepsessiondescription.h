/*
 *  Copyright 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// TODO(deadbeef): Move this out of api/; it's an implementation detail and
// shouldn't be used externally.

#ifndef API_JSEPSESSIONDESCRIPTION_H_
#define API_JSEPSESSIONDESCRIPTION_H_

#include <memory>
#include <string>
#include <vector>

#include BOSS_WEBRTC_U_api__candidate_h //original-code:"api/candidate.h"
#include BOSS_WEBRTC_U_api__jsep_h //original-code:"api/jsep.h"
#include BOSS_WEBRTC_U_api__jsepicecandidate_h //original-code:"api/jsepicecandidate.h"
#include BOSS_WEBRTC_U_rtc_base__constructormagic_h //original-code:"rtc_base/constructormagic.h"

namespace cricket {
class SessionDescription;
}

namespace webrtc {

// Implementation of SessionDescriptionInterface.
class JsepSessionDescription : public SessionDescriptionInterface {
 public:
  explicit JsepSessionDescription(SdpType type);
  // TODO(steveanton): Remove this once callers have switched to SdpType.
  explicit JsepSessionDescription(const std::string& type);
  virtual ~JsepSessionDescription();

  // Takes ownership of |description|.
  // TODO(deadbeef): Make this use an std::unique_ptr<>, so ownership logic is
  // more clear.
  bool Initialize(cricket::SessionDescription* description,
      const std::string& session_id,
      const std::string& session_version);

  virtual cricket::SessionDescription* description() {
    return description_.get();
  }
  virtual const cricket::SessionDescription* description() const {
    return description_.get();
  }
  virtual std::string session_id() const {
    return session_id_;
  }
  virtual std::string session_version() const {
    return session_version_;
  }
  virtual SdpType GetType() const { return type_; }
  virtual std::string type() const { return SdpTypeToString(type_); }
  // Allows changing the type. Used for testing.
  virtual bool AddCandidate(const IceCandidateInterface* candidate);
  virtual size_t RemoveCandidates(
      const std::vector<cricket::Candidate>& candidates);
  virtual size_t number_of_mediasections() const;
  virtual const IceCandidateCollection* candidates(
      size_t mediasection_index) const;
  virtual bool ToString(std::string* out) const;

  static const int kDefaultVideoCodecId;
  static const char kDefaultVideoCodecName[];

 private:
  std::unique_ptr<cricket::SessionDescription> description_;
  std::string session_id_;
  std::string session_version_;
  SdpType type_;
  std::vector<JsepCandidateCollection> candidate_collection_;

  bool GetMediasectionIndex(const IceCandidateInterface* candidate,
                            size_t* index);
  int GetMediasectionIndex(const cricket::Candidate& candidate);

  RTC_DISALLOW_COPY_AND_ASSIGN(JsepSessionDescription);
};

}  // namespace webrtc

#endif  // API_JSEPSESSIONDESCRIPTION_H_
