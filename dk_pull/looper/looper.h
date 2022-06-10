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

#pragma once

#include <functional>
#include <memory>

#include "dk_pull/common/uncopyable.h"

namespace dk_pull {
namespace looper {

class Looper final : public std::enable_shared_from_this<Looper> {
 public:
  using LooperFunction = std::function<void(const std::function<void()>& next)>;

  static std::shared_ptr<Looper> Create(const LooperFunction& fn);

  void Start();

  ~Looper() = default;

 private:
  explicit Looper(const LooperFunction& fn);

  std::function<void()> makeNext();

  LooperFunction looperFunction;

  bool nextCalled = false;

  bool looperFunctionReturned = true;

  DK_DECLARE_UNCOPYABLE(Looper);
};

}  // namespace looper
}  // namespace dk_pull
