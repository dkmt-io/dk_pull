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

#include <glog/logging.h>
#include <uv.h>

#include <cstdint>
#include <functional>
#include <memory>

#include "dk_pull/common/uncopyable.h"
#include "dk_pull/event/timer/timer.h"

namespace dk_pull {
namespace event {
namespace timer {

class UvTimer final : public Timer,
                      public std::enable_shared_from_this<UvTimer> {
 public:
  virtual ~UvTimer();

  virtual bool Stop();

  static std::shared_ptr<UvTimer> Create(const Options& options,
                                         uv_loop_t* uvLoop);

 private:
  static void uvCloseCallback(uv_handle_t* handle);

  static void uvTimerCallback(uv_timer_t* timer);

  UvTimer(const Options& options, uv_loop_t* uvLoop);

  bool start();

  void close();

  uv_any_handle uvHandle;

  std::shared_ptr<UvTimer> self;

  bool started = false;

  bool stopped = false;

  bool closed = false;

  DK_DECLARE_UNCOPYABLE(UvTimer);
};

}  // namespace timer
}  // namespace event
}  // namespace dk_pull
