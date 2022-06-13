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

#include "dk_pull/pull_stream/sources/count.h"

#include "dk_pull/types/end_or_error.h"

namespace dk_pull {
namespace pull_stream {
namespace sources {

Count::Count(uint64_t m) : maximum(m) {}

std::shared_ptr<Count> Count::Create(uint64_t maximum) {
  return std::shared_ptr<Count>(new Count(maximum));
}

dk_pull::types::Source<uint64_t> Count::Source() {
  using dk_pull::types::Abort;
  using dk_pull::types::Done;
  using dk_pull::types::SourceCallback;
  auto self = this->shared_from_this();
  return [self](const Abort& abort, SourceCallback<uint64_t> cb) {
    if (cb == nullptr) {
      cb = [](auto /*unused*/, auto /*unused*/) {};
    }
    if (abort && (cb != nullptr)) {
      cb(abort, 0);
      return;
    }
    if (self->i > self->maximum) {
      cb(Done::TRUE, 0);
      return;
    }
    uint64_t v = self->i;
    self->i++;
    cb(Done::FALSE, std::move(v));  // NOLINT
  };
}

}  // namespace sources
}  // namespace pull_stream
}  // namespace dk_pull
