/*******************************************************************************
 * Copyright (C) 2022 Hans Liu
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

#include "dk_pull/event/event_loop.h"

#include <glog/logging.h>

#include <mutex>
#include <thread>

namespace dk_pull {
namespace event {

EventLoop::EventLoop() : uvLoop() {
  int rc = uv_loop_init(&uvLoop);
  CHECK_EQ(rc, 0) << "uv_loop_init() failed: " << uv_strerror(rc);
}

EventLoop::~EventLoop() {
  int rc = uv_loop_close(&uvLoop);
  CHECK_EQ(rc, 0) << "uv_loop_close() failed: " << uv_strerror(rc);
}

void EventLoop::Run() {
  int rc = uv_run(&uvLoop, UV_RUN_DEFAULT);
  CHECK_EQ(rc, 0) << "uv_run() failed: " << uv_strerror(rc);
}

EventLoop& EventLoop::Default() {
  static std::atomic<bool> initialized(false);
  static std::mutex mutex;
  static std::shared_ptr<EventLoop> defaultLoop;
  static std::thread::id defaultThreadID;
  std::thread::id threadID = std::this_thread::get_id();
  if (initialized) {
    CHECK(defaultThreadID == threadID);
    CHECK(defaultLoop != nullptr);
    return *defaultLoop;
  }
  std::lock_guard<std::mutex> lock(mutex);
  if (initialized) {
    CHECK(defaultThreadID == threadID);
    CHECK(defaultLoop != nullptr);
    return *defaultLoop;
  }
  defaultThreadID = threadID;
  defaultLoop = std::make_shared<EventLoop>();
  initialized = true;
  return *defaultLoop;
}

std::shared_ptr<Timer> EventLoop::CreateTimer(const Timer::Options& options) {
  auto timer = Timer::Create(options, &uvLoop);
  return timer;
}

}  // namespace event
}  // namespace dk_pull
