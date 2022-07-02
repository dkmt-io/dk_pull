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
#include <memory>

#include "dk_pull/types/end_or_error.h"
#include "dk_pull/types/source.h"

namespace dk_pull {
namespace pull_stream {
namespace sources {

template <typename T>
dk_pull::types::Source<T> MakeEmpty() {
  using dk_pull::types::Abort;
  using dk_pull::types::Done;
  using dk_pull::types::SourceCallback;
  return [](const Abort& /*abort*/, const SourceCallback<T>& cb) {
    cb(Done::TRUE, T());
  };
}

}  // namespace sources
}  // namespace pull_stream
}  // namespace dk_pull
