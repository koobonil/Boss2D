/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include BOSS_WEBRTC_U_modules__video_coding__encoded_frame_h //original-code:"modules/video_coding/encoded_frame.h"

namespace webrtc {

VCMEncodedFrame::VCMEncodedFrame()
    : webrtc::EncodedImage(),
      _renderTimeMs(-1),
      _payloadType(0),
      _missingFrame(false),
      _codec(kVideoCodecUnknown),
      _rotation_set(false) {
  _codecSpecificInfo.codecType = kVideoCodecUnknown;
}

VCMEncodedFrame::~VCMEncodedFrame() {
  Free();
}

void VCMEncodedFrame::Free() {
  Reset();
  if (_buffer != NULL) {
    delete[] _buffer;
    _buffer = NULL;
  }
}

void VCMEncodedFrame::Reset() {
  _renderTimeMs = -1;
  _timeStamp = 0;
  _payloadType = 0;
  _frameType = kVideoFrameDelta;
  _encodedWidth = 0;
  _encodedHeight = 0;
  _completeFrame = false;
  _missingFrame = false;
  _length = 0;
  _codecSpecificInfo.codecType = kVideoCodecUnknown;
  _codec = kVideoCodecUnknown;
  rotation_ = kVideoRotation_0;
  content_type_ = VideoContentType::UNSPECIFIED;
  timing_.flags = VideoSendTiming::kInvalid;
  _rotation_set = false;
}

void VCMEncodedFrame::CopyCodecSpecific(const RTPVideoHeader* header) {
  if (header) {
    switch (header->codec) {
      case kVideoCodecVP8: {
        if (_codecSpecificInfo.codecType != kVideoCodecVP8) {
          // This is the first packet for this frame.
          _codecSpecificInfo.codecSpecific.VP8.temporalIdx = 0;
          _codecSpecificInfo.codecSpecific.VP8.layerSync = false;
          _codecSpecificInfo.codecSpecific.VP8.keyIdx = -1;
          _codecSpecificInfo.codecType = kVideoCodecVP8;
        }
        _codecSpecificInfo.codecSpecific.VP8.nonReference =
            header->vp8().nonReference;
        if (header->vp8().temporalIdx != kNoTemporalIdx) {
          _codecSpecificInfo.codecSpecific.VP8.temporalIdx =
              header->vp8().temporalIdx;
          _codecSpecificInfo.codecSpecific.VP8.layerSync =
              header->vp8().layerSync;
        }
        if (header->vp8().keyIdx != kNoKeyIdx) {
          _codecSpecificInfo.codecSpecific.VP8.keyIdx = header->vp8().keyIdx;
        }
        break;
      }
      case kVideoCodecVP9: {
        const auto& vp9_header =
            absl::get<RTPVideoHeaderVP9>(header->video_type_header);
        if (_codecSpecificInfo.codecType != kVideoCodecVP9) {
          // This is the first packet for this frame.
          _codecSpecificInfo.codecSpecific.VP9.temporal_idx = 0;
          _codecSpecificInfo.codecSpecific.VP9.spatial_idx = 0;
          _codecSpecificInfo.codecSpecific.VP9.gof_idx = 0;
          _codecSpecificInfo.codecSpecific.VP9.inter_layer_predicted = false;
          _codecSpecificInfo.codecType = kVideoCodecVP9;
        }
        _codecSpecificInfo.codecSpecific.VP9.inter_pic_predicted =
            vp9_header.inter_pic_predicted;
        _codecSpecificInfo.codecSpecific.VP9.flexible_mode =
            vp9_header.flexible_mode;
        _codecSpecificInfo.codecSpecific.VP9.num_ref_pics =
            vp9_header.num_ref_pics;
        for (uint8_t r = 0; r < vp9_header.num_ref_pics; ++r) {
          _codecSpecificInfo.codecSpecific.VP9.p_diff[r] =
              vp9_header.pid_diff[r];
        }
        _codecSpecificInfo.codecSpecific.VP9.ss_data_available =
            vp9_header.ss_data_available;
        if (vp9_header.temporal_idx != kNoTemporalIdx) {
          _codecSpecificInfo.codecSpecific.VP9.temporal_idx =
              vp9_header.temporal_idx;
          _codecSpecificInfo.codecSpecific.VP9.temporal_up_switch =
              vp9_header.temporal_up_switch;
        }
        if (vp9_header.spatial_idx != kNoSpatialIdx) {
          _codecSpecificInfo.codecSpecific.VP9.spatial_idx =
              vp9_header.spatial_idx;
          _codecSpecificInfo.codecSpecific.VP9.inter_layer_predicted =
              vp9_header.inter_layer_predicted;
        }
        if (vp9_header.gof_idx != kNoGofIdx) {
          _codecSpecificInfo.codecSpecific.VP9.gof_idx = vp9_header.gof_idx;
        }
        if (vp9_header.ss_data_available) {
          _codecSpecificInfo.codecSpecific.VP9.num_spatial_layers =
              vp9_header.num_spatial_layers;
          _codecSpecificInfo.codecSpecific.VP9
              .spatial_layer_resolution_present =
              vp9_header.spatial_layer_resolution_present;
          if (vp9_header.spatial_layer_resolution_present) {
            for (size_t i = 0; i < vp9_header.num_spatial_layers; ++i) {
              _codecSpecificInfo.codecSpecific.VP9.width[i] =
                  vp9_header.width[i];
              _codecSpecificInfo.codecSpecific.VP9.height[i] =
                  vp9_header.height[i];
            }
          }
          _codecSpecificInfo.codecSpecific.VP9.gof.CopyGofInfoVP9(
              vp9_header.gof);
        }
        break;
      }
      case kVideoCodecH264: {
        _codecSpecificInfo.codecType = kVideoCodecH264;
        break;
      }
      default: {
        _codecSpecificInfo.codecType = kVideoCodecUnknown;
        break;
      }
    }
  }
}

void VCMEncodedFrame::VerifyAndAllocate(size_t minimumSize) {
  if (minimumSize > _size) {
    // create buffer of sufficient size
    uint8_t* newBuffer = new uint8_t[minimumSize];
    if (_buffer) {
      // copy old data
      memcpy(newBuffer, _buffer, _size);
      delete[] _buffer;
    }
    _buffer = newBuffer;
    _size = minimumSize;
  }
}

}  // namespace webrtc
