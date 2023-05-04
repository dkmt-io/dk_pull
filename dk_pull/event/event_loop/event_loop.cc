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

#include "dk_pull/event/event_loop/event_loop.h"

#include <glog/logging.h>

#include <memory>

#include "dk_pull/event/event_loop/uv_event_loop.h"

namespace dk_pull {
namespace event {
namespace event_loop {

std::unique_ptr<EventLoop> EventLoop::Create(const EventProvider& provider) {
  switch (provider) {
    case EventProvider::LIBUV: {
      return std::make_unique<UvEventLoop>();
    }
    default: {
      LOG(FATAL) << "Unknown provider: " << static_cast<int>(provider);
    }
  }
  return nullptr;
}

std::shared_ptr<::dk_pull::event::timer::Timer> EventLoop::SetInterval(
    uint64_t ms, const Timer::Callback& cb) {
  using dk_pull::event::event_loop::EventLoop;
  auto timer = CreateTimer({
      .callback = cb,
      .delay = ms,
      .type = Timer::Type::REPEATED,
  });
  return timer;
}

std::shared_ptr<::dk_pull::event::timer::Timer> EventLoop::SetTimeout(
    uint64_t ms, const Timer::Callback& cb) {
  using dk_pull::event::event_loop::EventLoop;
  auto timer = CreateTimer({
      .callback = cb,
      .delay = ms,
      .type = Timer::Type::ONE_TIME,
  });
  return timer;
}

}  // namespace event_loop
}  // namespace event
}  // namespace dk_pull