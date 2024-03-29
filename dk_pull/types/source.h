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
#include "dk_pull/types/end_or_error.h"

namespace dk_pull {
namespace types {  //

template <typename T>
using SourceCallback = std::function<void(const Done&, T&&)>;

template <typename T>
using Source = std::function<void(const Abort&, const SourceCallback<T>&)>;

template <typename T>
class SourceContext {
 public:
  virtual dk_pull::types::Source<T> Source() = 0;

  virtual ~SourceContext() = default;

 protected:
  SourceContext() = default;

 private:
  DK_DECLARE_UNCOPYABLE(SourceContext);
};

}  // namespace types
}  // namespace dk_pull
