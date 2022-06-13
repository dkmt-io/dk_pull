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

#include <gtest/gtest.h>

#include <array>
#include <functional>
#include <thread>

#include "dk_pull/event/event_loop.h"

namespace dk_pull {
namespace event {
namespace test {

TEST(SetImmediateTest, SubmitFromMainThread) {
  auto defaultTid = std::this_thread::get_id();
  std::atomic<int> counter(0);
  auto task = [defaultTid, &counter]() {
    EXPECT_EQ(defaultTid, std::this_thread::get_id());
    counter.fetch_add(1);
  };
  SetImmediate(task);
  SetImmediate(task);
  EXPECT_EQ(0, counter.load());
  EventLoop::Default().Run();
  EXPECT_EQ(2, counter.load());
}

TEST(SetImmediateTest, SubmitFromAnotherThread) {
  auto defaultTid = std::this_thread::get_id();
  std::atomic<int> counter(0);
  std::thread t([defaultTid, &counter]() {
    auto task = [defaultTid, &counter]() {
      EXPECT_EQ(defaultTid, std::this_thread::get_id());
      counter.fetch_add(1);
    };
    SetImmediate(task);
    SetImmediate(task);
    SetImmediate(task);
    SetImmediate(task);
    SetImmediate(task);
  });
  t.join();
  EventLoop::Default().Run();
  EXPECT_EQ(5, counter.load());
}

TEST(SetImmediateTest, SubmitFromMultipleThreads) {
  auto defaultTid = std::this_thread::get_id();
  std::atomic<int> counter(0);
  auto task = [defaultTid, &counter]() {
    SetImmediate([defaultTid, &counter]() {
      EXPECT_EQ(defaultTid, std::this_thread::get_id());
      counter.fetch_add(1);
    });
  };
  std::array<std::unique_ptr<std::thread>, 1024> threads;  // NOLINT
  for (auto& thread : threads) {
    thread.reset(new std::thread(task));
  }
  for (const auto& thread : threads) {
    thread->join();
  }
  EventLoop::Default().Run();
  EXPECT_EQ(1024, counter.load());
}

TEST(SetImmediateTest, SubmitFromTask) {
  std::atomic<int> counter(0);
  std::function<void()> task;
  task = [&]() {
    int c = counter.fetch_add(1);
    if (c == 1023) {  // NOLINT
      return;
    }
    SetImmediate(task);
  };
  SetImmediate(task);
  EventLoop::Default().Run();
  EXPECT_EQ(1024, counter.load());
}

}  // namespace test
}  // namespace event
}  // namespace dk_pull
