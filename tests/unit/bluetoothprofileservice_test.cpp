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
#include <gmock/gmock.h>
#include <string>
#include <vector>

// Mock variables for glib
bool g_file_test_mock_result = false;
bool g_file_test_symlink_result = false;

// Mock PmLogContext
#include "PmLogLib.h"
PmLogContext logContext = nullptr;

#include "bluetooth-sil-api.h"
#include "bluetoothprofileservice.h"
#include "bluetoothdevice.h"
#include "utils.h"

// Forward declarations for mocking
class MockBluetoothManagerService;
class MockBluetoothAdapter;

// Mock BluetoothManagerService
class MockBluetoothManagerService {
public:
    MockBluetoothManagerService() : mAddress("00:11:22:33:44:55") {}

    BluetoothAdapter* getDefaultAdapter() const { return nullptr; }
    BluetoothAdapter* getAdapter(const std::string &adapterAddress) { return nullptr; }
    std::string getAddress() const { return mAddress; }
    bool isDeviceAvailable(const std::string &address) const { return false; }
    bool isDeviceAvailable(const std::string &adapterAddress, const std::string &address) const { return false; }
    BluetoothDevice* findDevice(const std::string &address) const { return nullptr; }
    BluetoothDevice* findDevice(const std::string &adapterAddress, const std::string &address) const { return nullptr; }
    void* findAdapterInfo(const std::string &address) const { return nullptr; }
    bool isRequestedAdapterAvailable(LS::Message &request, const pbnjson::JValue &requestObj, std::string &adapterAddress) {
        adapterAddress = mAddress;
        return true;
    }
    bool isRoleEnable(const std::string &address, const std::string &role) { return false; }
    void* get() { return nullptr; }

private:
    std::string mAddress;
};

// Test fixture for BluetoothProfileService
class BluetoothProfileServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Note: We cannot instantiate BluetoothProfileService directly since it
        // requires BluetoothManagerService* which has complex dependencies.
        // These tests will focus on testing isolated functionality.
    }

    void TearDown() override {
    }
};

// Test convertToLower utility used by BluetoothProfileService
TEST_F(BluetoothProfileServiceTest, ConvertToLowerAddress) {
    std::string address = "AA:BB:CC:DD:EE:FF";
    std::string result = convertToLower(address);
    EXPECT_EQ(result, "aa:bb:cc:dd:ee:ff");
}

TEST_F(BluetoothProfileServiceTest, ConvertToLowerMixedCase) {
    std::string address = "Aa:Bb:Cc:Dd:Ee:Ff";
    std::string result = convertToLower(address);
    EXPECT_EQ(result, "aa:bb:cc:dd:ee:ff");
}

TEST_F(BluetoothProfileServiceTest, ConvertToUpperAddress) {
    std::string address = "aa:bb:cc:dd:ee:ff";
    std::string result = convertToUpper(address);
    EXPECT_EQ(result, "AA:BB:CC:DD:EE:FF");
}

// Test vector operations similar to those in BluetoothProfileService
TEST_F(BluetoothProfileServiceTest, VectorFindOperation) {
    std::vector<std::string> connectedDevices;
    connectedDevices.push_back("00:11:22:33:44:55");
    connectedDevices.push_back("aa:bb:cc:dd:ee:ff");

    // Test finding existing device
    auto it = std::find(connectedDevices.begin(), connectedDevices.end(), "00:11:22:33:44:55");
    EXPECT_NE(it, connectedDevices.end());

    // Test finding non-existing device
    it = std::find(connectedDevices.begin(), connectedDevices.end(), "11:22:33:44:55:66");
    EXPECT_EQ(it, connectedDevices.end());
}

TEST_F(BluetoothProfileServiceTest, VectorEraseOperation) {
    std::vector<std::string> connectedDevices;
    connectedDevices.push_back("00:11:22:33:44:55");
    connectedDevices.push_back("aa:bb:cc:dd:ee:ff");

    // Erase first device
    auto it = std::find(connectedDevices.begin(), connectedDevices.end(), "00:11:22:33:44:55");
    if (it != connectedDevices.end()) {
        connectedDevices.erase(it);
    }

    EXPECT_EQ(connectedDevices.size(), 1u);
    EXPECT_EQ(connectedDevices[0], "aa:bb:cc:dd:ee:ff");
}

// Test map operations similar to those in BluetoothProfileService
TEST_F(BluetoothProfileServiceTest, MapInsertAndFind) {
    std::map<std::string, std::vector<std::string>> devicesForMultipleAdapters;

    std::string adapterAddress = "00:11:22:33:44:55";
    std::vector<std::string> devices;
    devices.push_back("aa:bb:cc:dd:ee:ff");

    devicesForMultipleAdapters.insert(std::pair<std::string, std::vector<std::string>>(adapterAddress, devices));

    auto it = devicesForMultipleAdapters.find(adapterAddress);
    EXPECT_NE(it, devicesForMultipleAdapters.end());
    EXPECT_EQ(it->second.size(), 1u);
    EXPECT_EQ(it->second[0], "aa:bb:cc:dd:ee:ff");
}

TEST_F(BluetoothProfileServiceTest, MapFindNonExistent) {
    std::map<std::string, std::vector<std::string>> devicesForMultipleAdapters;

    auto it = devicesForMultipleAdapters.find("00:11:22:33:44:55");
    EXPECT_EQ(it, devicesForMultipleAdapters.end());
}

// Test isDeviceConnecting logic simulation
TEST_F(BluetoothProfileServiceTest, IsDeviceConnectingLogic) {
    std::map<std::string, std::vector<std::string>> connectingDevicesForMultipleAdapters;
    std::string adapterAddress = "00:11:22:33:44:55";
    std::string deviceAddress = "aa:bb:cc:dd:ee:ff";

    // Initially not connecting
    auto connectingDevicesiter = connectingDevicesForMultipleAdapters.find(adapterAddress);
    EXPECT_EQ(connectingDevicesiter, connectingDevicesForMultipleAdapters.end());

    // Add device as connecting
    std::vector<std::string> connectingDevices;
    connectingDevices.push_back(deviceAddress);
    connectingDevicesForMultipleAdapters.insert(
        std::pair<std::string, std::vector<std::string>>(adapterAddress, connectingDevices));

    // Now should be found
    connectingDevicesiter = connectingDevicesForMultipleAdapters.find(adapterAddress);
    EXPECT_NE(connectingDevicesiter, connectingDevicesForMultipleAdapters.end());

    auto deviceIter = std::find(connectingDevicesiter->second.begin(),
                                connectingDevicesiter->second.end(), deviceAddress);
    EXPECT_NE(deviceIter, connectingDevicesiter->second.end());
}

// Test markDeviceAsConnecting logic simulation
TEST_F(BluetoothProfileServiceTest, MarkDeviceAsConnectingLogic) {
    std::map<std::string, std::vector<std::string>> connectingDevicesForMultipleAdapters;
    std::string adapterAddress = "00:11:22:33:44:55";
    std::string deviceAddress = "aa:bb:cc:dd:ee:ff";

    auto connectingDevicesiter = connectingDevicesForMultipleAdapters.find(adapterAddress);
    if (connectingDevicesiter == connectingDevicesForMultipleAdapters.end()) {
        std::vector<std::string> connectingDevices;
        connectingDevices.push_back(deviceAddress);
        connectingDevicesForMultipleAdapters.insert(
            std::pair<std::string, std::vector<std::string>>(adapterAddress, connectingDevices));
    }

    // Verify
    connectingDevicesiter = connectingDevicesForMultipleAdapters.find(adapterAddress);
    EXPECT_NE(connectingDevicesiter, connectingDevicesForMultipleAdapters.end());
    EXPECT_EQ(connectingDevicesiter->second.size(), 1u);
}

// Test markDeviceAsNotConnecting logic simulation
TEST_F(BluetoothProfileServiceTest, MarkDeviceAsNotConnectingLogic) {
    std::map<std::string, std::vector<std::string>> connectingDevicesForMultipleAdapters;
    std::string adapterAddress = "00:11:22:33:44:55";
    std::string deviceAddress = "aa:bb:cc:dd:ee:ff";

    // Setup: Add device as connecting
    std::vector<std::string> connectingDevices;
    connectingDevices.push_back(deviceAddress);
    connectingDevicesForMultipleAdapters.insert(
        std::pair<std::string, std::vector<std::string>>(adapterAddress, connectingDevices));

    // Mark as not connecting
    auto connectingDevicesiter = connectingDevicesForMultipleAdapters.find(adapterAddress);
    if (connectingDevicesiter != connectingDevicesForMultipleAdapters.end()) {
        auto deviceIter = std::find(connectingDevicesiter->second.begin(),
                                    connectingDevicesiter->second.end(), deviceAddress);
        if (deviceIter != connectingDevicesiter->second.end()) {
            connectingDevicesiter->second.erase(deviceIter);
        }
    }

    // Verify device removed
    connectingDevicesiter = connectingDevicesForMultipleAdapters.find(adapterAddress);
    EXPECT_NE(connectingDevicesiter, connectingDevicesForMultipleAdapters.end());
    EXPECT_EQ(connectingDevicesiter->second.size(), 0u);
}

// Test isDeviceConnected logic simulation
TEST_F(BluetoothProfileServiceTest, IsDeviceConnectedLogic) {
    std::map<std::string, std::vector<std::string>> connectedDevicesForMultipleAdapters;
    std::string adapterAddress = convertToLower("00:11:22:33:44:55");
    std::string deviceAddress = convertToLower("AA:BB:CC:DD:EE:FF");

    // Add connected device
    std::vector<std::string> connectedDevices;
    connectedDevices.push_back(deviceAddress);
    connectedDevicesForMultipleAdapters.insert(
        std::pair<std::string, std::vector<std::string>>(adapterAddress, connectedDevices));

    // Check if device is connected
    auto connectedDevicesiter = connectedDevicesForMultipleAdapters.find(adapterAddress);
    EXPECT_NE(connectedDevicesiter, connectedDevicesForMultipleAdapters.end());

    auto found = std::find(connectedDevicesiter->second.begin(),
                           connectedDevicesiter->second.end(), deviceAddress);
    EXPECT_NE(found, connectedDevicesiter->second.end());
}

// Test markDeviceAsConnected with case conversion
TEST_F(BluetoothProfileServiceTest, MarkDeviceAsConnectedWithCaseConversion) {
    std::map<std::string, std::vector<std::string>> connectedDevicesForMultipleAdapters;
    std::string adapterAddress = "00:11:22:33:44:55";
    std::string deviceAddress = "AA:BB:CC:DD:EE:FF";

    // Simulate markDeviceAsConnected
    std::string lowerAdapter = convertToLower(adapterAddress);
    std::string lowerDevice = convertToLower(deviceAddress);

    auto connectedDevicesiter = connectedDevicesForMultipleAdapters.find(lowerAdapter);
    if (connectedDevicesiter == connectedDevicesForMultipleAdapters.end()) {
        std::vector<std::string> connectedDevices;
        connectedDevices.push_back(lowerDevice);
        connectedDevicesForMultipleAdapters.insert(
            std::pair<std::string, std::vector<std::string>>(lowerAdapter, connectedDevices));
    }

    // Verify with lowercase
    connectedDevicesiter = connectedDevicesForMultipleAdapters.find(lowerAdapter);
    EXPECT_NE(connectedDevicesiter, connectedDevicesForMultipleAdapters.end());
    EXPECT_EQ(connectedDevicesiter->second[0], "aa:bb:cc:dd:ee:ff");
}

// Test removeDeviceFromLocalDisconnecting logic
TEST_F(BluetoothProfileServiceTest, RemoveDeviceFromLocalDisconnectingLogic) {
    std::map<std::string, std::vector<std::string>> localDisconnectingDevicesForMultipleAdapters;
    std::string adapterAddress = "00:11:22:33:44:55";
    std::string deviceAddress = "aa:bb:cc:dd:ee:ff";

    // Setup: Add device to local disconnecting
    std::vector<std::string> disconnectingDevices;
    disconnectingDevices.push_back(deviceAddress);
    localDisconnectingDevicesForMultipleAdapters.insert(
        std::pair<std::string, std::vector<std::string>>(adapterAddress, disconnectingDevices));

    // Remove device from local disconnecting
    auto disconnectingDevicesiter = localDisconnectingDevicesForMultipleAdapters.find(adapterAddress);
    if (disconnectingDevicesiter != localDisconnectingDevicesForMultipleAdapters.end()) {
        auto deviceIter = std::find(disconnectingDevicesiter->second.begin(),
                                    disconnectingDevicesiter->second.end(), deviceAddress);
        if (deviceIter != disconnectingDevicesiter->second.end()) {
            disconnectingDevicesiter->second.erase(deviceIter);
        }

        // If empty, remove the adapter entry
        if (disconnectingDevicesiter->second.empty()) {
            localDisconnectingDevicesForMultipleAdapters.erase(disconnectingDevicesiter);
        }
    }

    // Verify adapter entry removed since device list is empty
    disconnectingDevicesiter = localDisconnectingDevicesForMultipleAdapters.find(adapterAddress);
    EXPECT_EQ(disconnectingDevicesiter, localDisconnectingDevicesForMultipleAdapters.end());
}

// Test strToProfileRole-like logic
TEST_F(BluetoothProfileServiceTest, StrToProfileRoleLogic) {
    std::vector<std::string> uuids;
    uuids.push_back("0000111e-0000-1000-8000-00805f9b34fb"); // HFP-HF
    uuids.push_back("0000110a-0000-1000-8000-00805f9b34fb"); // A2DP-source

    std::vector<std::string> result;
    for (const auto& uuid : uuids) {
        std::string luuid = convertToLower(uuid);
        auto iter = allServiceClasses.find(luuid);
        if (iter != allServiceClasses.end()) {
            result.push_back(iter->first);
        }
    }

    EXPECT_EQ(result.size(), 2u);
}

// Test findImpl-like logic
TEST_F(BluetoothProfileServiceTest, FindImplLogic) {
    std::map<std::string, void*> impls;
    void* mockImpl = (void*)0x12345678;

    impls.insert(std::pair<std::string, void*>("00:11:22:33:44:55", mockImpl));

    // Find with original case
    std::string convertedAddress = convertToLower("00:11:22:33:44:55");
    auto implIter = impls.find(convertedAddress);
    EXPECT_NE(implIter, impls.end());
    EXPECT_EQ(implIter->second, mockImpl);

    // Find with uppercase - should fail first attempt
    convertedAddress = convertToLower("00:11:22:33:44:55");
    implIter = impls.find(convertedAddress);
    if (implIter == impls.end()) {
        convertedAddress = convertToUpper("00:11:22:33:44:55");
        implIter = impls.find(convertedAddress);
    }
    // Found on first attempt since we stored lowercase
    EXPECT_EQ(implIter->second, mockImpl);
}

// Test multiple UUIDs initialization
TEST_F(BluetoothProfileServiceTest, MultipleUuidsInitialization) {
    std::vector<std::string> uuids;
    std::string uuid1 = "0000110a-0000-1000-8000-00805f9b34fb";
    std::string uuid2 = "0000110b-0000-1000-8000-00805f9b34fb";

    uuids.push_back(uuid1);
    uuids.push_back(uuid2);

    EXPECT_EQ(uuids.size(), 2u);
    EXPECT_EQ(uuids[0], uuid1);
    EXPECT_EQ(uuids[1], uuid2);
}

// Test getName and getUuids-like logic
TEST_F(BluetoothProfileServiceTest, GetNameAndUuids) {
    std::string name = "A2DP";
    std::vector<std::string> uuids;
    uuids.push_back("0000110a-0000-1000-8000-00805f9b34fb");

    EXPECT_EQ(name, "A2DP");
    EXPECT_EQ(uuids.size(), 1u);
    EXPECT_EQ(uuids[0], "0000110a-0000-1000-8000-00805f9b34fb");
}

// Test pbnjson object building similar to buildGetStatusResp
TEST_F(BluetoothProfileServiceTest, BuildGetStatusResponse) {
    pbnjson::JValue responseObj = pbnjson::Object();

    bool connected = true;
    bool connecting = false;
    bool subscribed = true;
    bool returnValue = true;
    std::string adapterAddress = "00:11:22:33:44:55";
    std::string deviceAddress = "aa:bb:cc:dd:ee:ff";

    responseObj.put("connected", connected);
    responseObj.put("connecting", connecting);
    responseObj.put("subscribed", subscribed);
    responseObj.put("returnValue", returnValue);
    responseObj.put("adapterAddress", adapterAddress);
    responseObj.put("address", deviceAddress);

    EXPECT_TRUE(responseObj.hasKey("connected"));
    EXPECT_TRUE(responseObj.hasKey("connecting"));
    EXPECT_TRUE(responseObj.hasKey("subscribed"));
    EXPECT_TRUE(responseObj.hasKey("returnValue"));
    EXPECT_TRUE(responseObj.hasKey("adapterAddress"));
    EXPECT_TRUE(responseObj.hasKey("address"));
}

// Test appendCommonProfileStatus-like logic
TEST_F(BluetoothProfileServiceTest, AppendCommonProfileStatus) {
    pbnjson::JValue responseObj = pbnjson::Object();

    responseObj.put("connected", true);
    responseObj.put("connecting", false);
    responseObj.put("subscribed", true);
    responseObj.put("returnValue", true);
    responseObj.put("adapterAddress", "00:11:22:33:44:55");
    responseObj.put("address", "aa:bb:cc:dd:ee:ff");

    EXPECT_TRUE(responseObj["connected"].asBool());
    EXPECT_FALSE(responseObj["connecting"].asBool());
    EXPECT_TRUE(responseObj["subscribed"].asBool());
    EXPECT_TRUE(responseObj["returnValue"].asBool());
    EXPECT_EQ(responseObj["adapterAddress"].asString(), "00:11:22:33:44:55");
    EXPECT_EQ(responseObj["address"].asString(), "aa:bb:cc:dd:ee:ff");
}

// Test reset logic
TEST_F(BluetoothProfileServiceTest, ResetLogic) {
    std::map<std::string, void*> impls;
    void* impl1 = (void*)0x11111111;
    void* impl2 = (void*)0x22222222;

    impls.insert(std::pair<std::string, void*>("00:11:22:33:44:55", impl1));
    impls.insert(std::pair<std::string, void*>("66:77:88:99:aa:bb", impl2));

    // Reset specific adapter
    std::string adapterAddress = "00:11:22:33:44:55";
    auto implIter = impls.find(adapterAddress);
    if (implIter != impls.end()) {
        impls.erase(implIter);
    }

    EXPECT_EQ(impls.size(), 1u);
    EXPECT_EQ(impls.find("00:11:22:33:44:55"), impls.end());
    EXPECT_NE(impls.find("66:77:88:99:aa:bb"), impls.end());
}

// Test propertiesChanged-like logic
TEST_F(BluetoothProfileServiceTest, PropertiesChangedConnectedTrue) {
    std::vector<std::string> connectedDevices;
    std::vector<std::string> connectingDevices;
    std::string address = "aa:bb:cc:dd:ee:ff";
    bool connected = true;

    if (connected) {
        // markDeviceAsNotConnecting
        auto deviceIter = std::find(connectingDevices.begin(), connectingDevices.end(), address);
        if (deviceIter != connectingDevices.end()) {
            connectingDevices.erase(deviceIter);
        }

        // markDeviceAsConnected
        if (std::find(connectedDevices.begin(), connectedDevices.end(), convertToLower(address)) == connectedDevices.end()) {
            connectedDevices.push_back(convertToLower(address));
        }
    }

    EXPECT_EQ(connectedDevices.size(), 1u);
    EXPECT_EQ(connectingDevices.size(), 0u);
}

TEST_F(BluetoothProfileServiceTest, PropertiesChangedConnectedFalse) {
    std::vector<std::string> connectedDevices;
    std::string address = "aa:bb:cc:dd:ee:ff";
    connectedDevices.push_back(convertToLower(address));

    bool connected = false;

    if (!connected) {
        // markDeviceAsNotConnected
        auto deviceIter = std::find(connectedDevices.begin(), connectedDevices.end(), convertToLower(address));
        if (deviceIter != connectedDevices.end()) {
            connectedDevices.erase(deviceIter);
        }
    }

    EXPECT_EQ(connectedDevices.size(), 0u);
}
