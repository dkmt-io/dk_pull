/*******************************************************************************
 * Copyright (C) 2022 Hans Liu
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

#include "dk_pull/looper/looper.h"

#include "glog/logging.h"

namespace dk_pull {
namespace looper {

std::shared_ptr<Looper> Looper::Create(const LooperFunction& fn) {
  return std::shared_ptr<Looper>(new Looper(fn));
}

Looper::Looper(const LooperFunction& fn) : looperFunction(fn) {
  CHECK_NOTNULL(looperFunction);
}

void Looper::Start() { looperFunction(makeNext()); }

std::function<void()> Looper::makeNext() {
  auto self = shared_from_this();
  return [self]() {
    self->nextCalled = true;
    if (!self->looperFunctionReturned) {
      return;
    }
    self->looperFunctionReturned = false;
    while (self->nextCalled) {
      self->nextCalled = false;
      self->looperFunction(self->makeNext());
    }
    self->looperFunctionReturned = true;
  };
}

}  // namespace looper
}  // namespace dk_pull
