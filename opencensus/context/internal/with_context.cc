// Copyright 2018, OpenCensus Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "opencensus/context/with_context.h"

#include <utility>

#include "opencensus/context/context.h"

namespace opencensus {
namespace context {

WithContext::WithContext(const Context& ctx) : swapped_context_(ctx) {
  using std::swap;
  swap(*Context::InternalMutableCurrent(), swapped_context_);
}

WithContext::WithContext(Context&& ctx) : swapped_context_(std::move(ctx)) {
  using std::swap;
  swap(*Context::InternalMutableCurrent(), swapped_context_);
}

WithContext::~WithContext() {
  using std::swap;
  swap(*Context::InternalMutableCurrent(), swapped_context_);
}

}  // namespace context
}  // namespace opencensus
