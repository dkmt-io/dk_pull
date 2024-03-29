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

#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "dk_pull/common/uncopyable.h"
#include "dk_pull/types/source.h"
#include "glog/logging.h"

namespace dk_pull {
namespace pull_stream {
namespace sources {

template <typename T>
class Values final : public dk_pull::types::SourceContext<T>,
                     public std::enable_shared_from_this<Values<T>> {
 public:
  static std::shared_ptr<Values> Create(std::vector<T>&& values) {
    auto self = std::shared_ptr<Values>(new Values(std::move(values)));
    return self;
  }

  dk_pull::types::Source<T> Source() override {
    auto self = this->shared_from_this();
    return [self](const dk_pull::types::Abort& abort,
                  dk_pull::types::SourceCallback<T> cb) {
      using dk_pull::types::Done;
      if (cb == nullptr) {
        cb = [](auto /*unused*/, auto /*unused*/) {};
      }
      if (abort) {
        cb(abort, T());
        return;
      }
      if (self->values.empty()) {
        cb(Done::TRUE, T());
        return;
      }
      T t = std::move(self->values.front());
      self->values.pop_front();
      cb(Done::FALSE, std::move(t));
    };
  }

  virtual ~Values() = default;

 private:
  explicit Values(std::vector<T>&& v) : values(v.begin(), v.end()) {}

  std::list<T> values;

  DK_DECLARE_UNCOPYABLE(Values);
};

}  // namespace sources
}  // namespace pull_stream
}  // namespace dk_pull
