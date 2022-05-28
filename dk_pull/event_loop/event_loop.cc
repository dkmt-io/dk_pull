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

#include "dk_pull/event_loop/event_loop.h"

#include <glog/logging.h>

namespace dk_pull {
namespace event_loop {

std::shared_ptr<EventLoop> EventLoop::Create() {
  std::shared_ptr<EventLoop> result(new EventLoop());
  return result;
}

EventLoop::EventLoop() : uvLoop() {
  int rc = uv_loop_init(&uvLoop);
  CHECK_EQ(0, rc);
}

EventLoop::~EventLoop() {
  int rc = uv_loop_close(&uvLoop);
  CHECK_EQ(0, rc);
}

void EventLoop::Run() {
  int rc = uv_run(&uvLoop, UV_RUN_DEFAULT);
  CHECK_EQ(0, rc);
}

}  // namespace event_loop
}  // namespace dk_pull
