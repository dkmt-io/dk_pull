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

namespace dk_pull {
namespace common {

namespace {

template <typename StringType>
void DoSplitString(std::vector<StringType>* result, const StringType& full,
                   char delim) {
  result->clear();
  if (full.empty()) {
    return;
  }
  size_t pos = 0;
  size_t prev_pos = 0;
  StringType token;
  while ((pos = full.find(delim, prev_pos)) != std::string::npos) {
    token = full.substr(prev_pos, pos - prev_pos);
    result->push_back(token);
    prev_pos = pos + 1;
  }
  token = full.substr(prev_pos, full.length() - prev_pos);
  result->push_back(token);
}

}  // namespace

void SplitString(std::vector<std::string>* result, const std::string& full,
                 char delim) {
  DoSplitString<std::string>(result, full, delim);
}

void SplitString(std::vector<std::string_view>* result,
                 const std::string_view& full, char delim) {
  DoSplitString<std::string_view>(result, full, delim);
}

}  // namespace common
}  // namespace dk_pull
