/*
 *  Copyright 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include BOSS_WEBRTC_U_api__ortc__sessiondescription_h //original-code:"api/ortc/sessiondescription.h"
#include "test/gtest.h"

namespace webrtc {

class SessionDescriptionTest : public testing::Test {};

TEST_F(SessionDescriptionTest, CreateSessionDescription) {
  SessionDescription s(-1, "0");
  EXPECT_EQ(-1, s.session_id());
  EXPECT_EQ("0", s.session_version());
}
}  // namespace webrtc
