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

#include <atomic>
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <mutex>

#include "dk_pull/common/uncopyable.h"
#include "dk_pull/event/event_provider.h"
#include "dk_pull/event/timer/timer.h"

namespace dk_pull {
namespace event {
namespace event_loop {

class EventLoop {
 public:
  using Task = std::function<void()>;
  using Timer = dk_pull::event::timer::Timer;

  virtual ~EventLoop();

  virtual void Run() = 0;

  virtual std::shared_ptr<Timer> CreateTimer(const Timer::Options& options) = 0;

  static std::unique_ptr<EventLoop> Create(const EventProvider& provider);

 protected:
  EventLoop() = default;

 private:
  DK_DECLARE_UNCOPYABLE(EventLoop);
};

}  // namespace event_loop
}  // namespace event
}  // namespace dk_pull
