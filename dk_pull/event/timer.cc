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

#include "dk_pull/event/timer.h"

#include <glog/logging.h>

#include <memory>

#include "dk_pull/event/event_loop.h"

namespace dk_pull {
namespace event {

Timer::Timer(const Options& options, uv_loop_t* uvLoop)
    : callback(options.callback),
      delay(options.delay),
      type(options.type),
      uvHandle() {
  CHECK_NOTNULL(callback);
  CHECK_NOTNULL(uvLoop);
  int rc = uv_timer_init(uvLoop, &uvHandle.timer);
  uvHandle.timer.data = this;
  CHECK_EQ(rc, 0) << "uv_timer_init() failed: " << uv_strerror(rc);
}

Timer::~Timer() = default;

std::shared_ptr<Timer> Timer::Create(const Options& options,
                                     uv_loop_t* uvLoop) {
  std::shared_ptr<Timer> timer(new Timer(options, uvLoop));
  timer->start();
  return timer;
}

bool Timer::start() {
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

void Timer::SetCallback(const Callback& cb) { callback = cb; }

bool Timer::Stop() {
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

void Timer::close() {
  if (closed || (uv_is_closing(&uvHandle.handle) != 0)) {
    return;
  }
  uv_close(&uvHandle.handle, started ? uvCloseCallback : nullptr);
}

void Timer::uvCloseCallback(uv_handle_t* handle) {
  auto* timer = static_cast<Timer*>(handle->data);
  timer->closed = true;
  timer->self = nullptr;
}

void Timer::uvTimerCallback(uv_timer_t* uvTimer) {
  CHECK_NOTNULL(uvTimer);
  auto* timer = static_cast<Timer*>(uvTimer->data);
  CHECK_NOTNULL(timer);
  timer->callback();
  if (timer->type == Type::ONE_TIME) {
    timer->close();
  }
}

}  // namespace event
}  // namespace dk_pull
