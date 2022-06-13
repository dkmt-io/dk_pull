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

#include "dk_pull/types/sink.h"
#include "dk_pull/types/source.h"

namespace dk_pull {
namespace types {

template <typename SourceType, typename SinkType>
class Duplex : public SourceContext<SourceType>, public SinkContext<SinkType> {
 public:
  virtual ~Duplex() = default;

 protected:
  Duplex() = default;

 private:
  DK_DECLARE_UNCOPYABLE(Duplex);
};

template <typename SourceType, typename SinkType>
auto MakeDuplex(const Source<SourceType>& source, const Sink<SinkType>& sink) {
  class Impl : public Duplex<SourceType, SinkType> {
   public:
    Impl(const Impl&) noexcept = default;
    Impl(Impl&&) noexcept = default;
    Impl& operator=(const Impl&) noexcept = default;
    Impl& operator=(Impl&&) noexcept = default;
    virtual ~Impl() = default;

    Impl(const Source<SourceType>& src, const Sink<SinkType>& snk) noexcept
        : source(src), sink(snk) {}

    auto Source() override { return source; }

    auto Sink() override { return sink; }

   private:
    dk_pull::types::Source<SourceType> source;
    dk_pull::types::Sink<SinkType> sink;
  };
  return Impl(source, sink);
}

}  // namespace types
}  // namespace dk_pull
