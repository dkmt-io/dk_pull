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

#include <glog/logging.h>
#include <gtest/gtest.h>
#include <uv.h>

#include <atomic>
#include <thread>

#include "dk_pull/event/event_loop/event_loop.h"

namespace dk_pull {
namespace event {
namespace test {

using dk_pull::event::EventProvider;
using dk_pull::event::event_loop::EventLoop;
using dk_pull::event::timer::Timer;

TEST(TimerTest, TestSetTimeout) {
  auto loop = EventLoop::Create(EventProvider::LIBUV);
  EXPECT_NE(nullptr, loop);
  loop->Run();
  std::atomic<int> count(0);
  loop->SetTimeout(100, [&count]() { count++; });               // NOLINT
  auto timer = loop->SetTimeout(100, [&count]() { count++; });  // NOLINT
  loop->Run();
  EXPECT_EQ(2, count);
}

TEST(TimerTest, SetInterval) {
  std::atomic<int> count(0);
  auto loop = EventLoop::Create(EventProvider::LIBUV);
  std::shared_ptr<Timer> timer;
  timer = loop->SetInterval(1000, [&count, &timer]() {  // NOLINT
    LOG(INFO) << count++;
    if (count == 100) {  // NOLINT
      timer->Stop();
    }
  });
  loop->Run();
  EXPECT_EQ(100, count);
}

void TestOneTimeTimer(EventLoop* loop) {
  std::atomic<int> count(0);
  loop->CreateTimer({
      .callback = [&count]() { count++; },
      .delay = 100,  // NOLINT
      .type = Timer::Type::ONE_TIME,
  });
  auto timer = loop->CreateTimer({
      .callback = [&count]() { count++; },
      .delay = 100,  // NOLINT
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
            if (count == 10) {  // NOLINT
              timer->Stop();
            }
          },
      .delay = 10,  // NOLINT
      .type = Timer::Type::REPEATED,
  });
  loop->Run();
  EXPECT_EQ(10, count);
}

TEST(TimerTest, MultiThreadedTest) {
  std::thread t1([]() {
    auto loop = EventLoop::Create(EventProvider::LIBUV);
    EXPECT_NE(nullptr, loop);
    TestOneTimeTimer(loop.get());
    TestRepeatedTimer(loop.get());
  });
  std::thread t2([]() {
    auto loop = EventLoop::Create(EventProvider::LIBUV);
    TestRepeatedTimer(loop.get());
    TestOneTimeTimer(loop.get());
  });
  t1.join();
  t2.join();
}

}  // namespace test
}  // namespace event
}  // namespace dk_pull
