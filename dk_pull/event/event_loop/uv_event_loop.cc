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

#include "dk_pull/event/event_loop/uv_event_loop.h"

#include <glog/logging.h>

#include <mutex>
#include <thread>

#include "dk_pull/event/timer/uv_timer.h"

namespace dk_pull {
namespace event {
namespace event_loop {

UvEventLoop::UvEventLoop() : EventLoop(){
  int rc = uv_loop_init(&uvLoop);
  CHECK_EQ(rc, 0) << "uv_loop_init() failed: " << uv_strerror(rc);
}

UvEventLoop::~UvEventLoop() {
  int rc = uv_loop_close(&uvLoop);
  CHECK_EQ(rc, 0) << "uv_loop_close() failed: " << uv_strerror(rc);
}

void UvEventLoop::Run() {
  while (uv_loop_alive(&uvLoop) != 0) {
    uv_run(&uvLoop, UV_RUN_ONCE);
  }
}

std::shared_ptr<::dk_pull::event::timer::Timer> UvEventLoop::CreateTimer(
    const Timer::Options& options) {
  using dk_pull::event::timer::UvTimer;
  auto timer = UvTimer::Create(options, &uvLoop);
  return std::dynamic_pointer_cast<Timer>(timer);
}

}  // namespace event_loop
}  // namespace event
}  // namespace dk_pull
