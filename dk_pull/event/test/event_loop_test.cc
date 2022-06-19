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

#include <gtest/gtest.h>

namespace dk_pull {
namespace event {
namespace test {

TEST(EventLoopTest, BasicTest) {
  using dk_pull::event::EventLoop;
  EventLoop loop;
  loop.Run();
}

TEST(EventLoopTest, DefaultLoopTest) {
  using dk_pull::event::EventLoop;
  EventLoop::Default().Run();
}

}  // namespace test
}  // namespace event
}  // namespace dk_pull
