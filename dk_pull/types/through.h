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

#include "dk_pull/types/source.h"

namespace dk_pull {
namespace types {  //

template <typename In, typename Out>
using Through = std::function<Source<Out>(const Source<In>&)>;

template <typename In, typename Out>
class ThroughContext {
 public:
  virtual dk_pull::types::Through<In, Out> Through() = 0;

  virtual ~ThroughContext() = default;

 protected:
  ThroughContext() = default;

 private:
  DK_DECLARE_UNCOPYABLE(ThroughContext);
};

}  // namespace types
}  // namespace dk_pull
