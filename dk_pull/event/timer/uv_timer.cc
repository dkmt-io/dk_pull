/*******************************************************************************
 * Copyright (C) 2022 Huahang Liu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#include "dk_pull/event/timer/uv_timer.h"

#include <glog/logging.h>

#include <memory>

namespace dk_pull {
namespace event {
namespace timer {

UvTimer::UvTimer(const Options& options, uv_loop_t* uvLoop) : Timer(options) {
  CHECK_NOTNULL(callback);
  CHECK_NOTNULL(uvLoop);
  int rc = uv_timer_init(uvLoop, &uvHandle.timer);
  uvHandle.timer.data = this;
  CHECK_EQ(rc, 0) << "uv_timer_init() failed: " << uv_strerror(rc);
}

UvTimer::~UvTimer() = default;

std::shared_ptr<UvTimer> UvTimer::Create(const Options& options,
                                         uv_loop_t* uvLoop) {
  std::shared_ptr<UvTimer> timer(new UvTimer(options, uvLoop));
  timer->start();
  return timer;
}

bool UvTimer::start() {
  CHECK(!started) << "Timer already started";
  CHECK(!stopped) << "Timer already stopped";
  CHECK(!uv_is_closing(&uvHandle.handle)) << "Timer is closing";
  started = true;
  uint64_t repeat = type == Type::REPEATED ? 1 : 0;
  int rc = uv_timer_start(&uvHandle.timer, uvTimerCallback, delay, repeat);
  if (rc != 0) {
    LOG(ERROR) << "uv_timer_start() failed: " << uv_strerror(rc);
  }
  self = shared_from_this();
  return rc == 0;
}

bool UvTimer::Stop() {
  CHECK(started) << "Timer not started";
  CHECK(!stopped) << "Timer already stopped";
  stopped = true;
  int rc = uv_timer_stop(&uvHandle.timer);
  if (rc != 0) {
    LOG(ERROR) << "uv_timer_stop() failed: " << uv_strerror(rc);
  }
  close();
  return rc == 0;
}

void UvTimer::close() {
  if (closed || (uv_is_closing(&uvHandle.handle) != 0)) {
    return;
  }
  uv_close(&uvHandle.handle, started ? uvCloseCallback : nullptr);
}

void UvTimer::uvCloseCallback(uv_handle_t* handle) {
  auto* timer = static_cast<UvTimer*>(handle->data);
  timer->closed = true;
  timer->self = nullptr;
}

void UvTimer::uvTimerCallback(uv_timer_t* uvTimer) {
  CHECK_NOTNULL(uvTimer);
  auto* timer = static_cast<UvTimer*>(uvTimer->data);
  CHECK_NOTNULL(timer);
  timer->callback();
  if (timer->type == Timer::Type::ONE_TIME) {
    timer->close();
  }
}

}  // namespace timer
}  // namespace event
}  // namespace dk_pull
