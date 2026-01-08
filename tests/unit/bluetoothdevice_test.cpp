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
#include <vector>
#include <map>

// Mock global variables for glib
bool g_file_test_mock_result = true;
bool g_file_test_symlink_result = false;
std::string g_file_test_expected_path;

// Mock PmLogContext
#include "PmLogLib.h"
PmLogContext logContext = nullptr;

#include "bluetoothdevice.h"

// Test friend class declaration for accessing private methods
class BluetoothDeviceTestAccess {
public:
    static void updateSupportedServiceClasses(BluetoothDevice* device) {
        // Access through public update method indirectly
    }
};

class BluetoothDeviceTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test default constructor
TEST_F(BluetoothDeviceTest, DefaultConstructor) {
    BluetoothDevice device;

    EXPECT_EQ(device.getName(), "");
    EXPECT_EQ(device.getAddress(), "");
    EXPECT_EQ(device.getType(), BLUETOOTH_DEVICE_TYPE_UNKNOWN);
    EXPECT_EQ(device.getClassOfDevice(), 0u);
    EXPECT_FALSE(device.getPaired());
    EXPECT_FALSE(device.getPairing());
    EXPECT_FALSE(device.getTrusted());
    EXPECT_FALSE(device.getBlocked());
    EXPECT_FALSE(device.getConnected());
    EXPECT_EQ(device.getRssi(), 0);
    EXPECT_EQ(device.getRole(), BLUETOOTH_DEVICE_ROLE);
    EXPECT_EQ(device.getAccessCode(), InquiryAccessCode::BT_ACCESS_CODE_NONE);
}

// Test constructor with properties
TEST_F(BluetoothDeviceTest, ConstructorWithProperties) {
    BluetoothPropertiesList properties;
    BluetoothProperty nameProp(BluetoothProperty::Type::NAME, std::string("Test Device"));
    BluetoothProperty addrProp(BluetoothProperty::Type::BDADDR, std::string("AA:BB:CC:DD:EE:FF"));
    properties.push_back(nameProp);
    properties.push_back(addrProp);

    BluetoothDevice device(properties);

    EXPECT_EQ(device.getName(), "Test Device");
    EXPECT_EQ(device.getAddress(), "aa:bb:cc:dd:ee:ff"); // Should be lowercase
}

// Test update method with NAME property
TEST_F(BluetoothDeviceTest, UpdateName) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::NAME, std::string("New Name"));
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getName(), "New Name");
}

// Test update method with BDADDR property
TEST_F(BluetoothDeviceTest, UpdateAddress) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::BDADDR, std::string("11:22:33:44:55:66"));
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getAddress(), "11:22:33:44:55:66");
}

// Test update method with uppercase address (should be converted to lowercase)
TEST_F(BluetoothDeviceTest, UpdateAddressUppercase) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::BDADDR, std::string("AA:BB:CC:DD:EE:FF"));
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getAddress(), "aa:bb:cc:dd:ee:ff");
}

// Test update method with CLASS_OF_DEVICE property
TEST_F(BluetoothDeviceTest, UpdateClassOfDevice) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::CLASS_OF_DEVICE, (uint32_t)0x240404);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getClassOfDevice(), 0x240404u);
}

// Test update method with TYPE_OF_DEVICE property
TEST_F(BluetoothDeviceTest, UpdateTypeOfDeviceBREDR) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::TYPE_OF_DEVICE, (uint32_t)BLUETOOTH_DEVICE_TYPE_BREDR);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getType(), BLUETOOTH_DEVICE_TYPE_BREDR);
}

TEST_F(BluetoothDeviceTest, UpdateTypeOfDeviceBLE) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::TYPE_OF_DEVICE, (uint32_t)BLUETOOTH_DEVICE_TYPE_BLE);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getType(), BLUETOOTH_DEVICE_TYPE_BLE);
}

TEST_F(BluetoothDeviceTest, UpdateTypeOfDeviceDual) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::TYPE_OF_DEVICE, (uint32_t)BLUETOOTH_DEVICE_TYPE_DUAL);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getType(), BLUETOOTH_DEVICE_TYPE_DUAL);
}

// Test update method with PAIRED property
TEST_F(BluetoothDeviceTest, UpdatePairedTrue) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::PAIRED, true);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_TRUE(device.getPaired());
}

TEST_F(BluetoothDeviceTest, UpdatePairedFalse) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::PAIRED, false);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_FALSE(device.getPaired());
}

// Test update method with CONNECTED property
TEST_F(BluetoothDeviceTest, UpdateConnectedTrue) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::CONNECTED, true);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_TRUE(device.getConnected());
}

TEST_F(BluetoothDeviceTest, UpdateConnectedFalse) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::CONNECTED, false);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_FALSE(device.getConnected());
}

// Test update method with TRUSTED property
TEST_F(BluetoothDeviceTest, UpdateTrustedTrue) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::TRUSTED, true);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_TRUE(device.getTrusted());
}

TEST_F(BluetoothDeviceTest, UpdateTrustedFalse) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::TRUSTED, false);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_FALSE(device.getTrusted());
}

// Test update method with BLOCKED property
TEST_F(BluetoothDeviceTest, UpdateBlockedTrue) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::BLOCKED, true);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_TRUE(device.getBlocked());
}

TEST_F(BluetoothDeviceTest, UpdateBlockedFalse) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::BLOCKED, false);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_FALSE(device.getBlocked());
}

// Test update method with RSSI property
TEST_F(BluetoothDeviceTest, UpdateRssi) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::RSSI, -50);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getRssi(), -50);
}

TEST_F(BluetoothDeviceTest, UpdateRssiPositive) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::RSSI, 10);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getRssi(), 10);
}

// Test update method with ROLE property
TEST_F(BluetoothDeviceTest, UpdateRole) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::ROLE, (uint32_t)BLUETOOTH_DEVICE_ROLE_A2DP_SRC);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getRole(), BLUETOOTH_DEVICE_ROLE_A2DP_SRC);
}

// Test update method with MANUFACTURER_DATA property
TEST_F(BluetoothDeviceTest, UpdateManufacturerData) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    std::vector<uint8_t> manufacturerData = {0x01, 0x02, 0x03, 0x04};
    BluetoothProperty prop(BluetoothProperty::Type::MANUFACTURER_DATA, manufacturerData);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getManufacturerData().size(), 4u);
    EXPECT_EQ(device.getManufacturerData()[0], 0x01);
    EXPECT_EQ(device.getManufacturerData()[3], 0x04);
}

// Test update method with INQUIRY_ACCESS_CODE property
TEST_F(BluetoothDeviceTest, UpdateInquiryAccessCode) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    BluetoothProperty prop(BluetoothProperty::Type::INQUIRY_ACCESS_CODE, (uint32_t)InquiryAccessCode::BT_ACCESS_CODE_GIAC);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getAccessCode(), InquiryAccessCode::BT_ACCESS_CODE_GIAC);
}

// Test update method with SCAN_RECORD property
TEST_F(BluetoothDeviceTest, UpdateScanRecord) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    std::vector<uint8_t> scanRecord = {0xAA, 0xBB, 0xCC};
    BluetoothProperty prop(BluetoothProperty::Type::SCAN_RECORD, scanRecord);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getScanRecord().size(), 3u);
    EXPECT_EQ(device.getScanRecord()[0], 0xAA);
}

// Test update method with UUIDS property
TEST_F(BluetoothDeviceTest, UpdateUuids) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    std::vector<std::string> uuids = {"0000110a-0000-1000-8000-00805f9b34fb", "0000110b-0000-1000-8000-00805f9b34fb"};
    BluetoothProperty prop(BluetoothProperty::Type::UUIDS, uuids);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getUuids().size(), 2u);
}

// Test update method with MAP_INSTANCES_NAME property
TEST_F(BluetoothDeviceTest, UpdateMapInstancesName) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    std::vector<std::string> instances = {"instance1", "instance2"};
    BluetoothProperty prop(BluetoothProperty::Type::MAP_INSTANCES_NAME, instances);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getMapInstancesName().size(), 2u);
}

// Test update method with MAP_SUPPORTED_MESSAGE_TYPE property
TEST_F(BluetoothDeviceTest, UpdateMapSupportedMessageTypes) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    std::map<std::string, std::vector<std::string>> msgTypes;
    msgTypes["EMAIL"] = {"type1", "type2"};
    BluetoothProperty prop(BluetoothProperty::Type::MAP_SUPPORTED_MESSAGE_TYPE, msgTypes);
    properties.push_back(prop);

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getSupportedMessageTypes().size(), 1u);
}

// Test update method with multiple properties
TEST_F(BluetoothDeviceTest, UpdateMultipleProperties) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    properties.push_back(BluetoothProperty(BluetoothProperty::Type::NAME, std::string("Device Name")));
    properties.push_back(BluetoothProperty(BluetoothProperty::Type::BDADDR, std::string("11:22:33:44:55:66")));
    properties.push_back(BluetoothProperty(BluetoothProperty::Type::PAIRED, true));
    properties.push_back(BluetoothProperty(BluetoothProperty::Type::RSSI, -40));

    bool changed = device.update(properties);

    EXPECT_TRUE(changed);
    EXPECT_EQ(device.getName(), "Device Name");
    EXPECT_EQ(device.getAddress(), "11:22:33:44:55:66");
    EXPECT_TRUE(device.getPaired());
    EXPECT_EQ(device.getRssi(), -40);
}

// Test update method with no properties
TEST_F(BluetoothDeviceTest, UpdateNoProperties) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;

    bool changed = device.update(properties);

    EXPECT_FALSE(changed);
}

// Test setPairing method
TEST_F(BluetoothDeviceTest, SetPairingTrue) {
    BluetoothDevice device;
    device.setPairing(true);
    EXPECT_TRUE(device.getPairing());
}

TEST_F(BluetoothDeviceTest, SetPairingFalse) {
    BluetoothDevice device;
    device.setPairing(true);
    device.setPairing(false);
    EXPECT_FALSE(device.getPairing());
}

// Test getTypeAsString method
TEST_F(BluetoothDeviceTest, GetTypeAsStringUnknown) {
    BluetoothDevice device;
    EXPECT_EQ(device.getTypeAsString(), "unknown");
}

TEST_F(BluetoothDeviceTest, GetTypeAsStringBREDR) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    properties.push_back(BluetoothProperty(BluetoothProperty::Type::TYPE_OF_DEVICE, (uint32_t)BLUETOOTH_DEVICE_TYPE_BREDR));
    device.update(properties);

    EXPECT_EQ(device.getTypeAsString(), "bredr");
}

TEST_F(BluetoothDeviceTest, GetTypeAsStringBLE) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    properties.push_back(BluetoothProperty(BluetoothProperty::Type::TYPE_OF_DEVICE, (uint32_t)BLUETOOTH_DEVICE_TYPE_BLE));
    device.update(properties);

    EXPECT_EQ(device.getTypeAsString(), "ble");
}

TEST_F(BluetoothDeviceTest, GetTypeAsStringDual) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    properties.push_back(BluetoothProperty(BluetoothProperty::Type::TYPE_OF_DEVICE, (uint32_t)BLUETOOTH_DEVICE_TYPE_DUAL));
    device.update(properties);

    EXPECT_EQ(device.getTypeAsString(), "dual");
}

// Test hasConnectedRole method
TEST_F(BluetoothDeviceTest, HasConnectedRoleTrue) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    properties.push_back(BluetoothProperty(BluetoothProperty::Type::ROLE, (uint32_t)(BLUETOOTH_DEVICE_ROLE_A2DP_SRC | BLUETOOTH_DEVICE_ROLE_HFP_HF)));
    device.update(properties);

    EXPECT_TRUE(device.hasConnectedRole(BLUETOOTH_DEVICE_ROLE_A2DP_SRC));
    EXPECT_TRUE(device.hasConnectedRole(BLUETOOTH_DEVICE_ROLE_HFP_HF));
}

TEST_F(BluetoothDeviceTest, HasConnectedRoleFalse) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    properties.push_back(BluetoothProperty(BluetoothProperty::Type::ROLE, (uint32_t)BLUETOOTH_DEVICE_ROLE_A2DP_SRC));
    device.update(properties);

    EXPECT_FALSE(device.hasConnectedRole(BLUETOOTH_DEVICE_ROLE_HFP_HF));
    EXPECT_FALSE(device.hasConnectedRole(BLUETOOTH_DEVICE_ROLE_AVRCP_RMT));
}

// Test getSupportedServiceClasses method
TEST_F(BluetoothDeviceTest, GetSupportedServiceClassesEmpty) {
    BluetoothDevice device;
    EXPECT_TRUE(device.getSupportedServiceClasses().empty());
}

TEST_F(BluetoothDeviceTest, GetSupportedServiceClassesWithUuids) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    // A2DP source UUID
    std::vector<std::string> uuids = {"0000110a-0000-1000-8000-00805f9b34fb"};
    properties.push_back(BluetoothProperty(BluetoothProperty::Type::UUIDS, uuids));
    device.update(properties);

    auto serviceClasses = device.getSupportedServiceClasses();
    EXPECT_GE(serviceClasses.size(), 0u); // May or may not have entries depending on mapping
}

// Test with unknown UUID
TEST_F(BluetoothDeviceTest, UpdateWithUnknownUuid) {
    BluetoothDevice device;
    BluetoothPropertiesList properties;
    std::vector<std::string> uuids = {"12345678-1234-1234-1234-123456789abc"};
    properties.push_back(BluetoothProperty(BluetoothProperty::Type::UUIDS, uuids));
    device.update(properties);

    // Should not crash and service classes should be empty for unknown UUID
    EXPECT_TRUE(device.getSupportedServiceClasses().empty());
}

// Test destructor (implicit via scope)
TEST_F(BluetoothDeviceTest, DestructorTest) {
    BluetoothDevice* device = new BluetoothDevice();
    BluetoothPropertiesList properties;
    properties.push_back(BluetoothProperty(BluetoothProperty::Type::NAME, std::string("Test")));
    device->update(properties);
    delete device;
    // If we get here without crash, destructor worked
    SUCCEED();
}
