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
#include <utility>
#include <vector>

#include "dk_pull/pull_stream/sinks/reduce.h"
#include "dk_pull/types/sink.h"

namespace dk_pull {
namespace pull_stream {
namespace sinks {

template <typename T>
dk_pull::types::Sink<T> MakeCollect(
    const typename Reduce<std::vector<T>, T>::Callback& cb) {
  auto reduce = Reduce<std::vector<T>, T>::Create(
      [](std::vector<T>&& vector, T&& v) {
        vector.push_back(std::move(v));
        return std::move(vector);
      },
      cb);
  return [reduce](const dk_pull::types::Source<T>& source) {
    return reduce->Sink()(source);
  };
}

}  // namespace sinks
}  // namespace pull_stream
}  // namespace dk_pull
