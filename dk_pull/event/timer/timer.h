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

#include <cstdint>
#include <functional>
#include <memory>

#include "dk_pull/common/uncopyable.h"

namespace dk_pull {
namespace event {
namespace timer {

class Timer {
 public:
  typedef std::function<void()> Callback;

  enum class Type : uint8_t {
    ONE_TIME = 0,  // one time execution
    REPEATED = 1,  // repeated execution
  };

  struct Options {
    Callback callback;  // The function to call when the timer elapses.
    uint64_t delay;     // Time (in ms) to wait before calling the callback.
    Type type;
  };

  virtual ~Timer() = default;

  virtual bool Stop() = 0;

 protected:
  Timer(const Options& options);

  Callback callback;

  uint64_t delay;

  Type type;

 private:
  DK_DECLARE_UNCOPYABLE(Timer);
};

}  // namespace timer
}  // namespace event
}  // namespace dk_pull
