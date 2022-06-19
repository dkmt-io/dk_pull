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

#include "dk_pull/event/set_immediate.h"

#include <functional>
#include <memory>

#include "dk_pull/event/event_loop.h"

namespace dk_pull {
namespace event {

void SetImmediate(const std::function<void()>& cb) {
  using dk_pull::event::EventLoop;
  EventLoop::Default().SubmitTask(cb);
}

}  // namespace event
}  // namespace dk_pull
