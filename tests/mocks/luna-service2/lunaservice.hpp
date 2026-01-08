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

#ifndef LUNASERVICE_HPP_
#define LUNASERVICE_HPP_

#include "lunaservice.h"
#include <string>
#include <functional>

namespace LS {

class Error : public std::exception {
public:
    Error() {}
    Error(const char* msg) : mMessage(msg) {}
    const char* what() const noexcept override { return mMessage.c_str(); }
private:
    std::string mMessage;
};

class Message {
public:
    Message() : mMessage(nullptr) {}
    Message(LSMessage *msg) : mMessage(msg) {}

    LSMessage* get() const { return mMessage; }

    const char* getPayload() const {
        return mMessage ? mMessage->payload : "";
    }

    bool isSubscription() const {
        return mMessage ? mMessage->isSubscription : false;
    }

    void respond(const char* payload) const {
        // Mock implementation - stores response
        if (mMessage) {
            mMessage->response = payload;
        }
    }

private:
    LSMessage* mMessage;
};

class SubscriptionPoint {
public:
    SubscriptionPoint() {}

    void setServiceHandle(void* handle) {
        mHandle = handle;
    }

    void subscribe(const Message &message) {
        // Mock implementation
    }

    bool post(const char* payload) {
        return true;
    }

private:
    void* mHandle = nullptr;
};

class ServerStatus {
public:
    ServerStatus() = default;
};

} // namespace LS

#endif // LUNASERVICE_HPP_
