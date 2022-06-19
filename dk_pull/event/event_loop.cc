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

#include "dk_pull/event/event_loop.h"

#include <glog/logging.h>

#include <mutex>
#include <thread>

namespace dk_pull {
namespace event {

EventLoop::EventLoop() : uvLoop(), uvAsync(), uvAsyncClosed(false) {
  int rc = uv_loop_init(&uvLoop);
  CHECK_EQ(rc, 0) << "uv_loop_init() failed: " << uv_strerror(rc);
  rc = uv_async_init(&uvLoop, &uvAsync.async, nullptr);
  CHECK_EQ(rc, 0) << "uv_async_init() failed: " << uv_strerror(rc);
  uvAsync.async.data = this;
  uv_unref(&uvAsync.handle);
}

void EventLoop::uvAsyncCloseCallback(uv_handle_t* uvHandle) {
  CHECK_NOTNULL(uvHandle);
  auto* eventLoop = static_cast<EventLoop*>(uvHandle->data);
  eventLoop->uvAsyncClosed = true;
}

EventLoop::~EventLoop() {
  uv_close(&uvAsync.handle, uvAsyncCloseCallback);
  while (!uvAsyncClosed) {
    int rc = uv_run(&uvLoop, UV_RUN_ONCE);
    CHECK_EQ(rc, 0) << "uv_run() failed: " << uv_strerror(rc);
  }
  int rc = uv_loop_close(&uvLoop);
  CHECK_EQ(rc, 0) << "uv_loop_close() failed: " << uv_strerror(rc);
}

bool EventLoop::hasTask() {
  std::lock_guard<std::mutex> guard(taskQueueMutex);
  return !taskQueue.empty();
}

void EventLoop::processTasks() {
  std::list<Task> tasks;
  {
    std::lock_guard<std::mutex> guard(taskQueueMutex);
    taskQueue.swap(tasks);
  }
  for (const auto& task : tasks) {
    task();
  }
}

void EventLoop::Run() {
  while (uv_loop_alive(&uvLoop) != 0 || hasTask()) {
    uv_ref(&uvAsync.handle);
    uv_run(&uvLoop, UV_RUN_ONCE);
    uv_unref(&uvAsync.handle);
    processTasks();
  }
}

EventLoop& EventLoop::Default() {
  static std::atomic<bool> initialized(false);
  static std::mutex mutex;
  static std::shared_ptr<EventLoop> defaultLoop;
  if (initialized) {
    CHECK(defaultLoop != nullptr);
    return *defaultLoop;
  }
  std::lock_guard<std::mutex> lock(mutex);
  if (initialized) {
    CHECK(defaultLoop != nullptr);
    return *defaultLoop;
  }
  defaultLoop = std::make_shared<EventLoop>();
  initialized = true;
  return *defaultLoop;
}

std::shared_ptr<Timer> EventLoop::CreateTimer(const Timer::Options& options) {
  auto timer = Timer::Create(options, &uvLoop);
  return timer;
}

void EventLoop::SubmitTask(const Task& task) {
  {
    std::lock_guard<std::mutex> guard(taskQueueMutex);
    taskQueue.push_back(task);
  }
  uv_async_send(&uvAsync.async);
}

}  // namespace event
}  // namespace dk_pull
