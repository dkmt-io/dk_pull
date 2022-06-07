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

#include "dk_pull/types/end_or_error.h"

#include <string>

namespace dk_pull {
namespace types {

EndOrError::EndOrError(bool end) : isEnd(end), isError(false) {}

EndOrError::EndOrError(const std::string& message)
    : isEnd(false), isError(true), errorMessage(message) {}

bool EndOrError::IsEnd() const { return isEnd; }

bool EndOrError::IsError() const { return isError; }

const std::string& EndOrError::GetErrorMessage() const { return errorMessage; }

}  // namespace types
}  // namespace dk_pull
