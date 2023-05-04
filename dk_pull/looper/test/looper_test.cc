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

#include "dk_pull/looper/looper.h"

#include <random>

#include "dk_pull/event/event_loop/event_loop.h"
#include "gtest/gtest.h"

TEST(LooperTest, SyncTest) {
  using dk_pull::looper::Looper;
  constexpr int N = 1000000;
  int c = 0;
  auto looper = Looper::Create([&c](const std::function<void()>& next) {
    c++;
    if (c < N) {
      next();
    }
  });
  looper->Start();
  EXPECT_EQ(N, c);
}

TEST(LooperTest, AsyncTest) {
  using dk_pull::event::EventProvider;
  using dk_pull::event::event_loop::EventLoop;
  using dk_pull::looper::Looper;
  auto event_loop = EventLoop::Create(EventProvider::LIBUV);
  EXPECT_NE(nullptr, event_loop);
  constexpr int N = 100000;
  int c = 0;
  auto looper =
      Looper::Create([&c, &event_loop](const std::function<void()>& next) {
        c++;
        if (c < N) {
          event_loop->SetTimeout(0, [next]() { next(); });
        }
      });
  looper->Start();
  event_loop->Run();
  EXPECT_EQ(N, c);
}

TEST(LooperTest, RandomTest) {
  using dk_pull::event::EventProvider;
  using dk_pull::event::event_loop::EventLoop;
  using dk_pull::looper::Looper;
  auto event_loop = EventLoop::Create(EventProvider::LIBUV);
  EXPECT_NE(nullptr, event_loop);
  constexpr int N = 100000;
  int c = 0;
  auto looper =
      Looper::Create([&c, &event_loop](const std::function<void()>& next) {
        c++;
        if (c < N) {
          std::random_device rd;
          static thread_local std::mt19937 mt(rd());
          if (mt() % 2 == 0) {
            next();
            return;
          }
          event_loop->SetTimeout(0, next);
        }
      });
  looper->Start();
  event_loop->Run();
  EXPECT_EQ(N, c);
}
