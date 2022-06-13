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

#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "dk_pull/common/finally.h"
#include "dk_pull/common/uncopyable.h"
#include "dk_pull/types/end_or_error.h"
#include "dk_pull/types/sink.h"
#include "dk_pull/types/source.h"
#include "dk_pull/types/through.h"
#include "glog/logging.h"

namespace dk_pull {
namespace pull_stream {

template <typename T>
class ProtocolChecker final
    : public dk_pull::types::ThroughContext<T, T>,
      public std::enable_shared_from_this<ProtocolChecker<T>> {
 public:
  using Abort = dk_pull::types::Abort;
  using Done = dk_pull::types::Done;

  struct Options {
    bool forbid_extra_requests = false;
    bool enforce_stream_termination = false;
    bool notify_eagerly = true;
  };

  static std::shared_ptr<ProtocolChecker> Create(const Options& options = {}) {
    return std::shared_ptr<ProtocolChecker>(new ProtocolChecker(options));
  }

  dk_pull::types::Through<T, T> Through() override {
    using dk_pull::types::Abort;
    using dk_pull::types::Done;
    using dk_pull::types::Source;
    using dk_pull::types::SourceCallback;
    auto self = this->shared_from_this();
    return [self](const Source<T>& source) -> Source<T> {
      return [self, source](const Abort& abort, const SourceCallback<T>& cb) {
        using dk_pull::common::Finally;
        if (self->aborted || self->done) {
          if (!abort) {
            self->notifyError(
                "Invariant 1 violated: value requested after termination");
          }
          if (self->forbidExtraRequests) {
            if (self->aborted) {
              self->notifyError(
                  "Invariant 6 violated: request made after the stream was "
                  "aborted");
            }
            if (self->done) {
              self->notifyError(
                  "Invariant 6 violated: request made after the stream has "
                  "terminated");
            }
          }
        }
        if (!self->aborted) {
          self->aborted = abort;
        }
        std::shared_ptr<uint64_t> i(new uint64_t(self->i));
        self->i++;
        if (cb == nullptr) {
          self->skipped[*i] = true;
          source(self->aborted, noop);
          return;
        }
        std::shared_ptr<uint64_t> counter(new uint64_t(0));
        source(abort, [self, i, cb, counter](const Done& done, T&& v) {
          if ((!done) && (self->aborted || self->done)) {
            std::stringstream buffer;
            buffer << "Invariant 5 violated: callback " << *i
                   << " returned a value after termination";
            self->notifyError(buffer.str());
          }
          (*counter)++;
          if ((*counter) > 1) {
            std::stringstream buffer;
            buffer << "Invariant 3 violated: callback " << *i << " invoked "
                   << *counter << " times";
            self->notifyError(buffer.str());
          }
          if (*i < self->latest) {
            std::stringstream buffer;
            buffer << "Invariant 4 violated: callback " << *i
                   << " invoked after callback " << self->latest;
            self->notifyError(buffer.str());
          } else if (*i > self->latest + 1) {
            std::stringstream buffer;
            buffer << "Invariant 4 violated: callback " << *i
                   << " invoked before callback " << self->latest + 1;
            self->notifyError(buffer.str());
          } else {
            self->latest = *i;
          }
          if (!self->done) {
            self->done = done;
          }
          cb(self->done, std::move(v));
        });
      };
    };
  }

  auto Terminate() {
    if (i > latest + 1) {
      for (auto k = latest + 1; k < i; k++) {
        if (skipped.count(k) == 0 || skipped[k]) {
          std::stringstream buffer;
          buffer << "Invariant 2 violated: callback " << k
                 << " was never invoked";
          notifyError(buffer.str());
        }
      }
    }
    if (enforceStreamTermination && !done && !aborted) {
      std::stringstream buffer;
      buffer << "Invariant 7 violated: stream was never terminated";
      notifyError(buffer.str());
    }
    return errors;
  }

  virtual ~ProtocolChecker() = default;

 private:
  static void noop(const dk_pull::types::Done& done, T&& v) {}

  explicit ProtocolChecker(const Options& options)
      : forbidExtraRequests(options.forbid_extra_requests),
        enforceStreamTermination(options.enforce_stream_termination),
        notifyEagerly(options.notify_eagerly) {}

  void notifyError(const std::string& err) {
    if (notifyEagerly) {
      throw std::logic_error(err);
    }
    errors.push_back(err);
  }

  bool forbidExtraRequests = false;

  bool enforceStreamTermination = false;

  bool notifyEagerly = true;

  uint64_t i = 1;

  uint64_t latest = 0;

  std::map<uint64_t, bool> skipped;

  Abort aborted = Abort::FALSE;

  Done done = Done::FALSE;

  std::vector<std::string> errors;

  DK_DECLARE_UNCOPYABLE(ProtocolChecker);
};

}  // namespace pull_stream
}  // namespace dk_pull
