/*
 *  Copyright 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef API_SETREMOTEDESCRIPTIONOBSERVERINTERFACE_H_
#define API_SETREMOTEDESCRIPTIONOBSERVERINTERFACE_H_

#include BOSS_WEBRTC_U_api__rtcerror_h //original-code:"api/rtcerror.h"
#include BOSS_WEBRTC_U_rtc_base__refcount_h //original-code:"rtc_base/refcount.h"

namespace webrtc {

// An observer for PeerConnectionInterface::SetRemoteDescription(). The
// callback is invoked such that the state of the peer connection can be
// examined to accurately reflect the effects of the SetRemoteDescription
// operation.
class SetRemoteDescriptionObserverInterface : public rtc::RefCountInterface {
 public:
  // On success, |error.ok()| is true.
  virtual void OnSetRemoteDescriptionComplete(RTCError error) = 0;
};

}  // namespace webrtc

#endif  // API_SETREMOTEDESCRIPTIONOBSERVERINTERFACE_H_
