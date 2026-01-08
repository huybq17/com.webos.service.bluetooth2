// Copyright (c) 2014-2024 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef PMLOGLIB_H_
#define PMLOGLIB_H_

#include <cstdarg>

// PmLogContext type
typedef void* PmLogContext;

// PmLogErr type
typedef int PmLogErr;
#define kPmLogErr_None 0

// Global log context (mock)
extern PmLogContext logContext;

// Log level functions (all no-ops for testing)
inline void PmLogCritical(PmLogContext context, const char* msgid, int kvcount, ...) {}
inline void PmLogError(PmLogContext context, const char* msgid, int kvcount, ...) {}
inline void PmLogWarning(PmLogContext context, const char* msgid, int kvcount, ...) {}
inline void PmLogInfo(PmLogContext context, const char* msgid, int kvcount, ...) {}
inline void PmLogDebug(PmLogContext context, const char* fmt, ...) {}

// PmLogGetContext mock
inline PmLogErr PmLogGetContext(const char* contextName, PmLogContext* ctx) {
    if (ctx) *ctx = nullptr;
    return kPmLogErr_None;
}

#endif // PMLOGLIB_H_
