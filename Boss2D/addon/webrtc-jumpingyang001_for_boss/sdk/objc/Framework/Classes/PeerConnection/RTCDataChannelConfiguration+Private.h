/*
 *  Copyright 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#import "WebRTC/RTCDataChannelConfiguration.h"

#include BOSS_WEBRTC_U_api__datachannelinterface_h //original-code:"api/datachannelinterface.h"

NS_ASSUME_NONNULL_BEGIN

@interface RTCDataChannelConfiguration ()

@property(nonatomic, readonly) webrtc::DataChannelInit nativeDataChannelInit;

@end

NS_ASSUME_NONNULL_END
