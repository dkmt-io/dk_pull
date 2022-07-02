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

#include "dk_pull/pull_stream/sinks/collect.h"

#include <gtest/gtest.h>

#include <atomic>
#include <utility>
#include <vector>

#include "dk_pull/pull_stream/protocol_checker.h"
#include "dk_pull/pull_stream/pull.h"
#include "dk_pull/pull_stream/sources/empty.h"

TEST(CollectTest, BasicTest) {
  using dk_pull::pull_stream::ProtocolChecker;
  using dk_pull::pull_stream::Pull;
  using dk_pull::pull_stream::sinks::MakeCollect;
  using dk_pull::types::Done;
  std::atomic<bool> ended = false;
  auto source = dk_pull::pull_stream::sources::MakeEmpty<int>();
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
        EXPECT_TRUE(vector.empty());
        EXPECT_TRUE(checker->Terminate().empty());
        ended = true;
      });
  Pull(source, probe, sink);
  EXPECT_TRUE(ended.load());
}
