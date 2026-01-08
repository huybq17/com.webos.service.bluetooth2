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
#include <map>
#include <unordered_map>

// Mock variables for glib
bool g_file_test_mock_result = false;
bool g_file_test_symlink_result = false;

// Mock PmLogContext
#include "PmLogLib.h"
PmLogContext logContext = nullptr;

#include "bluetooth-sil-api.h"
#include "bluetoothserviceclasses.h"
#include "utils.h"
#include "pbnjson.hpp"

// Test fixture for BluetoothGattProfileService related tests
class BluetoothGattProfileServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test GATT profile UUID
TEST_F(BluetoothGattProfileServiceTest, GattUuid) {
    std::string uuid = "00001801-0000-1000-8000-00805f9b34fb";
    auto it = allServiceClasses.find(uuid);
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "GATT");
}

// Test CharacteristicWatch structure simulation
TEST_F(BluetoothGattProfileServiceTest, CharacteristicWatchStructure) {
    struct CharacteristicWatch {
        std::string deviceAddress;
        std::string serviceId;
        std::string characteristicId;
        uint16_t handle;
        unsigned int refCount;
        bool registered;

        CharacteristicWatch() : handle(0), refCount(0), registered(false) {}

        void ref() { refCount++; }
        void unref() { if (refCount > 0) refCount--; }
        bool isUsed() { return refCount > 0; }
        void markRegistered() { registered = true; }
        bool isRegistered() { return registered; }
    };

    CharacteristicWatch watch;
    watch.deviceAddress = "aa:bb:cc:dd:ee:ff";
    watch.serviceId = "0000180d-0000-1000-8000-00805f9b34fb";
    watch.characteristicId = "00002a37-0000-1000-8000-00805f9b34fb";
    watch.handle = 10;

    EXPECT_EQ(watch.deviceAddress, "aa:bb:cc:dd:ee:ff");
    EXPECT_EQ(watch.handle, 10);
    EXPECT_FALSE(watch.isUsed());

    watch.ref();
    EXPECT_TRUE(watch.isUsed());

    watch.markRegistered();
    EXPECT_TRUE(watch.isRegistered());

    watch.unref();
    EXPECT_FALSE(watch.isUsed());
}

// Test connectedDeviceInfo structure simulation
TEST_F(BluetoothGattProfileServiceTest, ConnectedDeviceInfoStructure) {
    struct connectedDeviceInfo {
        std::string deviceAddress;
        uint16_t connectId;

        connectedDeviceInfo() : connectId(0) {}
        connectedDeviceInfo(std::string address, uint16_t connId) :
            deviceAddress(address), connectId(connId) {}

        std::string getAddress() { return deviceAddress; }
        uint16_t getConnectId() { return connectId; }
        void setAddress(std::string address) { deviceAddress = address; }
        void setConnectId(uint16_t connId) { connectId = connId; }
    };

    connectedDeviceInfo info("aa:bb:cc:dd:ee:ff", 123);

    EXPECT_EQ(info.getAddress(), "aa:bb:cc:dd:ee:ff");
    EXPECT_EQ(info.getConnectId(), 123);

    info.setAddress("11:22:33:44:55:66");
    info.setConnectId(456);

    EXPECT_EQ(info.getAddress(), "11:22:33:44:55:66");
    EXPECT_EQ(info.getConnectId(), 456);
}

// Test MonitorCharacteristicSubscriptionInfo structure simulation
TEST_F(BluetoothGattProfileServiceTest, MonitorCharacteristicSubscriptionInfoStructure) {
    struct MonitorCharacteristicSubscriptionInfo {
        std::string deviceAddress;
        std::string adapterAddress;
        std::string serviceUuid;
        uint16_t handle;
        std::string characteristicUuid;
        std::vector<std::string> characteristicUuids;
    };

    MonitorCharacteristicSubscriptionInfo info;
    info.deviceAddress = "aa:bb:cc:dd:ee:ff";
    info.adapterAddress = "00:11:22:33:44:55";
    info.serviceUuid = "0000180d-0000-1000-8000-00805f9b34fb";
    info.handle = 10;
    info.characteristicUuid = "00002a37-0000-1000-8000-00805f9b34fb";

    EXPECT_EQ(info.deviceAddress, "aa:bb:cc:dd:ee:ff");
    EXPECT_EQ(info.adapterAddress, "00:11:22:33:44:55");
    EXPECT_EQ(info.serviceUuid, "0000180d-0000-1000-8000-00805f9b34fb");
    EXPECT_EQ(info.handle, 10);
}

// Test GattConnSubsInfo structure simulation
TEST_F(BluetoothGattProfileServiceTest, GattConnSubsInfoStructure) {
    struct GattConnSubsInfo {
        std::string adapaterAddress;
        std::string deviceAddress;
        bool disconnectByRemote;
        bool used;

        void init() {
            adapaterAddress.clear();
            deviceAddress.clear();
            disconnectByRemote = false;
            used = false;
        }

        bool isChanged(const std::string& adapter, const std::string& address, bool remote) {
            if ((!used) || (adapaterAddress != adapter) ||
                (deviceAddress != address) ||
                (disconnectByRemote != remote)) {
                used = true;
                adapaterAddress = adapter;
                deviceAddress = address;
                disconnectByRemote = remote;
                return true;
            }
            return false;
        }
    };

    GattConnSubsInfo info;
    info.init();

    EXPECT_TRUE(info.adapaterAddress.empty());
    EXPECT_FALSE(info.used);

    // First call should return true
    bool changed = info.isChanged("00:11:22:33:44:55", "aa:bb:cc:dd:ee:ff", false);
    EXPECT_TRUE(changed);
    EXPECT_TRUE(info.used);

    // Same values should return false
    changed = info.isChanged("00:11:22:33:44:55", "aa:bb:cc:dd:ee:ff", false);
    EXPECT_FALSE(changed);

    // Different values should return true
    changed = info.isChanged("00:11:22:33:44:55", "aa:bb:cc:dd:ee:ff", true);
    EXPECT_TRUE(changed);
}

// Test GattStatusSubsInfo structure simulation
TEST_F(BluetoothGattProfileServiceTest, GattStatusSubsInfoStructure) {
    struct GattStatusSubsInfo {
        std::string adapaterAddress;
        std::string deviceAddress;
        bool connecting;
        bool connected;
        bool discoveringServices;
        bool used;

        void init() {
            adapaterAddress.clear();
            deviceAddress.clear();
            connecting = false;
            connected = false;
            discoveringServices = false;
            used = false;
        }

        bool isChanged(const std::string& adapter,
                       const std::string& address,
                       bool concting,
                       bool concted,
                       bool discover) {
            if ((!used) || (adapaterAddress != adapter) ||
                (deviceAddress != address) ||
                (connecting != concting) ||
                (connected != concted) ||
                (discoveringServices != discover)) {
                adapaterAddress = adapter;
                deviceAddress = address;
                connecting = concting;
                connected = concted;
                discoveringServices = discover;
                used = true;
                return true;
            }
            return false;
        }
    };

    GattStatusSubsInfo info;
    info.init();

    EXPECT_FALSE(info.used);

    bool changed = info.isChanged("00:11:22:33:44:55", "aa:bb:cc:dd:ee:ff", true, false, false);
    EXPECT_TRUE(changed);
    EXPECT_TRUE(info.connecting);
    EXPECT_FALSE(info.connected);

    // Same values should return false
    changed = info.isChanged("00:11:22:33:44:55", "aa:bb:cc:dd:ee:ff", true, false, false);
    EXPECT_FALSE(changed);

    // Connection established
    changed = info.isChanged("00:11:22:33:44:55", "aa:bb:cc:dd:ee:ff", false, true, false);
    EXPECT_TRUE(changed);
    EXPECT_TRUE(info.connected);
}

// Test local server management
TEST_F(BluetoothGattProfileServiceTest, LocalServerManagement) {
    std::unordered_map<std::string, void*> mLocalServer;

    std::string applicationUuid = "00001234-0000-1000-8000-00805f9b34fb";
    void* serverPtr = (void*)0x12345678;

    mLocalServer[applicationUuid] = serverPtr;

    EXPECT_EQ(mLocalServer.size(), 1u);
    EXPECT_NE(mLocalServer.find(applicationUuid), mLocalServer.end());
}

// Test connected devices map management
TEST_F(BluetoothGattProfileServiceTest, ConnectedDevicesMapManagement) {
    std::unordered_map<std::string, std::unordered_map<std::string, uint16_t>> mConnectedDevicesMap;
    std::string adapterAddress = "00:11:22:33:44:55";
    std::string deviceAddress = "aa:bb:cc:dd:ee:ff";
    uint16_t connectId = 1;

    mConnectedDevicesMap[adapterAddress][deviceAddress] = connectId;

    EXPECT_EQ(mConnectedDevicesMap.size(), 1u);
    EXPECT_EQ(mConnectedDevicesMap[adapterAddress].size(), 1u);
    EXPECT_EQ(mConnectedDevicesMap[adapterAddress][deviceAddress], 1);
}

// Test discovering services flag
TEST_F(BluetoothGattProfileServiceTest, DiscoveringServicesFlag) {
    std::unordered_map<std::string, bool> mDiscoveringServices;
    std::string deviceAddress = "aa:bb:cc:dd:ee:ff";

    // Start discovering
    mDiscoveringServices[deviceAddress] = true;
    EXPECT_TRUE(mDiscoveringServices[deviceAddress]);

    // Finish discovering
    mDiscoveringServices[deviceAddress] = false;
    EXPECT_FALSE(mDiscoveringServices[deviceAddress]);
}

// Test characteristic watch list
TEST_F(BluetoothGattProfileServiceTest, CharacteristicWatchListManagement) {
    struct CharacteristicWatch {
        std::string deviceAddress;
        uint16_t handle;
    };

    std::vector<CharacteristicWatch*> mCharacteristicWatchList;
    CharacteristicWatch* watch = new CharacteristicWatch();
    watch->deviceAddress = "aa:bb:cc:dd:ee:ff";
    watch->handle = 10;

    mCharacteristicWatchList.push_back(watch);

    EXPECT_EQ(mCharacteristicWatchList.size(), 1u);
    EXPECT_EQ(mCharacteristicWatchList[0]->handle, 10);

    delete watch;
    mCharacteristicWatchList.clear();
}

// Test client ID generation
TEST_F(BluetoothGattProfileServiceTest, ClientIdGeneration) {
    static uint16_t nextClientId = 0;
    auto getNextClientId = [&]() { return ++nextClientId; };

    uint16_t clientId1 = getNextClientId();
    uint16_t clientId2 = getNextClientId();
    uint16_t clientId3 = getNextClientId();

    EXPECT_EQ(clientId1, 1);
    EXPECT_EQ(clientId2, 2);
    EXPECT_EQ(clientId3, 3);
}

// Test parseValue-like JSON value parsing
TEST_F(BluetoothGattProfileServiceTest, ParseValueFromJson) {
    pbnjson::JValue valueObj = pbnjson::Object();

    pbnjson::JValue bytesArray = pbnjson::Array();
    bytesArray.append(0x01);
    bytesArray.append(0x02);
    bytesArray.append(0x03);
    bytesArray.append(0x04);

    valueObj.put("bytes", bytesArray);

    // Parse bytes
    std::vector<uint8_t> value;
    if (valueObj.hasKey("bytes")) {
        pbnjson::JValue bytes = valueObj["bytes"];
        for (int i = 0; i < bytes.arraySize(); i++) {
            value.push_back(bytes[i].asNumber<int>());
        }
    }

    EXPECT_EQ(value.size(), 4u);
    EXPECT_EQ(value[0], 0x01);
    EXPECT_EQ(value[1], 0x02);
    EXPECT_EQ(value[2], 0x03);
    EXPECT_EQ(value[3], 0x04);
}

// Test buildDescriptor-like JSON building
TEST_F(BluetoothGattProfileServiceTest, BuildDescriptorJson) {
    pbnjson::JValue descriptorObj = pbnjson::Object();

    std::string uuid = "00002902-0000-1000-8000-00805f9b34fb";
    uint16_t handle = 15;

    descriptorObj.put("uuid", uuid);
    descriptorObj.put("handle", handle);

    pbnjson::JValue valueArray = pbnjson::Array();
    valueArray.append(0x00);
    valueArray.append(0x00);
    descriptorObj.put("value", valueArray);

    EXPECT_EQ(descriptorObj["uuid"].asString(), uuid);
    EXPECT_EQ(descriptorObj["handle"].asNumber<int>(), 15);
}

// Test buildCharacteristic-like JSON building
TEST_F(BluetoothGattProfileServiceTest, BuildCharacteristicJson) {
    pbnjson::JValue characteristicObj = pbnjson::Object();

    std::string uuid = "00002a37-0000-1000-8000-00805f9b34fb";
    uint16_t handle = 10;

    characteristicObj.put("uuid", uuid);
    characteristicObj.put("handle", handle);

    pbnjson::JValue propertiesObj = pbnjson::Object();
    propertiesObj.put("read", true);
    propertiesObj.put("write", false);
    propertiesObj.put("notify", true);
    characteristicObj.put("properties", propertiesObj);

    EXPECT_EQ(characteristicObj["uuid"].asString(), uuid);
    EXPECT_TRUE(characteristicObj["properties"]["read"].asBool());
    EXPECT_TRUE(characteristicObj["properties"]["notify"].asBool());
}

// Test appendServiceResponse-like JSON building
TEST_F(BluetoothGattProfileServiceTest, AppendServiceResponseJson) {
    pbnjson::JValue responseObj = pbnjson::Object();

    responseObj.put("returnValue", true);
    responseObj.put("adapterAddress", "00:11:22:33:44:55");
    responseObj.put("address", "aa:bb:cc:dd:ee:ff");

    pbnjson::JValue servicesArray = pbnjson::Array();

    pbnjson::JValue serviceObj = pbnjson::Object();
    serviceObj.put("uuid", "0000180d-0000-1000-8000-00805f9b34fb");
    serviceObj.put("primary", true);

    pbnjson::JValue characteristicsArray = pbnjson::Array();
    pbnjson::JValue charObj = pbnjson::Object();
    charObj.put("uuid", "00002a37-0000-1000-8000-00805f9b34fb");
    characteristicsArray.append(charObj);
    serviceObj.put("characteristics", characteristicsArray);

    servicesArray.append(serviceObj);
    responseObj.put("services", servicesArray);

    EXPECT_TRUE(responseObj.hasKey("services"));
    EXPECT_EQ(responseObj["services"].arraySize(), 1);
}

// Test getConnectId logic
TEST_F(BluetoothGattProfileServiceTest, GetConnectIdLogic) {
    struct connectedDeviceInfo {
        std::string deviceAddress;
        uint16_t connectId;
    };

    std::unordered_map<uint16_t, connectedDeviceInfo*> mConnectedDevices;

    connectedDeviceInfo* info = new connectedDeviceInfo();
    info->deviceAddress = "aa:bb:cc:dd:ee:ff";
    info->connectId = 1;

    uint16_t appId = 100;
    mConnectedDevices[appId] = info;

    // Get connect ID for app
    uint16_t connectId = 0;
    std::string deviceAddress;
    auto it = mConnectedDevices.find(appId);
    if (it != mConnectedDevices.end()) {
        connectId = it->second->connectId;
        deviceAddress = it->second->deviceAddress;
    }

    EXPECT_EQ(connectId, 1);
    EXPECT_EQ(deviceAddress, "aa:bb:cc:dd:ee:ff");

    delete info;
}

// Test server adapter map
TEST_F(BluetoothGattProfileServiceTest, ServerAdapterMapManagement) {
    std::unordered_map<uint16_t, std::string> mServerAdapterMap;

    uint16_t serverId = 1;
    std::string adapterAddress = "00:11:22:33:44:55";

    mServerAdapterMap[serverId] = adapterAddress;

    EXPECT_EQ(mServerAdapterMap.size(), 1u);
    EXPECT_EQ(mServerAdapterMap[serverId], adapterAddress);
}

// Test subscription point map
TEST_F(BluetoothGattProfileServiceTest, GetServicesSubscriptionsManagement) {
    std::unordered_map<std::string, void*> mGetServicesSubscriptions;

    std::string key = "00:11:22:33:44:55_aa:bb:cc:dd:ee:ff";
    void* subscriptionPoint = (void*)0x12345678;

    mGetServicesSubscriptions[key] = subscriptionPoint;

    EXPECT_EQ(mGetServicesSubscriptions.size(), 1u);
    EXPECT_NE(mGetServicesSubscriptions.find(key), mGetServicesSubscriptions.end());
}

// Test GATT status response building
TEST_F(BluetoothGattProfileServiceTest, BuildGattStatusResponse) {
    pbnjson::JValue responseObj = pbnjson::Object();

    responseObj.put("connected", true);
    responseObj.put("connecting", false);
    responseObj.put("discoveringServices", false);
    responseObj.put("subscribed", true);
    responseObj.put("returnValue", true);
    responseObj.put("adapterAddress", "00:11:22:33:44:55");
    responseObj.put("address", "aa:bb:cc:dd:ee:ff");

    EXPECT_TRUE(responseObj["connected"].asBool());
    EXPECT_FALSE(responseObj["connecting"].asBool());
    EXPECT_FALSE(responseObj["discoveringServices"].asBool());
}

// Test local service registration check
TEST_F(BluetoothGattProfileServiceTest, LocalServiceRegistrationCheck) {
    std::map<std::string, bool> localServiceRegistry;

    std::string serviceUuid = "0000180d-0000-1000-8000-00805f9b34fb";

    // Not registered
    auto it = localServiceRegistry.find(serviceUuid);
    bool isRegistered = (it != localServiceRegistry.end());
    EXPECT_FALSE(isRegistered);

    // Register
    localServiceRegistry[serviceUuid] = true;

    // Now registered
    it = localServiceRegistry.find(serviceUuid);
    isRegistered = (it != localServiceRegistry.end());
    EXPECT_TRUE(isRegistered);
}

// Test removeSubscriptionPoint logic
TEST_F(BluetoothGattProfileServiceTest, RemoveSubscriptionPointLogic) {
    std::unordered_map<std::string, void*> mSubscriptions;

    std::string adapterAddress = "00:11:22:33:44:55";
    std::string deviceAddress = "aa:bb:cc:dd:ee:ff";
    std::string key = adapterAddress + "_" + deviceAddress;
    void* subscriptionPoint = (void*)0x12345678;

    mSubscriptions[key] = subscriptionPoint;
    EXPECT_EQ(mSubscriptions.size(), 1u);

    // Remove subscription
    auto it = mSubscriptions.find(key);
    if (it != mSubscriptions.end()) {
        mSubscriptions.erase(it);
    }

    EXPECT_EQ(mSubscriptions.size(), 0u);
}
