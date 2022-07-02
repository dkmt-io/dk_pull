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
#include "dk_pull/pull_stream/sinks/drain.h"
#include "dk_pull/types/sink.h"
#include "dk_pull/types/source.h"

namespace dk_pull {
namespace pull_stream {
namespace sinks {

template <typename Accumulator, typename T>
class Reduce final
    : public dk_pull::types::SinkContext<T>,
      public std::enable_shared_from_this<Reduce<Accumulator, T>> {
 public:
  using Callback = dk_pull::types::SourceCallback<Accumulator>;
  using Reducer = std::function<Accumulator(Accumulator&&, T&&)>;

  static std::shared_ptr<Reduce> Create(
      const Reducer& reducer, const Callback& cb,
      Accumulator&& accumulator = Accumulator()) {
    return std::shared_ptr<Reduce>(
        new Reduce(reducer, cb, std::move(accumulator)));
  }

  dk_pull::types::Sink<T> Sink() override {
    auto self = this->shared_from_this();
    std::weak_ptr<Reduce<Accumulator, T>> weak_self(self);
    return [self, weak_self](const dk_pull::types::Source<T>& source) {
      using dk_pull::pull_stream::sinks::Drain;
      using dk_pull::types::Done;
      CHECK(self != nullptr);
      CHECK(self->drain == nullptr);
      self->drain = Drain<T>::Create(
          [weak_self](T&& v) {
            auto self = weak_self.lock();
            CHECK(self != nullptr);
            self->accumulator = std::move(
                self->reducer(std::move(self->accumulator), std::move(v)));
          },
          [weak_self](const Done& done) {
            auto self = weak_self.lock();
            CHECK(self != nullptr);
            self->callback(done, std::move(self->accumulator));
          });
      self->drain->Sink()(source);
    };
  }

  virtual ~Reduce() = default;

 private:
  Reduce(const Reducer& r, const Callback& cb, Accumulator&& acc)
      : reducer(r), callback(cb), accumulator(std::move(acc)) {}

  std::shared_ptr<dk_pull::pull_stream::sinks::Drain<T>> drain = nullptr;

  Reducer reducer;

  Callback callback;

  Accumulator accumulator;

  DK_DECLARE_UNCOPYABLE(Reduce);
};

}  // namespace sinks
}  // namespace pull_stream
}  // namespace dk_pull
