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

#pragma once

#include <functional>

#include "dk_pull/common/uncopyable.h"

namespace dk_pull {
namespace common {

class Finally final {
 public:
  explicit Finally(const std::function<void()>& fn);

  ~Finally();

 private:
  std::function<void()> action;

  DK_DECLARE_UNCOPYABLE(Finally);
};

}  // namespace common
}  // namespace dk_pull
