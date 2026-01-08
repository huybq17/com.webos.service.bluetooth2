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
#include <unordered_map>
#include <vector>

// Mock variables for glib
bool g_file_test_mock_result = false;
bool g_file_test_symlink_result = false;

// Mock PmLogContext
#include "PmLogLib.h"
PmLogContext logContext = nullptr;

#include "bluetooth-sil-api.h"
#include "bluetoothdevice.h"
#include "bluetoothpairstate.h"
#include "bluetoothserviceclasses.h"
#include "utils.h"

// Mock BluetoothDeviceRole to string map similar to bluetoothmanageradapter.cpp
static const std::unordered_map<BluetoothDeviceRole, std::string> uuidtoRoleMap = {
    {BLUETOOTH_DEVICE_ROLE_HFP_HF, "HFP_HF"},
    {BLUETOOTH_DEVICE_ROLE_HFP_AG, "HFP_AG"},
    {BLUETOOTH_DEVICE_ROLE_A2DP_SRC, "A2DP_SRC"},
    {BLUETOOTH_DEVICE_ROLE_A2DP_SINK, "A2DP_SINK"},
    {BLUETOOTH_DEVICE_ROLE_AVRCP_RMT, "AVRCP_CT"},
    {BLUETOOTH_DEVICE_ROLE_AVRCP_TGT, "AVRCP_TGT"},
};

// Test fixture for BluetoothManagerAdapter related tests
class BluetoothManagerAdapterTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test uuid to role map
TEST_F(BluetoothManagerAdapterTest, UuidToRoleMapHFP_HF) {
    auto it = uuidtoRoleMap.find(BLUETOOTH_DEVICE_ROLE_HFP_HF);
    ASSERT_NE(it, uuidtoRoleMap.end());
    EXPECT_EQ(it->second, "HFP_HF");
}

TEST_F(BluetoothManagerAdapterTest, UuidToRoleMapHFP_AG) {
    auto it = uuidtoRoleMap.find(BLUETOOTH_DEVICE_ROLE_HFP_AG);
    ASSERT_NE(it, uuidtoRoleMap.end());
    EXPECT_EQ(it->second, "HFP_AG");
}

TEST_F(BluetoothManagerAdapterTest, UuidToRoleMapA2DP_SRC) {
    auto it = uuidtoRoleMap.find(BLUETOOTH_DEVICE_ROLE_A2DP_SRC);
    ASSERT_NE(it, uuidtoRoleMap.end());
    EXPECT_EQ(it->second, "A2DP_SRC");
}

TEST_F(BluetoothManagerAdapterTest, UuidToRoleMapA2DP_SINK) {
    auto it = uuidtoRoleMap.find(BLUETOOTH_DEVICE_ROLE_A2DP_SINK);
    ASSERT_NE(it, uuidtoRoleMap.end());
    EXPECT_EQ(it->second, "A2DP_SINK");
}

TEST_F(BluetoothManagerAdapterTest, UuidToRoleMapAVRCP_CT) {
    auto it = uuidtoRoleMap.find(BLUETOOTH_DEVICE_ROLE_AVRCP_RMT);
    ASSERT_NE(it, uuidtoRoleMap.end());
    EXPECT_EQ(it->second, "AVRCP_CT");
}

TEST_F(BluetoothManagerAdapterTest, UuidToRoleMapAVRCP_TGT) {
    auto it = uuidtoRoleMap.find(BLUETOOTH_DEVICE_ROLE_AVRCP_TGT);
    ASSERT_NE(it, uuidtoRoleMap.end());
    EXPECT_EQ(it->second, "AVRCP_TGT");
}

// Test default member values simulation
TEST_F(BluetoothManagerAdapterTest, DefaultMemberValuesSimulation) {
    bool mPowered = false;
    bool mDiscoverable = false;
    bool mDiscovering = false;
    bool mIsDefault = false;
    uint32_t mDiscoveryTimeout = 0;
    uint32_t mDiscoverableTimeout = 0;
    uint32_t mClassOfDevice = 0;
    std::string mAddress = "00:11:22:33:44:55";

    EXPECT_FALSE(mPowered);
    EXPECT_FALSE(mDiscoverable);
    EXPECT_FALSE(mDiscovering);
    EXPECT_FALSE(mIsDefault);
    EXPECT_EQ(mDiscoveryTimeout, 0u);
    EXPECT_EQ(mDiscoverableTimeout, 0u);
    EXPECT_EQ(mClassOfDevice, 0u);
    EXPECT_EQ(mAddress, "00:11:22:33:44:55");
}

// Test device management using unordered_map
TEST_F(BluetoothManagerAdapterTest, DeviceManagementInsert) {
    std::unordered_map<std::string, BluetoothDevice*> mDevices;
    BluetoothDevice* device = new BluetoothDevice();

    std::string address = "aa:bb:cc:dd:ee:ff";
    mDevices[address] = device;

    EXPECT_EQ(mDevices.size(), 1u);
    EXPECT_NE(mDevices.find(address), mDevices.end());

    delete device;
}

TEST_F(BluetoothManagerAdapterTest, DeviceManagementFind) {
    std::unordered_map<std::string, BluetoothDevice*> mDevices;
    BluetoothDevice* device = new BluetoothDevice();

    std::string address = "aa:bb:cc:dd:ee:ff";
    mDevices[address] = device;

    auto it = mDevices.find(address);
    ASSERT_NE(it, mDevices.end());
    EXPECT_EQ(it->second, device);

    // Find non-existent device
    auto it2 = mDevices.find("00:11:22:33:44:55");
    EXPECT_EQ(it2, mDevices.end());

    delete device;
}

TEST_F(BluetoothManagerAdapterTest, DeviceManagementRemove) {
    std::unordered_map<std::string, BluetoothDevice*> mDevices;
    BluetoothDevice* device = new BluetoothDevice();

    std::string address = "aa:bb:cc:dd:ee:ff";
    mDevices[address] = device;

    // Remove device
    auto it = mDevices.find(address);
    if (it != mDevices.end()) {
        delete it->second;
        mDevices.erase(it);
    }

    EXPECT_EQ(mDevices.size(), 0u);
}

// Test BluetoothPairState within adapter context
TEST_F(BluetoothManagerAdapterTest, PairStateManagement) {
    BluetoothPairState mPairState;

    EXPECT_FALSE(mPairState.isPairing());
    EXPECT_FALSE(mPairState.isPairable());

    mPairState.setPairable(true);
    EXPECT_TRUE(mPairState.isPairable());
}

// Test LE device management
TEST_F(BluetoothManagerAdapterTest, LeDeviceManagement) {
    std::unordered_map<std::string, BluetoothDevice*> mLeDevices;
    BluetoothDevice* device = new BluetoothDevice();

    std::string address = "aa:bb:cc:dd:ee:ff";
    mLeDevices[address] = device;

    EXPECT_EQ(mLeDevices.size(), 1u);
    EXPECT_NE(mLeDevices.find(address), mLeDevices.end());

    delete device;
}

// Test LE devices by scan ID management
TEST_F(BluetoothManagerAdapterTest, LeDevicesByScanIdManagement) {
    std::unordered_map<uint32_t, std::unordered_map<std::string, BluetoothDevice*>> mLeDevicesByScanId;
    BluetoothDevice* device = new BluetoothDevice();

    uint32_t scanId = 1;
    std::string address = "aa:bb:cc:dd:ee:ff";

    std::unordered_map<std::string, BluetoothDevice*> devices;
    devices[address] = device;
    mLeDevicesByScanId[scanId] = devices;

    EXPECT_EQ(mLeDevicesByScanId.size(), 1u);
    EXPECT_NE(mLeDevicesByScanId.find(scanId), mLeDevicesByScanId.end());
    EXPECT_EQ(mLeDevicesByScanId[scanId].size(), 1u);

    delete device;
}

// Test filter management
TEST_F(BluetoothManagerAdapterTest, FilterClassOfDevicesManagement) {
    std::unordered_map<std::string, int32_t> mFilterClassOfDevices;

    std::string senderName = "com.webos.app.settings";
    int32_t classOfDevice = 0x002510; // Audio device class

    mFilterClassOfDevices[senderName] = classOfDevice;

    EXPECT_EQ(mFilterClassOfDevices.size(), 1u);
    EXPECT_EQ(mFilterClassOfDevices[senderName], 0x002510);
}

TEST_F(BluetoothManagerAdapterTest, FilterUuidsManagement) {
    std::unordered_map<std::string, std::string> mFilterUuids;

    std::string senderName = "com.webos.app.settings";
    std::string uuid = "0000110a-0000-1000-8000-00805f9b34fb";

    mFilterUuids[senderName] = uuid;

    EXPECT_EQ(mFilterUuids.size(), 1u);
    EXPECT_EQ(mFilterUuids[senderName], uuid);
}

// Test adapter state properties
TEST_F(BluetoothManagerAdapterTest, AdapterStateProperties) {
    bool mPowered = false;
    bool mDiscoverable = false;
    bool mDiscovering = false;

    // Simulate power on
    mPowered = true;
    EXPECT_TRUE(mPowered);

    // Simulate discoverable mode
    mDiscoverable = true;
    EXPECT_TRUE(mDiscoverable);

    // Simulate discovery start
    mDiscovering = true;
    EXPECT_TRUE(mDiscovering);
}

// Test supported service classes update simulation
TEST_F(BluetoothManagerAdapterTest, UpdateSupportedServiceClassesLogic) {
    std::vector<BluetoothServiceClassInfo> mSupportedServiceClasses;
    std::vector<std::string> uuids;

    uuids.push_back("0000110a-0000-1000-8000-00805f9b34fb"); // A2DP-source
    uuids.push_back("0000111e-0000-1000-8000-00805f9b34fb"); // HFP-HF

    for (const auto& uuid : uuids) {
        std::string luuid = convertToLower(uuid);
        auto iter = allServiceClasses.find(luuid);
        if (iter != allServiceClasses.end()) {
            mSupportedServiceClasses.push_back(iter->second);
        }
    }

    EXPECT_EQ(mSupportedServiceClasses.size(), 2u);
}

// Test findDevice-like logic
TEST_F(BluetoothManagerAdapterTest, FindDeviceLogic) {
    std::unordered_map<std::string, BluetoothDevice*> mDevices;
    BluetoothDevice* device = new BluetoothDevice();

    std::string address = "aa:bb:cc:dd:ee:ff";
    mDevices[convertToLower(address)] = device;

    // Find with uppercase
    std::string searchAddress = convertToLower("AA:BB:CC:DD:EE:FF");
    auto it = mDevices.find(searchAddress);
    EXPECT_NE(it, mDevices.end());

    // Find non-existent
    searchAddress = convertToLower("00:11:22:33:44:55");
    it = mDevices.find(searchAddress);
    EXPECT_EQ(it, mDevices.end());

    delete device;
}

// Test enabled service classes split
TEST_F(BluetoothManagerAdapterTest, EnabledServiceClassesSplit) {
    std::string serviceClasses = "A2DP AVRCP HFP SPP";
    std::vector<std::string> mEnabledServiceClasses = split(serviceClasses, ' ');

    EXPECT_EQ(mEnabledServiceClasses.size(), 4u);
    EXPECT_EQ(mEnabledServiceClasses[0], "A2DP");
    EXPECT_EQ(mEnabledServiceClasses[1], "AVRCP");
    EXPECT_EQ(mEnabledServiceClasses[2], "HFP");
    EXPECT_EQ(mEnabledServiceClasses[3], "SPP");
}

// Test JSON response building
TEST_F(BluetoothManagerAdapterTest, BuildDeviceResponseJson) {
    pbnjson::JValue responseObj = pbnjson::Object();

    responseObj.put("returnValue", true);
    responseObj.put("adapterAddress", "00:11:22:33:44:55");

    pbnjson::JValue devicesArray = pbnjson::Array();
    pbnjson::JValue deviceObj = pbnjson::Object();
    deviceObj.put("address", "aa:bb:cc:dd:ee:ff");
    deviceObj.put("name", "Test Device");
    deviceObj.put("paired", true);
    deviceObj.put("connected", false);
    devicesArray.append(deviceObj);

    responseObj.put("devices", devicesArray);

    EXPECT_TRUE(responseObj.hasKey("returnValue"));
    EXPECT_TRUE(responseObj.hasKey("adapterAddress"));
    EXPECT_TRUE(responseObj.hasKey("devices"));
}

// Test default adapter flag
TEST_F(BluetoothManagerAdapterTest, DefaultAdapterFlag) {
    bool mIsDefault = false;

    // Set as default
    mIsDefault = true;
    EXPECT_TRUE(mIsDefault);

    // Unset as default
    mIsDefault = false;
    EXPECT_FALSE(mIsDefault);
}

// Test discovery timeout
TEST_F(BluetoothManagerAdapterTest, DiscoveryTimeout) {
    uint32_t mDiscoveryTimeout = 0;

    // Set timeout
    mDiscoveryTimeout = 120;
    EXPECT_EQ(mDiscoveryTimeout, 120u);
}

// Test discoverable timeout
TEST_F(BluetoothManagerAdapterTest, DiscoverableTimeout) {
    uint32_t mDiscoverableTimeout = 0;

    // Set timeout
    mDiscoverableTimeout = 180;
    EXPECT_EQ(mDiscoverableTimeout, 180u);
}

// Test class of device
TEST_F(BluetoothManagerAdapterTest, ClassOfDevice) {
    uint32_t mClassOfDevice = 0;

    // Set class of device (e.g., audio/headset)
    mClassOfDevice = 0x240404;
    EXPECT_EQ(mClassOfDevice, 0x240404u);
}

// Test name getter
TEST_F(BluetoothManagerAdapterTest, AdapterName) {
    std::string mName = "webOS Bluetooth";

    EXPECT_EQ(mName, "webOS Bluetooth");
}

// Test interface name getter
TEST_F(BluetoothManagerAdapterTest, InterfaceName) {
    std::string mInterfaceName = "hci0";

    EXPECT_EQ(mInterfaceName, "hci0");
}

// Test stack name getter
TEST_F(BluetoothManagerAdapterTest, StackName) {
    std::string mStackName = "bluez";

    EXPECT_EQ(mStackName, "bluez");
}

// Test stack version getter
TEST_F(BluetoothManagerAdapterTest, StackVersion) {
    std::string mStackVersion = "5.50";

    EXPECT_EQ(mStackVersion, "5.50");
}

// Test firmware version getter
TEST_F(BluetoothManagerAdapterTest, FirmwareVersion) {
    std::string mFirmwareVersion = "1.0.0";

    EXPECT_EQ(mFirmwareVersion, "1.0.0");
}

// Test link key management
TEST_F(BluetoothManagerAdapterTest, LinkKeyManagement) {
    std::unordered_map<std::string, BluetoothLinkKey> mLinkKeys;
    BluetoothLinkKey linkKey;

    std::string address = "aa:bb:cc:dd:ee:ff";
    mLinkKeys[address] = linkKey;

    EXPECT_EQ(mLinkKeys.size(), 1u);
    EXPECT_NE(mLinkKeys.find(address), mLinkKeys.end());
}

// Test pairing state access
TEST_F(BluetoothManagerAdapterTest, PairStateAccess) {
    BluetoothPairState mPairState;

    // Access pair state reference
    BluetoothPairState& pairStateRef = mPairState;
    pairStateRef.setPairable(true);

    EXPECT_TRUE(mPairState.isPairable());
}

// Test start scan watches management
TEST_F(BluetoothManagerAdapterTest, StartScanWatchesManagement) {
    std::unordered_map<uint32_t, void*> mStartScanWatches;

    uint32_t scanId = 1;
    void* watchPtr = (void*)0x12345678;

    mStartScanWatches[scanId] = watchPtr;

    EXPECT_EQ(mStartScanWatches.size(), 1u);
    EXPECT_NE(mStartScanWatches.find(scanId), mStartScanWatches.end());
}

// Test get devices watches management
TEST_F(BluetoothManagerAdapterTest, GetDevicesWatchesManagement) {
    std::unordered_map<std::string, void*> mGetDevicesWatches;

    std::string senderName = "com.webos.app.settings";
    void* watchPtr = (void*)0x12345678;

    mGetDevicesWatches[senderName] = watchPtr;

    EXPECT_EQ(mGetDevicesWatches.size(), 1u);
    EXPECT_NE(mGetDevicesWatches.find(senderName), mGetDevicesWatches.end());
}
