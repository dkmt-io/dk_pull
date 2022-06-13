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

#include "dk_pull/common/uncopyable.h"
#include "dk_pull/types/source.h"

namespace dk_pull {
namespace pull_stream {
namespace sources {

class Count final : public dk_pull::types::SourceContext<uint64_t>,
                    public std::enable_shared_from_this<Count> {
 public:
  static std::shared_ptr<Count> Create(uint64_t max);

  dk_pull::types::Source<uint64_t> Source() override;

  virtual ~Count() = default;

 private:
  explicit Count(uint64_t m);

  uint64_t i = 0;

  const uint64_t maximum;

  DK_DECLARE_UNCOPYABLE(Count);
};

}  // namespace sources
}  // namespace pull_stream
}  // namespace dk_pull
