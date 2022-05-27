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

#pragma once

#include <uv.h>

#include <cstdint>
#include <memory>

#include "dk_pull/common/uncopyable.h"
#include "dk_pull/event/timer.h"

namespace dk_pull {
namespace event {

class EventLoop final {
 public:
  EventLoop();

  void Run();

  std::shared_ptr<Timer> CreateTimer(const Timer::Options& options);

  ~EventLoop();

  static EventLoop& Default();

 private:
  uv_loop_t uvLoop;

  DK_DECLARE_UNCOPYABLE(EventLoop);
};

}  // namespace event
}  // namespace dk_pull
