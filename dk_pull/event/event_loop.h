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

#pragma once

#include <uv.h>

#include <atomic>
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <mutex>

#include "dk_pull/common/uncopyable.h"
#include "dk_pull/event/timer.h"

namespace dk_pull {
namespace event {

class EventLoop final {
 public:
  using Task = std::function<void()>;

  EventLoop();

  void Run();

  std::shared_ptr<Timer> CreateTimer(const Timer::Options& options);

  void SubmitTask(const Task& task);

  ~EventLoop();

  static EventLoop& Default();

 private:
  static void uvAsyncCloseCallback(uv_handle_t* /*uvHandle*/);

  void processTasks();

  bool hasTask();

  uv_loop_t uvLoop;
  uv_any_handle uvAsync;
  std::atomic<bool> uvAsyncClosed;
  std::list<Task> taskQueue;
  std::mutex taskQueueMutex;

  DK_DECLARE_UNCOPYABLE(EventLoop);
};

}  // namespace event
}  // namespace dk_pull
