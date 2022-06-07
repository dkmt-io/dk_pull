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

#pragma once

#include <string>

namespace dk_pull {
namespace types {

class EndOrError final {
 public:
  explicit EndOrError(bool end = false);

  explicit EndOrError(const std::string& message);

  inline bool IsEnd() const;

  inline bool IsError() const;

  inline const std::string& GetErrorMessage() const;

 private:
  bool isEnd = false;
  bool isError = false;
  std::string errorMessage;
};

using Done = EndOrError;

using Abort = EndOrError;

}  // namespace types
}  // namespace dk_pull
