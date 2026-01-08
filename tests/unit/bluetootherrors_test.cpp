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

#include <gtest/gtest.h>
#include <string>

#include "bluetootherrors.h"

class BluetoothErrorsTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test appendErrorResponse function
TEST_F(BluetoothErrorsTest, AppendErrorResponseNone) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_NONE);

    EXPECT_TRUE(obj.hasKey("returnValue"));
    EXPECT_TRUE(obj.hasKey("errorCode"));
    EXPECT_TRUE(obj.hasKey("errorText"));
    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_NONE));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseFail) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_FAIL);

    EXPECT_TRUE(obj.hasKey("returnValue"));
    EXPECT_TRUE(obj.hasKey("errorCode"));
    EXPECT_TRUE(obj.hasKey("errorText"));
    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_FAIL));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseNotReady) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_NOT_READY);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_NOT_READY));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseNoMem) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_NOMEM);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_NOMEM));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseBusy) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_BUSY);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_BUSY));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseUnsupported) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_UNSUPPORTED);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_UNSUPPORTED));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseParamInvalid) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_PARAM_INVALID);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_PARAM_INVALID));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseUnhandled) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_UNHANDLED);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_UNHANDLED));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseUnknownDeviceAddr) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_UNKNOWN_DEVICE_ADDR);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_UNKNOWN_DEVICE_ADDR));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseAuthCanceled) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_AUTHENTICATION_CANCELED);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_AUTHENTICATION_CANCELED));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseAuthFailed) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_AUTHENTICATION_FAILED);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_AUTHENTICATION_FAILED));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseAuthRejected) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_AUTHENTICATION_REJECTED);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_AUTHENTICATION_REJECTED));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseAuthTimeout) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_AUTHENTICATION_TIMEOUT);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_AUTHENTICATION_TIMEOUT));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseDeviceAlreadyPaired) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_DEVICE_ALREADY_PAIRED);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_DEVICE_ALREADY_PAIRED));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseDeviceNotPaired) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_DEVICE_NOT_PAIRED);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_DEVICE_NOT_PAIRED));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseDeviceAlreadyConnected) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_DEVICE_ALREADY_CONNECTED);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_DEVICE_ALREADY_CONNECTED));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseDeviceNotConnected) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_DEVICE_NOT_CONNECTED);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_DEVICE_NOT_CONNECTED));
}

// Test retrieveErrorText function
TEST_F(BluetoothErrorsTest, RetrieveErrorTextAdapterNotAvailable) {
    std::string errorText = retrieveErrorText(BT_ERR_ADAPTER_NOT_AVAILABLE);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Bluetooth adapter is not available");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextMsgParseFail) {
    std::string errorText = retrieveErrorText(BT_ERR_MSG_PARSE_FAIL);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Failed to parse incoming message");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextMethodNotSubscribed) {
    std::string errorText = retrieveErrorText(BT_ERR_MTHD_NOT_SUBSCRIBED);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Method needs to be subscribed");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextAllowOneSubscribe) {
    std::string errorText = retrieveErrorText(BT_ERR_ALLOW_ONE_SUBSCRIBE);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Only one subscription allowed");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextAddrParamMissing) {
    std::string errorText = retrieveErrorText(BT_ERR_ADDR_PARAM_MISSING);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Required 'address' parameter is not supplied");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextDeviceNotAvail) {
    std::string errorText = retrieveErrorText(BT_ERR_DEVICE_NOT_AVAIL);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Device with supplied address is not available");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextPairingCanceled) {
    std::string errorText = retrieveErrorText(BT_ERR_PAIRING_CANCELED);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Pairing canceled by user");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextNoPairing) {
    std::string errorText = retrieveErrorText(BT_ERR_NO_PAIRING);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "There is no pairing in progress");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextProfileUnavail) {
    std::string errorText = retrieveErrorText(BT_ERR_PROFILE_UNAVAIL);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Profile backend is not available");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextDevNotPaired) {
    std::string errorText = retrieveErrorText(BT_ERR_DEV_NOT_PAIRED);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Device is not paired");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextProfileConnectFail) {
    std::string errorText = retrieveErrorText(BT_ERR_PROFILE_CONNECT_FAIL);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Failed to connect with remote device");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextProfileConnected) {
    std::string errorText = retrieveErrorText(BT_ERR_PROFILE_CONNECTED);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Already connected");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextProfileNotConnected) {
    std::string errorText = retrieveErrorText(BT_ERR_PROFILE_NOT_CONNECTED);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Device is not connected to profile");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextBadJson) {
    std::string errorText = retrieveErrorText(BT_ERR_BAD_JSON);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Invalid JSON input");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextSchemaValidationFail) {
    std::string errorText = retrieveErrorText(BT_ERR_SCHEMA_VALIDATION_FAIL);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "The JSON input does not match the expected schema");
}

// Test retrieveErrorCodeText function
TEST_F(BluetoothErrorsTest, RetrieveErrorCodeTextNone) {
    std::string errorText = retrieveErrorCodeText(BLUETOOTH_ERROR_NONE);
    EXPECT_EQ(errorText, "No error");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorCodeTextFail) {
    std::string errorText = retrieveErrorCodeText(BLUETOOTH_ERROR_FAIL);
    EXPECT_EQ(errorText, "The operation failed for an unspecified or generic reason");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorCodeTextNotReady) {
    std::string errorText = retrieveErrorCodeText(BLUETOOTH_ERROR_NOT_READY);
    EXPECT_EQ(errorText, "The device is not ready to perform the requested operation");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorCodeTextNoMem) {
    std::string errorText = retrieveErrorCodeText(BLUETOOTH_ERROR_NOMEM);
    EXPECT_EQ(errorText, "The SIL failed to allocated memory");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorCodeTextBusy) {
    std::string errorText = retrieveErrorCodeText(BLUETOOTH_ERROR_BUSY);
    EXPECT_EQ(errorText, "The operation can not be performed at this time");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorCodeTextUnsupported) {
    std::string errorText = retrieveErrorCodeText(BLUETOOTH_ERROR_UNSUPPORTED);
    EXPECT_EQ(errorText, "The requested operation is not supported by the stack or device");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorCodeTextParamInvalid) {
    std::string errorText = retrieveErrorCodeText(BLUETOOTH_ERROR_PARAM_INVALID);
    EXPECT_EQ(errorText, "An invalid value was passed for one of the parameters");
}

// Test more error codes for comprehensive coverage
TEST_F(BluetoothErrorsTest, RetrieveErrorTextA2dpStartStreamingFailed) {
    std::string errorText = retrieveErrorText(BT_ERR_A2DP_START_STREAMING_FAILED);
    EXPECT_FALSE(errorText.empty());
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextGattServiceNameParamMissing) {
    std::string errorText = retrieveErrorText(BT_ERR_GATT_SERVICE_NAME_PARAM_MISSING);
    EXPECT_FALSE(errorText.empty());
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextHfpOpenScoFailed) {
    std::string errorText = retrieveErrorText(BT_ERR_HFP_OPEN_SCO_FAILED);
    EXPECT_FALSE(errorText.empty());
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextSppUuidParamMissing) {
    std::string errorText = retrieveErrorText(BT_ERR_SPP_UUID_PARAM_MISSING);
    EXPECT_FALSE(errorText.empty());
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextMeshNetKeyIndexParamMissing) {
    std::string errorText = retrieveErrorText(BT_ERR_MESH_NET_KEY_INDEX_PARAM_MISSING);
    EXPECT_FALSE(errorText.empty());
}

// Test mesh-related error codes
TEST_F(BluetoothErrorsTest, AppendErrorResponseMeshNetKeyDoesNotExist) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_MESH_NET_KEY_INDEX_DOES_NOT_EXIST);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_MESH_NET_KEY_INDEX_DOES_NOT_EXIST));
}

TEST_F(BluetoothErrorsTest, AppendErrorResponseMeshAppKeyAlreadyExists) {
    pbnjson::JValue obj = pbnjson::Object();
    appendErrorResponse(obj, BLUETOOTH_ERROR_MESH_APP_KEY_INDEX_ALREADY_EXISTS);

    EXPECT_FALSE(obj["returnValue"].asBool());
    EXPECT_EQ(obj["errorCode"].asNumber<int>(), static_cast<int>(BLUETOOTH_ERROR_MESH_APP_KEY_INDEX_ALREADY_EXISTS));
}

// Additional error code tests
TEST_F(BluetoothErrorsTest, RetrieveErrorTextAdapterTurnedOff) {
    std::string errorText = retrieveErrorText(BT_ERR_ADAPTER_TURNED_OFF);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Adapter is turned off");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextInvalidAdapterAddress) {
    std::string errorText = retrieveErrorText(BT_ERR_INVALID_ADAPTER_ADDRESS);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Adapter address is not valid");
}

TEST_F(BluetoothErrorsTest, RetrieveErrorTextPairingInProgress) {
    std::string errorText = retrieveErrorText(BT_ERR_PAIRING_IN_PROG);
    EXPECT_FALSE(errorText.empty());
    EXPECT_EQ(errorText, "Pairing already in progress");
}
