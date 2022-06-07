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

#include "dk_pull/pull_stream/sinks/drain.h"

#include <gtest/gtest.h>

#include <utility>
#include <vector>

#include "dk_pull/pull_stream/sources/values.h"

TEST(DrainTest, BasicTest) {
  using dk_pull::pull_stream::sinks::Drain;
  using dk_pull::pull_stream::sources::Values;
  using dk_pull::types::Done;
  auto source = Values<int>::Create({1, 2, 3});
  int counter = 0;
  Drain<int>::OnDone onDone = [&counter](const Done& done) {
    EXPECT_EQ(3, counter);
    EXPECT_TRUE(done);
    EXPECT_FALSE(done.IsError());
    counter++;
  };
  Drain<int>::OnValue onValue = [&counter](int&& v) {
    counter++;
    EXPECT_EQ(counter, v);
  };
  auto sink = Drain<int>::Create(onValue, onDone);
  sink(source);
  EXPECT_EQ(4, counter);
}
