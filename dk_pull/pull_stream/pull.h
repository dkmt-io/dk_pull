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

#include "dk_pull/types/duplex.h"
#include "dk_pull/types/end_or_error.h"
#include "dk_pull/types/sink.h"
#include "dk_pull/types/source.h"
#include "dk_pull/types/through.h"

namespace dk_pull {
namespace pull_stream {

template <typename T>
void Pull(const dk_pull::types::Source<T>& source,
          const dk_pull::types::Sink<T>& sink) {
  return sink(source);
}

template <typename T1, typename T2>
dk_pull::types::Source<T2> Pull(
    const dk_pull::types::Source<T1>& source,
    const dk_pull::types::Through<T1, T2>& through) {
  return through(source);
}

template <typename T1, typename T2>
void Pull(const dk_pull::types::Source<T1>& source,
          const dk_pull::types::Through<T1, T2>& through,
          const dk_pull::types::Sink<T2>& sink) {
  return Pull(Pull(source, through), sink);
}

template <typename T, typename U>
void Link(const dk_pull::types::Duplex<T, U>& lhs,
          const dk_pull::types::Duplex<U, T>& rhs) {
  Pull(lhs->source(), rhs->sink());
  Pull(rhs->source(), lhs->sink());
}

}  // namespace pull_stream
}  // namespace dk_pull
