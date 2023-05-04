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
#include "dk_pull/event/event_loop/event_loop.h"

namespace dk_pull {
namespace event {
namespace event_loop {

class UvEventLoop final : public EventLoop {
 public:
  using Timer = dk_pull::event::timer::Timer;

  UvEventLoop();

  virtual ~UvEventLoop();

  void Run() override;

  std::shared_ptr<Timer> CreateTimer(const Timer::Options& options) override;

 private:
  uv_loop_t uvLoop;

  DK_DECLARE_UNCOPYABLE(UvEventLoop);
};

}  // namespace event_loop
}  // namespace event
}  // namespace dk_pull
