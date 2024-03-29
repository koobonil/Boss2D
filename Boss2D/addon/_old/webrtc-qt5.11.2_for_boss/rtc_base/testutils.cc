/*
 *  Copyright 2007 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include BOSS_WEBRTC_U_rtc_base__testutils_h //original-code:"rtc_base/testutils.h"

namespace webrtc {
namespace testing {

StreamSink::StreamSink() = default;

StreamSink::~StreamSink() = default;

StreamSource::StreamSource() {
  Clear();
}

StreamSource::~StreamSource() = default;

StreamState StreamSource::GetState() const {
  return state_;
}

StreamResult StreamSource::Read(void* buffer,
                                size_t buffer_len,
                                size_t* read,
                                int* error) {
  if (SS_CLOSED == state_) {
    if (error)
      *error = -1;
    return SR_ERROR;
  }
  if ((SS_OPENING == state_) || (readable_data_.size() <= read_block_)) {
    return SR_BLOCK;
  }
  size_t count = std::min(buffer_len, readable_data_.size() - read_block_);
  memcpy(buffer, &readable_data_[0], count);
  size_t new_size = readable_data_.size() - count;
  // Avoid undefined access beyond the last element of the vector.
  // This only happens when new_size is 0.
  if (count < readable_data_.size()) {
    memmove(&readable_data_[0], &readable_data_[count], new_size);
  }
  readable_data_.resize(new_size);
  if (read)
    *read = count;
  return SR_SUCCESS;
}

StreamResult StreamSource::Write(const void* data,
                                 size_t data_len,
                                 size_t* written,
                                 int* error) {
  if (SS_CLOSED == state_) {
    if (error)
      *error = -1;
    return SR_ERROR;
  }
  if (SS_OPENING == state_) {
    return SR_BLOCK;
  }
  if (SIZE_UNKNOWN != write_block_) {
    if (written_data_.size() >= write_block_) {
      return SR_BLOCK;
    }
    if (data_len > (write_block_ - written_data_.size())) {
      data_len = write_block_ - written_data_.size();
    }
  }
  if (written)
    *written = data_len;
  const char* cdata = static_cast<const char*>(data);
  written_data_.insert(written_data_.end(), cdata, cdata + data_len);
  return SR_SUCCESS;
}

void StreamSource::Close() {
  state_ = SS_CLOSED;
}

}  // namespace testing
}  // namespace webrtc
