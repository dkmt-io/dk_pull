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

#include "dk_pull/event/timer.h"

#include <gtest/gtest.h>
#include <uv.h>

#include <atomic>
#include <thread>

#include "dk_pull/event/event_loop.h"
#include "dk_pull/event/set_interval.h"
#include "dk_pull/event/set_timeout.h"

namespace dk_pull {
namespace event {
namespace test {

TEST(TimerTest, TestSetTimeout) {
  std::atomic<int> count(0);
  SetTimeout(100, [&count]() { count++; });
  auto timer = SetTimeout(100, [&count]() { count++; });
  EventLoop::Default().Run();
  EXPECT_EQ(2, count);
}

TEST(TimerTest, SetInterval) {
  std::atomic<int> count(0);
  std::shared_ptr<Timer> timer;
  timer = SetInterval(10, [&count, &timer]() {
    count++;
    if (count == 10) {
      timer->Stop();
    }
  });
  EventLoop::Default().Run();
  EXPECT_EQ(10, count);
}

void TestOneTimeTimer(EventLoop* loop) {
  std::atomic<int> count(0);
  loop->CreateTimer({
      .callback = [&count]() { count++; },
      .delay = 100,
      .type = Timer::Type::ONE_TIME,
  });
  auto timer = loop->CreateTimer({
      .callback = [&count]() { count++; },
      .delay = 100,
      .type = Timer::Type::ONE_TIME,
  });
  loop->Run();
  EXPECT_EQ(2, count);
}

void TestRepeatedTimer(EventLoop* loop) {
  std::atomic<int> count(0);
  std::shared_ptr<Timer> timer;
  timer = loop->CreateTimer({
      .callback =
          [&count, &timer]() {
            count++;
            if (count == 10) {
              timer->Stop();
            }
          },
      .delay = 10,
      .type = Timer::Type::REPEATED,
  });
  loop->Run();
  EXPECT_EQ(10, count);
}

TEST(TimerTest, MultiThreadedTest) {
  std::thread t1([]() {
    EventLoop loop;
    TestOneTimeTimer(&loop);
    TestRepeatedTimer(&loop);
  });
  std::thread t2([]() {
    EventLoop loop;
    TestRepeatedTimer(&loop);
    TestOneTimeTimer(&loop);
  });
  t1.join();
  t2.join();
}

}  // namespace test
}  // namespace event
}  // namespace dk_pull
