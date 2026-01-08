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

#ifndef LUNASERVICE_H_
#define LUNASERVICE_H_

#include <cstddef>

// Forward declarations
typedef struct LSHandle LSHandle;
typedef struct LSMessage LSMessage;

// Mock structures
struct LSHandle {
    void* data;
};

struct LSMessage {
    const char* payload;
    const char* applicationId;
    const char* senderServiceName;
    bool isSubscription;
    int refCount;
};

// Mock functions
inline void LSMessageRef(LSMessage *message) {
    if (message) message->refCount++;
}

inline void LSMessageUnref(LSMessage *message) {
    if (message && message->refCount > 0) message->refCount--;
}

inline const char* LSMessageGetPayload(LSMessage *message) {
    return message ? message->payload : nullptr;
}

inline const char* LSMessageGetApplicationID(LSMessage *message) {
    return message ? message->applicationId : nullptr;
}

inline const char* LSMessageGetSenderServiceName(LSMessage *message) {
    return message ? message->senderServiceName : nullptr;
}

inline bool LSMessageIsSubscription(LSMessage *message) {
    return message ? message->isSubscription : false;
}

#endif // LUNASERVICE_H_
