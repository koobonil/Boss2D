/*
 *  Copyright (c) 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef LOGGING_RTC_EVENT_LOG_FAKE_RTC_EVENT_LOG_H_
#define LOGGING_RTC_EVENT_LOG_FAKE_RTC_EVENT_LOG_H_

#include <map>
#include <memory>

#include BOSS_WEBRTC_U_logging__rtc_event_log__events__rtc_event_h //original-code:"logging/rtc_event_log/events/rtc_event.h"
#include BOSS_WEBRTC_U_logging__rtc_event_log__rtc_event_log_h //original-code:"logging/rtc_event_log/rtc_event_log.h"
#include BOSS_WEBRTC_U_rtc_base__asyncinvoker_h //original-code:"rtc_base/asyncinvoker.h"
#include BOSS_WEBRTC_U_rtc_base__thread_h //original-code:"rtc_base/thread.h"

namespace webrtc {

class FakeRtcEventLog : public RtcEventLog {
 public:
  explicit FakeRtcEventLog(rtc::Thread* thread);
  ~FakeRtcEventLog() override;
  bool StartLogging(std::unique_ptr<RtcEventLogOutput> output,
                    int64_t output_period_ms) override;
  void StopLogging() override;
  void Log(std::unique_ptr<RtcEvent> event) override;
  int GetEventCount(RtcEvent::Type event_type) { return count_[event_type]; }

 private:
  void IncrementEventCount(RtcEvent::Type event_type) { ++count_[event_type]; }
  std::map<RtcEvent::Type, int> count_;
  rtc::Thread* thread_;
  rtc::AsyncInvoker invoker_;
};

}  // namespace webrtc

#endif  // LOGGING_RTC_EVENT_LOG_FAKE_RTC_EVENT_LOG_H_
