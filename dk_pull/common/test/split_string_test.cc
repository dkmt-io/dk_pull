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

#include "dk_pull/common/split_string.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace dk_pull {
namespace common {

TEST(SplitStringTest, Case1) {
  std::string s;
  std::vector<std::string> splits = {"1", "2"};
  SplitString(&splits, s, 'a');
  EXPECT_EQ(0, splits.size());
}

TEST(SplitStringTest, Case1SV) {
  std::string s;
  std::vector<std::string_view> splits = {"1", "2"};
  SplitString(&splits, s, 'a');
  EXPECT_EQ(0, splits.size());
}

TEST(SplitStringTest, Case2) {
  std::string s = ",a";
  std::vector<std::string> splits = {"1", "2"};
  SplitString(&splits, s, ',');
  std::vector<std::string> expected = {"", "a"};
  EXPECT_EQ(expected, splits);
}

TEST(SplitStringTest, Case2SV) {
  std::string s = ",a";
  std::vector<std::string_view> splits = {"1", "2"};
  SplitString(&splits, s, ',');
  std::vector<std::string_view> expected = {"", "a"};
  EXPECT_EQ(expected, splits);
}

TEST(SplitStringTest, Case3) {
  std::string s = ",,";
  std::vector<std::string> splits = {"1", "2"};
  SplitString(&splits, s, ',');
  std::vector<std::string> expected = {"", "", ""};
  EXPECT_EQ(expected, splits);
}

TEST(SplitStringTest, Case3SV) {
  std::string s = ",,";
  std::vector<std::string_view> splits = {"1", "2"};
  SplitString(&splits, s, ',');
  std::vector<std::string_view> expected = {"", "", ""};
  EXPECT_EQ(expected, splits);
}

TEST(SplitStringTest, Case4) {
  std::string s = "ab";
  std::vector<std::string> splits = {"1", "2"};
  SplitString(&splits, s, ',');
  std::vector<std::string> expected = {"ab"};
  EXPECT_EQ(expected, splits);
}

TEST(SplitStringTest, Case4SV) {
  std::string s = "ab";
  std::vector<std::string_view> splits = {"1", "2"};
  SplitString(&splits, s, ',');
  std::vector<std::string_view> expected = {"ab"};
  EXPECT_EQ(expected, splits);
}

TEST(SplitStringTest, Case5) {
  std::string s = "ab";
  std::vector<std::string> splits = {"1", "2"};
  SplitString(&splits, s, ',');
  std::vector<std::string> expected = {"ab"};
  EXPECT_EQ(expected, splits);
}

TEST(SplitStringTest, Case5SV) {
  std::string s = "ab";
  std::vector<std::string_view> splits = {"1", "2"};
  SplitString(&splits, s, ',');
  std::vector<std::string_view> expected = {"ab"};
  EXPECT_EQ(expected, splits);
}

TEST(SplitStringTest, Case6) {
  std::string s = "ab,";
  std::vector<std::string> splits = {"1", "2"};
  SplitString(&splits, s, ',');
  std::vector<std::string> expected = {"ab", ""};
  EXPECT_EQ(expected, splits);
}

TEST(SplitStringTest, Case6SV) {
  std::string s = "ab,";
  std::vector<std::string_view> splits = {"1", "2"};
  SplitString(&splits, s, ',');
  std::vector<std::string_view> expected = {"ab", ""};
  EXPECT_EQ(expected, splits);
}

TEST(SplitStringTest, Case7) {
  std::string s = "a x bc x defg";
  std::vector<std::string> splits = {"1", "2"};
  SplitString(&splits, s, 'x');
  std::vector<std::string> expected = {"a ", " bc ", " defg"};
  EXPECT_EQ(expected, splits);
}

TEST(SplitStringTest, Case7SV) {
  std::string s = "a x bc x defg";
  std::vector<std::string_view> splits = {"1", "2"};
  SplitString(&splits, s, 'x');
  std::vector<std::string_view> expected = {"a ", " bc ", " defg"};
  EXPECT_EQ(expected, splits);
}

}  // namespace common
}  // namespace dk_pull
