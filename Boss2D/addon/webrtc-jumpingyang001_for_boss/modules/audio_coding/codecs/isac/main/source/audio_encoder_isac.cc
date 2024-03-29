/*
 *  Copyright (c) 2014 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include BOSS_WEBRTC_U_modules__audio_coding__codecs__isac__main__include__audio_encoder_isac_h //original-code:"modules/audio_coding/codecs/isac/main/include/audio_encoder_isac.h"

#include BOSS_WEBRTC_U_modules__audio_coding__codecs__isac__audio_encoder_isac_t_impl_h //original-code:"modules/audio_coding/codecs/isac/audio_encoder_isac_t_impl.h"

namespace webrtc {

// Explicit instantiation:
template class AudioEncoderIsacT<IsacFloat>;

}  // namespace webrtc
