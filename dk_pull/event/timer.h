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

#include <glog/logging.h>
#include <uv.h>

#include <cstdint>
#include <functional>
#include <memory>

#include "dk_pull/common/uncopyable.h"

namespace dk_pull {
namespace event {

class Timer final : public std::enable_shared_from_this<Timer> {
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

  static std::shared_ptr<Timer> Create(const Options& options,
                                       uv_loop_t* uvLoop);

  virtual ~Timer();

  void SetCallback(const Callback& cb);

  bool Stop();

 private:
  static void uvCloseCallback(uv_handle_t* handle);

  static void uvTimerCallback(uv_timer_t* timer);

  Timer(const Options& options, uv_loop_t* uvLoop);

  bool start();

  void close();

  Callback callback;

  uint64_t delay;

  Type type;

  uv_any_handle uvHandle;

  std::shared_ptr<Timer> self;

  bool started = false;

  bool stopped = false;

  bool closed = false;

  DK_DECLARE_UNCOPYABLE(Timer);
};

}  // namespace event
}  // namespace dk_pull
