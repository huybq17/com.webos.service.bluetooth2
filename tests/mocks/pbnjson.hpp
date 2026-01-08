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

#ifndef PBNJSON_HPP_
#define PBNJSON_HPP_

#include <string>
#include <map>
#include <vector>
#include <memory>

namespace pbnjson {

class JValue {
public:
    JValue() : mType(Type::Null) {}

    static JValue Object() {
        JValue v;
        v.mType = Type::Object;
        return v;
    }

    static JValue Array() {
        JValue v;
        v.mType = Type::Array;
        return v;
    }

    void put(const std::string &key, const JValue &value) {
        mObjectData[key] = std::make_shared<JValue>(value);
    }

    void put(const std::string &key, const std::string &value) {
        JValue v;
        v.mType = Type::String;
        v.mStringValue = value;
        mObjectData[key] = std::make_shared<JValue>(v);
    }

    void put(const std::string &key, const char *value) {
        put(key, std::string(value));
    }

    void put(const std::string &key, int value) {
        JValue v;
        v.mType = Type::Number;
        v.mIntValue = value;
        mObjectData[key] = std::make_shared<JValue>(v);
    }

    void put(const std::string &key, bool value) {
        JValue v;
        v.mType = Type::Boolean;
        v.mBoolValue = value;
        mObjectData[key] = std::make_shared<JValue>(v);
    }

    void append(const JValue &value) {
        mArrayData.push_back(std::make_shared<JValue>(value));
    }

    void append(int value) {
        JValue v;
        v.mType = Type::Number;
        v.mIntValue = value;
        mArrayData.push_back(std::make_shared<JValue>(v));
    }

    void append(uint8_t value) {
        append(static_cast<int>(value));
    }

    bool hasKey(const std::string &key) const {
        return mObjectData.find(key) != mObjectData.end();
    }

    JValue operator[](const std::string &key) const {
        auto it = mObjectData.find(key);
        if (it != mObjectData.end()) {
            return *it->second;
        }
        return JValue();
    }

    JValue operator[](int index) const {
        if (index >= 0 && index < static_cast<int>(mArrayData.size())) {
            return *mArrayData[index];
        }
        return JValue();
    }

    std::string asString() const {
        return mStringValue;
    }

    bool asBool() const {
        return mBoolValue;
    }

    template<typename T>
    T asNumber() const {
        return static_cast<T>(mIntValue);
    }

    int arraySize() const {
        return static_cast<int>(mArrayData.size());
    }

    bool isNull() const {
        return mType == Type::Null;
    }

    bool isObject() const {
        return mType == Type::Object;
    }

    bool isArray() const {
        return mType == Type::Array;
    }

    bool isString() const {
        return mType == Type::String;
    }

    bool isNumber() const {
        return mType == Type::Number;
    }

    bool isBoolean() const {
        return mType == Type::Boolean;
    }

private:
    enum class Type {
        Null,
        Object,
        Array,
        String,
        Number,
        Boolean
    };

    Type mType;
    std::string mStringValue;
    int mIntValue = 0;
    bool mBoolValue = false;
    std::map<std::string, std::shared_ptr<JValue>> mObjectData;
    std::vector<std::shared_ptr<JValue>> mArrayData;
};

} // namespace pbnjson

// Free functions that match the pbnjson API
namespace pbnjson {
    inline JValue Object() { return JValue::Object(); }
    inline JValue Array() { return JValue::Array(); }
}

#endif // PBNJSON_HPP_
