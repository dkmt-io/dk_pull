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

#include "dk_pull/pull_stream/protocol_checker.h"

#include <vector>

#include "dk_pull/pull_stream/pull.h"
#include "dk_pull/pull_stream/sinks/drain.h"
#include "dk_pull/pull_stream/sources/count.h"
#include "dk_pull/pull_stream/sources/values.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

TEST(ProtocolCheckerTest, BasicTest) {
  using dk_pull::pull_stream::ProtocolChecker;
  using dk_pull::pull_stream::Pull;
  using dk_pull::pull_stream::sinks::Drain;
  using dk_pull::pull_stream::sources::Count;
  auto checker = ProtocolChecker<uint64_t>::Create({
      .forbid_extra_requests = true,
      .enforce_stream_termination = true,
      .notify_eagerly = true,
  });
  auto count = Count::Create(10);  // NOLINT
  bool finished = false;
  Drain<uint64_t>::OnValue onValue = [](auto v) {
    LOG(INFO) << "On value: " << v;
  };
  Drain<uint64_t>::OnDone onDone =
      [checker, &finished](const dk_pull::types::Done& done) {
        auto errors = checker->Terminate();
        EXPECT_EQ(0, errors.size());
        finished = true;
      };
  auto drain = Drain<uint64_t>::Create(onValue, onDone);
  Pull(count->Source(), checker->Through(), drain->Sink());
}

TEST(ProtocolCheckerTest, TestInvariant1) {
  using dk_pull::pull_stream::ProtocolChecker;
  using dk_pull::pull_stream::Pull;
  using dk_pull::pull_stream::sources::Count;
  using dk_pull::types::Abort;
  auto checker = ProtocolChecker<uint64_t>::Create({
      .forbid_extra_requests = true,
      .enforce_stream_termination = true,
      .notify_eagerly = true,
  });
  auto count = Count::Create(10);  // NOLINT
  auto source = Pull(count->Source(), checker->Through());
  source(Abort::TRUE, nullptr);
  bool error_emitted = false;
  try {
    source(Abort::FALSE, nullptr);
  } catch (const std::logic_error& e) {
    error_emitted = true;
    EXPECT_STREQ("Invariant 1 violated: value requested after termination",
                 e.what());
  }
  EXPECT_TRUE(error_emitted);
}

TEST(ProtocolCheckerTest, TestInvariant2) {
  using dk_pull::pull_stream::ProtocolChecker;
  using dk_pull::pull_stream::Pull;
  using dk_pull::pull_stream::sources::Count;
  using dk_pull::types::Abort;
  using dk_pull::types::Source;
  using dk_pull::types::SourceCallback;
  auto checker = ProtocolChecker<int>::Create({
      .forbid_extra_requests = true,
      .enforce_stream_termination = true,
      .notify_eagerly = true,
  });
  auto source = Pull(Source<int>([](auto /*unused*/, auto /*unused*/) {}),
                     checker->Through());
  source(Abort::FALSE, nullptr);
  bool error_emitted = false;
  try {
    checker->Terminate();
  } catch (const std::logic_error& e) {
    error_emitted = true;
    EXPECT_STREQ("Invariant 2 violated: callback 1 was never invoked",
                 e.what());
  }
  EXPECT_TRUE(error_emitted);
}

TEST(ProtocolCheckerTest, TestInvariant3) {
  using dk_pull::pull_stream::ProtocolChecker;
  using dk_pull::pull_stream::Pull;
  using dk_pull::pull_stream::sources::Count;
  using dk_pull::types::Abort;
  using dk_pull::types::Done;
  using dk_pull::types::Source;
  using dk_pull::types::SourceCallback;
  auto checker = ProtocolChecker<int>::Create({
      .forbid_extra_requests = true,
      .enforce_stream_termination = true,
      .notify_eagerly = true,
  });
  SourceCallback<int> fn;
  auto source = Pull(Source<int>([&fn](auto /*unused*/, auto cb) { fn = cb; }),
                     checker->Through());
  source(Abort::FALSE, [](auto /*unused*/, auto /*unused*/) {});
  fn(Done::FALSE, 1);
  bool error_emitted = false;
  try {
    fn(Done::FALSE, 1);
    checker->Terminate();
  } catch (const std::logic_error& e) {
    error_emitted = true;
    EXPECT_STREQ("Invariant 3 violated: callback 1 invoked 2 times", e.what());
  }
  EXPECT_TRUE(error_emitted);
}

TEST(ProtocolCheckerTest, TestInvariant4) {
  using dk_pull::pull_stream::ProtocolChecker;
  using dk_pull::pull_stream::Pull;
  using dk_pull::types::Abort;
  using dk_pull::types::Done;
  using dk_pull::types::Source;
  using dk_pull::types::SourceCallback;
  auto checker = ProtocolChecker<int>::Create({
      .forbid_extra_requests = true,
      .enforce_stream_termination = true,
      .notify_eagerly = true,
  });
  std::vector<SourceCallback<int>> cbs;
  auto source =
      Pull(Source<int>([&cbs](auto /*unused*/, auto cb) { cbs.push_back(cb); }),
           checker->Through());
  source(Abort::FALSE, [](auto /*unused*/, auto /*unused*/) {});
  source(Abort::FALSE, [](auto /*unused*/, auto /*unused*/) {});
  bool error_emitted = false;
  try {
    cbs[1](Done::FALSE, 1);
    cbs[0](Done::FALSE, 1);
    checker->Terminate();
  } catch (const std::logic_error& e) {
    error_emitted = true;
    EXPECT_STREQ("Invariant 4 violated: callback 2 invoked before callback 1",
                 e.what());
  }
  EXPECT_TRUE(error_emitted);
}

TEST(ProtocolCheckerTest, TestInvariant5) {
  using dk_pull::pull_stream::ProtocolChecker;
  using dk_pull::pull_stream::Pull;
  using dk_pull::pull_stream::sources::Count;
  using dk_pull::types::Abort;
  using dk_pull::types::Done;
  using dk_pull::types::Source;
  using dk_pull::types::SourceCallback;
  auto checker = ProtocolChecker<int>::Create();
  SourceCallback<int> fn;
  auto source = Pull(Source<int>([&fn](auto /*unused*/, auto cb) { fn = cb; }),
                     checker->Through());
  source(Abort::FALSE, [](auto /*unused*/, auto /*unused*/) {});
  fn(Done::TRUE, 1);
  source(Abort::TRUE, [](auto /*unused*/, auto /*unused*/) {});
  bool error_emitted = false;
  try {
    fn(Done::FALSE, 1);
  } catch (const std::logic_error& e) {
    error_emitted = true;
    EXPECT_STREQ(
        "Invariant 5 violated: callback 2 returned a value after termination",
        e.what());
  }
  EXPECT_TRUE(error_emitted);
}

TEST(ProtocolCheckerTest, TestInvariant6RequestAfterAborted) {
  using dk_pull::pull_stream::ProtocolChecker;
  using dk_pull::pull_stream::Pull;
  using dk_pull::pull_stream::sources::Count;
  using dk_pull::types::Abort;
  using dk_pull::types::Done;
  using dk_pull::types::Source;
  using dk_pull::types::SourceCallback;
  auto checker = ProtocolChecker<uint64_t>::Create({
      .forbid_extra_requests = true,
  });
  auto count = Count::Create(0);
  auto source = Pull(count->Source(), checker->Through());
  source(Abort::TRUE, [](auto /*unused*/, auto /*unused*/) {});
  bool error_emitted = false;
  try {
    source(Abort::TRUE, [](auto /*unused*/, auto /*unused*/) {});
  } catch (const std::logic_error& e) {
    error_emitted = true;
    EXPECT_STREQ(
        "Invariant 6 violated: request made after the stream was aborted",
        e.what());
  }
  EXPECT_TRUE(error_emitted);
}

TEST(ProtocolCheckerTest, TestInvariant6RequestAfterTerminated) {
  using dk_pull::pull_stream::ProtocolChecker;
  using dk_pull::pull_stream::Pull;
  using dk_pull::pull_stream::sources::Count;
  using dk_pull::types::Abort;
  using dk_pull::types::Done;
  using dk_pull::types::Source;
  using dk_pull::types::SourceCallback;
  auto checker = ProtocolChecker<uint64_t>::Create({
      .forbid_extra_requests = true,
  });
  auto count = Count::Create(1);
  auto source = Pull(count->Source(), checker->Through());
  source(Abort::FALSE, [](auto /*unused*/, auto /*unused*/) {});
  source(Abort::FALSE, [](auto /*unused*/, auto /*unused*/) {});
  bool error_emitted = false;
  try {
    source(Abort::FALSE, [](auto /*unused*/, auto /*unused*/) {});
    source(Abort::TRUE, [](auto /*unused*/, auto /*unused*/) {});
  } catch (const std::logic_error& e) {
    error_emitted = true;
    EXPECT_STREQ(
        "Invariant 6 violated: request made after the stream has terminated",
        e.what());
  }
  EXPECT_TRUE(error_emitted);
}

TEST(ProtocolCheckerTest, TestInvariant7) {
  using dk_pull::pull_stream::ProtocolChecker;
  using dk_pull::pull_stream::Pull;
  using dk_pull::pull_stream::sources::Count;
  using dk_pull::types::Abort;
  using dk_pull::types::Done;
  using dk_pull::types::Source;
  using dk_pull::types::SourceCallback;
  auto checker = ProtocolChecker<int>::Create({
      .forbid_extra_requests = true,
      .enforce_stream_termination = true,
  });
  auto source =
      Pull(Source<int>([](auto /*unused*/, auto cb) { cb(Done::FALSE, 1); }),
           checker->Through());
  source(Abort::FALSE, [](auto /*unused*/, auto /*unused*/) {});
  source(Abort::FALSE, [](auto /*unused*/, auto /*unused*/) {});
  bool error_emitted = false;
  try {
    checker->Terminate();
  } catch (const std::logic_error& e) {
    error_emitted = true;
    EXPECT_STREQ("Invariant 7 violated: stream was never terminated", e.what());
  }
  EXPECT_TRUE(error_emitted);
}

TEST(ProtocolCheckerTest, NotifyDuringTermination) {
  using dk_pull::pull_stream::ProtocolChecker;
  using dk_pull::pull_stream::Pull;
  using dk_pull::pull_stream::sources::Count;
  using dk_pull::types::Abort;
  using dk_pull::types::Done;
  using dk_pull::types::Source;
  using dk_pull::types::SourceCallback;
  auto checker = ProtocolChecker<int>::Create({
      .forbid_extra_requests = true,
      .enforce_stream_termination = true,
      .notify_eagerly = false,
  });
  auto source =
      Pull(Source<int>([](auto /*unused*/, auto cb) { cb(Done::FALSE, 1); }),
           checker->Through());
  source(Abort::FALSE, [](auto /*unused*/, auto /*unused*/) {});
  source(Abort::FALSE, [](auto /*unused*/, auto /*unused*/) {});
  auto errors = checker->Terminate();
  EXPECT_EQ(1, errors.size());
  EXPECT_STREQ("Invariant 7 violated: stream was never terminated",
               errors[0].c_str());
}
