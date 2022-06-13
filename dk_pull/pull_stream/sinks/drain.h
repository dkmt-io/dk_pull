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

#include <memory>
#include <utility>

#include "dk_pull/common/uncopyable.h"
#include "dk_pull/looper/looper.h"
#include "dk_pull/types/sink.h"
#include "glog/logging.h"

namespace dk_pull {
namespace pull_stream {
namespace sinks {

template <typename T>
class Drain final : public dk_pull::types::SinkContext<T>,
                    public std::enable_shared_from_this<Drain<T>> {
 public:
  using OnDone = std::function<void(const dk_pull::types::Done&)>;
  using OnValue = std::function<void(T&&)>;

  static std::shared_ptr<Drain> Create(const OnValue& fn, const OnDone& done) {
    auto self = std::shared_ptr<Drain>(new Drain(fn, done));
    return self;
  }

  dk_pull::types::Sink<T> Sink() override {
    auto self = this->shared_from_this();
    return [self](const dk_pull::types::Source<T>& source) {
      using dk_pull::looper::Looper;
      using dk_pull::types::Abort;
      auto looper = Looper::Create([self, source](const auto& next) {
        source(Abort::FALSE, [self, next](const auto& e, T&& val) {
          if (e) {
            self->onDone(e);
            return;
          }
          self->onValue(std::move(val));
          next();
        });
      });
      looper->Start();
    };
  }

  virtual ~Drain() = default;

 private:
  Drain(const OnValue& fn, const OnDone& done) : onValue(fn), onDone(done) {
    if (onValue == nullptr) {
      onValue = [](auto /*unused*/) {};
    }
    if (onDone == nullptr) {
      onDone = [](auto /*unused*/) {};
    }
  }

  OnValue onValue;

  OnDone onDone;

  DK_DECLARE_UNCOPYABLE(Drain);
};

}  // namespace sinks
}  // namespace pull_stream
}  // namespace dk_pull
