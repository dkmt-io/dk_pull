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

#include "dk_pull/pull_stream/sources/values.h"

#include <gtest/gtest.h>

#include <atomic>
#include <utility>
#include <vector>

#include "dk_pull/pull_stream/protocol_checker.h"
#include "dk_pull/pull_stream/pull.h"
#include "dk_pull/pull_stream/sinks/collect.h"

TEST(ValuesTest, BasicTest) {
  using dk_pull::pull_stream::sources::Values;
  using dk_pull::types::Abort;
  using dk_pull::types::Done;
  auto values = Values<int>::Create({1, 2, 3});
  auto source = values->Source();
  bool callbackInvoked = false;
  source(Abort::FALSE, [&callbackInvoked](const Done& done, int&& v) {
    callbackInvoked = true;
    EXPECT_FALSE(done);
    EXPECT_EQ(1, v);
  });
  EXPECT_EQ(true, callbackInvoked);
  callbackInvoked = false;
  source(Abort::FALSE, [&callbackInvoked](const Done& done, int&& v) {
    callbackInvoked = true;
    EXPECT_FALSE(done);
    EXPECT_EQ(2, v);
  });
  EXPECT_EQ(true, callbackInvoked);
  callbackInvoked = false;
  source(Abort::FALSE, [&callbackInvoked](const Done& done, int&& v) {
    callbackInvoked = true;
    EXPECT_FALSE(done);
    EXPECT_EQ(3, v);
  });
  EXPECT_EQ(true, callbackInvoked);
  callbackInvoked = false;
  source(Abort::FALSE, [&callbackInvoked](const Done& done, int&& v) {
    callbackInvoked = true;
    EXPECT_TRUE(done);
    EXPECT_TRUE(done.IsEnd());
    EXPECT_FALSE(done.IsError());
  });
  EXPECT_EQ(true, callbackInvoked);
}

TEST(ValuesTest, TestCollectedValues) {
  using dk_pull::pull_stream::ProtocolChecker;
  using dk_pull::pull_stream::Pull;
  using dk_pull::pull_stream::sinks::MakeCollect;
  using dk_pull::pull_stream::sources::Values;
  using dk_pull::types::Abort;
  using dk_pull::types::Done;
  std::atomic<bool> ended = false;
  auto values = Values<int>::Create({1, 2, 3});
  auto source = values->Source();
  auto checker = ProtocolChecker<int>::Create({
      .forbid_extra_requests = true,
      .enforce_stream_termination = true,
      .notify_eagerly = true,
  });
  auto probe = checker->Through();
  auto sink = MakeCollect<int>(
      [checker, &ended](const Done& done, std::vector<int>&& vector) {
        EXPECT_FALSE(done.IsError());
        EXPECT_TRUE(done.IsEnd());
        EXPECT_FALSE(vector.empty());
        EXPECT_TRUE(checker->Terminate().empty());
        EXPECT_EQ(vector, std::vector<int>({1, 2, 3}));
        ended = true;
      });
  Pull(source, probe, sink);
  EXPECT_TRUE(ended.load());
}

TEST(ValuesTest, TestAbort) {
  using dk_pull::pull_stream::ProtocolChecker;
  using dk_pull::pull_stream::Pull;
  using dk_pull::pull_stream::sinks::MakeCollect;
  using dk_pull::pull_stream::sources::Values;
  using dk_pull::types::Abort;
  using dk_pull::types::Done;
  std::atomic<bool> ended = false;
  auto values = Values<int>::Create({1, 2, 3});
  auto checker = ProtocolChecker<int>::Create({
      .forbid_extra_requests = true,
      .enforce_stream_termination = true,
      .notify_eagerly = true,
  });
  auto probe = checker->Through();
  auto source = Pull(values->Source(), probe);
  source(Abort::FALSE, [checker, source, &ended](auto done, auto v) {
    EXPECT_FALSE(done);
    EXPECT_EQ(1, v);
    Abort abort("intentional");
    source(abort, [checker, &ended](auto done, auto /*v*/) {
      EXPECT_TRUE(done.IsError());
      EXPECT_FALSE(done.IsEnd());
      EXPECT_STREQ("intentional", done.GetErrorMessage().c_str());
      EXPECT_TRUE(checker->Terminate().empty());
      ended = true;
    });
  });
  EXPECT_TRUE(ended.load());
}
