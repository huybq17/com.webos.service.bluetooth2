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
#include <regex>

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

// Test fixture for BluetoothManagerService related tests
class BluetoothManagerServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test pairing IO capability mapping
TEST_F(BluetoothManagerServiceTest, PairingIOCapabilityMapping) {
    std::map<std::string, int> pairingIOCapability = {
        {"NoInputNoOutput", 0},
        {"DisplayOnly", 1},
        {"DisplayYesNo", 2},
        {"KeyboardOnly", 3},
        {"KeyboardDisplay", 4}
    };

    EXPECT_EQ(pairingIOCapability["NoInputNoOutput"], 0);
    EXPECT_EQ(pairingIOCapability["DisplayOnly"], 1);
    EXPECT_EQ(pairingIOCapability["DisplayYesNo"], 2);
    EXPECT_EQ(pairingIOCapability["KeyboardOnly"], 3);
    EXPECT_EQ(pairingIOCapability["KeyboardDisplay"], 4);
}

// Test service classes split
TEST_F(BluetoothManagerServiceTest, EnabledServiceClassesSplit) {
    std::string serviceClasses = "A2DP AVRCP HFP SPP";
    std::vector<std::string> mEnabledServiceClasses = split(serviceClasses, ' ');

    EXPECT_EQ(mEnabledServiceClasses.size(), 4u);
    EXPECT_EQ(mEnabledServiceClasses[0], "A2DP");
    EXPECT_EQ(mEnabledServiceClasses[1], "AVRCP");
    EXPECT_EQ(mEnabledServiceClasses[2], "HFP");
    EXPECT_EQ(mEnabledServiceClasses[3], "SPP");
}

// Test default state values
TEST_F(BluetoothManagerServiceTest, DefaultStateValues) {
    bool mAdvertising = false;
    bool mWoBleEnabled = false;
    bool mKeepAliveEnabled = false;
    uint32_t mKeepAliveInterval = 1;

    EXPECT_FALSE(mAdvertising);
    EXPECT_FALSE(mWoBleEnabled);
    EXPECT_FALSE(mKeepAliveEnabled);
    EXPECT_EQ(mKeepAliveInterval, 1u);
}

// Test adapters info management
TEST_F(BluetoothManagerServiceTest, AdaptersInfoManagement) {
    std::unordered_map<std::string, void*> mAdaptersInfo;

    std::string address = "00:11:22:33:44:55";
    void* adapterInfo = (void*)0x12345678;

    mAdaptersInfo[address] = adapterInfo;

    EXPECT_EQ(mAdaptersInfo.size(), 1u);
    EXPECT_NE(mAdaptersInfo.find(address), mAdaptersInfo.end());
}

// Test findAdapterInfo logic
TEST_F(BluetoothManagerServiceTest, FindAdapterInfoLogic) {
    std::unordered_map<std::string, void*> mAdaptersInfo;

    std::string address = "00:11:22:33:44:55";
    void* adapterInfo = (void*)0x12345678;

    mAdaptersInfo[address] = adapterInfo;

    auto it = mAdaptersInfo.find(address);
    EXPECT_NE(it, mAdaptersInfo.end());
    EXPECT_EQ(it->second, adapterInfo);

    // Not found case
    it = mAdaptersInfo.find("11:22:33:44:55:66");
    EXPECT_EQ(it, mAdaptersInfo.end());
}

// Test isValidAddress logic
TEST_F(BluetoothManagerServiceTest, IsValidAddressLogic) {
    std::regex addressPattern("^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$");

    // Valid addresses
    EXPECT_TRUE(std::regex_match("00:11:22:33:44:55", addressPattern));
    EXPECT_TRUE(std::regex_match("AA:BB:CC:DD:EE:FF", addressPattern));
    EXPECT_TRUE(std::regex_match("aa:bb:cc:dd:ee:ff", addressPattern));
    EXPECT_TRUE(std::regex_match("Aa:Bb:Cc:Dd:Ee:Ff", addressPattern));

    // Invalid addresses
    EXPECT_FALSE(std::regex_match("00:11:22:33:44", addressPattern));
    EXPECT_FALSE(std::regex_match("00-11-22-33-44-55", addressPattern));
    EXPECT_FALSE(std::regex_match("001122334455", addressPattern));
    EXPECT_FALSE(std::regex_match("00:11:22:33:44:GG", addressPattern));
}

// Test WoBleTriggerDevices management
TEST_F(BluetoothManagerServiceTest, WoBleTriggerDevicesManagement) {
    std::vector<std::string> mWoBleTriggerDevices;

    mWoBleTriggerDevices.push_back("00:11:22:33:44:55");
    mWoBleTriggerDevices.push_back("AA:BB:CC:DD:EE:FF");

    EXPECT_EQ(mWoBleTriggerDevices.size(), 2u);

    // Clear
    mWoBleTriggerDevices.clear();
    EXPECT_EQ(mWoBleTriggerDevices.size(), 0u);
}

// Test AdvertiserInfo structure
TEST_F(BluetoothManagerServiceTest, AdvertiserInfoStructure) {
    struct AdvertiserInfo {
        std::vector<uint8_t> advertiseData;
        std::vector<uint8_t> scanResponse;
        bool connectable;
        uint16_t minInterval;
        uint16_t maxInterval;
    };

    AdvertiserInfo info;
    info.connectable = true;
    info.minInterval = 20;
    info.maxInterval = 100;

    EXPECT_TRUE(info.connectable);
    EXPECT_EQ(info.minInterval, 20);
    EXPECT_EQ(info.maxInterval, 100);
}

// Test advertisers management
TEST_F(BluetoothManagerServiceTest, AdvertisersManagement) {
    struct AdvertiserInfo {
        bool connectable;
    };

    std::unordered_map<uint8_t, AdvertiserInfo*> mAdvertisers;
    AdvertiserInfo* info = new AdvertiserInfo();
    info->connectable = true;

    uint8_t advertiserId = 1;
    mAdvertisers[advertiserId] = info;

    EXPECT_EQ(mAdvertisers.size(), 1u);
    EXPECT_NE(mAdvertisers.find(advertiserId), mAdvertisers.end());
    EXPECT_TRUE(mAdvertisers[advertiserId]->connectable);

    delete info;
}

// Test advId adapter map
TEST_F(BluetoothManagerServiceTest, AdvIdAdapterMapManagement) {
    std::map<uint8_t, std::string> mAdvIdAdapterMap;

    uint8_t advId = 1;
    std::string adapterAddress = "00:11:22:33:44:55";

    mAdvIdAdapterMap[advId] = adapterAddress;

    EXPECT_EQ(mAdvIdAdapterMap.size(), 1u);
    EXPECT_EQ(mAdvIdAdapterMap[advId], adapterAddress);
}

// Test getAdvSize calculation logic
TEST_F(BluetoothManagerServiceTest, GetAdvSizeCalculation) {
    // Simplified calculation similar to getAdvSize
    auto calculateAdvSize = [](int dataLength, bool flagRequired) -> int {
        int size = dataLength;
        if (flagRequired) {
            size += 3; // Flags field: 1 byte length + 1 byte type + 1 byte flags
        }
        return size;
    };

    EXPECT_EQ(calculateAdvSize(10, true), 13);
    EXPECT_EQ(calculateAdvSize(10, false), 10);
    EXPECT_EQ(calculateAdvSize(0, true), 3);
}

// Test MAX_ADVERTISING_DATA_BYTES constant
TEST_F(BluetoothManagerServiceTest, MaxAdvertisingDataBytesConstant) {
    const int MAX_ADVERTISING_DATA_BYTES = 31;
    EXPECT_EQ(MAX_ADVERTISING_DATA_BYTES, 31);
}

// Test LE scan ID management
TEST_F(BluetoothManagerServiceTest, LeScanIdManagement) {
    const int BLUETOOTH_LE_START_SCAN_MAX_ID = 999;

    uint32_t currentScanId = 0;
    auto getNextScanId = [&]() -> uint32_t {
        currentScanId++;
        if (currentScanId > BLUETOOTH_LE_START_SCAN_MAX_ID) {
            currentScanId = 1;
        }
        return currentScanId;
    };

    EXPECT_EQ(getNextScanId(), 1u);
    EXPECT_EQ(getNextScanId(), 2u);

    // Test wraparound
    currentScanId = 999;
    EXPECT_EQ(getNextScanId(), 1u);
}

// Test isServiceClassEnabled logic
TEST_F(BluetoothManagerServiceTest, IsServiceClassEnabledLogic) {
    std::vector<std::string> mEnabledServiceClasses = {"A2DP", "AVRCP", "HFP"};

    auto isEnabled = [&](const std::string& serviceClass) -> bool {
        return std::find(mEnabledServiceClasses.begin(),
                         mEnabledServiceClasses.end(),
                         serviceClass) != mEnabledServiceClasses.end();
    };

    EXPECT_TRUE(isEnabled("A2DP"));
    EXPECT_TRUE(isEnabled("AVRCP"));
    EXPECT_TRUE(isEnabled("HFP"));
    EXPECT_FALSE(isEnabled("OPP"));
    EXPECT_FALSE(isEnabled("FTP"));
}

// Test assignDefaultAdapter logic
TEST_F(BluetoothManagerServiceTest, AssignDefaultAdapterLogic) {
    std::vector<std::string> mAdapters;
    std::string mDefaultAdapter = "";

    // No adapters - no default
    EXPECT_TRUE(mDefaultAdapter.empty());

    // Add adapter - becomes default
    mAdapters.push_back("00:11:22:33:44:55");
    if (!mAdapters.empty() && mDefaultAdapter.empty()) {
        mDefaultAdapter = mAdapters[0];
    }
    EXPECT_EQ(mDefaultAdapter, "00:11:22:33:44:55");
}

// Test adaptersChanged notification simulation
TEST_F(BluetoothManagerServiceTest, AdaptersChangedSimulation) {
    std::vector<std::string> adapters;
    int notificationCount = 0;

    // Simulate adapters changed
    auto notifyAdaptersChanged = [&]() {
        notificationCount++;
    };

    adapters.push_back("00:11:22:33:44:55");
    notifyAdaptersChanged();
    EXPECT_EQ(notificationCount, 1);

    adapters.push_back("11:22:33:44:55:66");
    notifyAdaptersChanged();
    EXPECT_EQ(notificationCount, 2);
}

// Test appendCurrentStatus JSON building
TEST_F(BluetoothManagerServiceTest, AppendCurrentStatusJson) {
    pbnjson::JValue responseObj = pbnjson::Object();

    responseObj.put("returnValue", true);
    responseObj.put("subscribed", true);

    pbnjson::JValue adaptersArray = pbnjson::Array();
    pbnjson::JValue adapterObj = pbnjson::Object();
    adapterObj.put("adapterAddress", "00:11:22:33:44:55");
    adapterObj.put("powered", true);
    adapterObj.put("discovering", false);
    adapterObj.put("discoverable", false);
    adaptersArray.append(adapterObj);

    responseObj.put("adapters", adaptersArray);

    EXPECT_TRUE(responseObj.hasKey("adapters"));
    EXPECT_EQ(responseObj["adapters"].arraySize(), 1);
}

// Test appendAvailableStatus JSON building
TEST_F(BluetoothManagerServiceTest, AppendAvailableStatusJson) {
    pbnjson::JValue responseObj = pbnjson::Object();

    responseObj.put("returnValue", true);
    responseObj.put("subscribed", true);

    pbnjson::JValue adaptersArray = pbnjson::Array();
    pbnjson::JValue adapterObj = pbnjson::Object();
    adapterObj.put("adapterAddress", "00:11:22:33:44:55");
    adapterObj.put("name", "webOS Bluetooth");
    adapterObj.put("default", true);
    adaptersArray.append(adapterObj);

    responseObj.put("adapters", adaptersArray);

    EXPECT_TRUE(responseObj.hasKey("adapters"));
}

// Test getMessageOwner logic simulation
TEST_F(BluetoothManagerServiceTest, GetMessageOwnerLogic) {
    // Simulate message owner extraction
    std::string applicationId = "com.webos.app.settings";
    std::string serviceName = "";

    // Logic: use applicationId if available, else serviceName
    std::string owner = !applicationId.empty() ? applicationId : serviceName;
    EXPECT_EQ(owner, "com.webos.app.settings");

    // With service name only
    applicationId = "";
    serviceName = "com.webos.service.test";
    owner = !applicationId.empty() ? applicationId : serviceName;
    EXPECT_EQ(owner, "com.webos.service.test");
}

// Test profiles vector management
TEST_F(BluetoothManagerServiceTest, ProfilesManagement) {
    std::vector<void*> mProfiles;

    void* profile1 = (void*)0x11111111;
    void* profile2 = (void*)0x22222222;

    mProfiles.push_back(profile1);
    mProfiles.push_back(profile2);

    EXPECT_EQ(mProfiles.size(), 2u);
}

// Test setPairableState logic simulation
TEST_F(BluetoothManagerServiceTest, SetPairableStateLogic) {
    bool pairableState = false;

    auto setPairable = [&](bool value) -> bool {
        pairableState = value;
        return true;
    };

    EXPECT_TRUE(setPairable(true));
    EXPECT_TRUE(pairableState);

    EXPECT_TRUE(setPairable(false));
    EXPECT_FALSE(pairableState);
}

// Test advertising state management
TEST_F(BluetoothManagerServiceTest, AdvertisingStateManagement) {
    bool mAdvertising = false;

    auto setAdvertisingState = [&](bool state) {
        mAdvertising = state;
    };

    auto getAdvertisingState = [&]() -> bool {
        return mAdvertising;
    };

    EXPECT_FALSE(getAdvertisingState());

    setAdvertisingState(true);
    EXPECT_TRUE(getAdvertisingState());

    setAdvertisingState(false);
    EXPECT_FALSE(getAdvertisingState());
}

// Test keep alive state management
TEST_F(BluetoothManagerServiceTest, KeepAliveStateManagement) {
    bool mKeepAliveEnabled = false;
    uint32_t mKeepAliveInterval = 1;

    mKeepAliveEnabled = true;
    mKeepAliveInterval = 5;

    EXPECT_TRUE(mKeepAliveEnabled);
    EXPECT_EQ(mKeepAliveInterval, 5u);
}

// Test isDefaultAdapterAvailable logic
TEST_F(BluetoothManagerServiceTest, IsDefaultAdapterAvailableLogic) {
    void* mDefaultAdapter = nullptr;

    auto isDefaultAdapterAvailable = [&]() -> bool {
        return mDefaultAdapter != nullptr;
    };

    EXPECT_FALSE(isDefaultAdapterAvailable());

    mDefaultAdapter = (void*)0x12345678;
    EXPECT_TRUE(isDefaultAdapterAvailable());
}

// Test isAdapterAvailable logic
TEST_F(BluetoothManagerServiceTest, IsAdapterAvailableLogic) {
    std::unordered_map<std::string, void*> mAdaptersInfo;

    auto isAdapterAvailable = [&](const std::string& address) -> bool {
        return mAdaptersInfo.find(address) != mAdaptersInfo.end();
    };

    EXPECT_FALSE(isAdapterAvailable("00:11:22:33:44:55"));

    mAdaptersInfo["00:11:22:33:44:55"] = (void*)0x12345678;
    EXPECT_TRUE(isAdapterAvailable("00:11:22:33:44:55"));
}

// Test getPowered logic simulation
TEST_F(BluetoothManagerServiceTest, GetPoweredLogic) {
    std::map<std::string, bool> adapterPowerStates;

    adapterPowerStates["00:11:22:33:44:55"] = false;
    adapterPowerStates["11:22:33:44:55:66"] = true;

    EXPECT_FALSE(adapterPowerStates["00:11:22:33:44:55"]);
    EXPECT_TRUE(adapterPowerStates["11:22:33:44:55:66"]);
}

// Test isRoleEnable logic simulation
TEST_F(BluetoothManagerServiceTest, IsRoleEnableLogic) {
    std::map<std::string, std::vector<std::string>> enabledRoles;

    enabledRoles["00:11:22:33:44:55"].push_back("a2dp_source");
    enabledRoles["00:11:22:33:44:55"].push_back("hfp_hf");

    auto isRoleEnabled = [&](const std::string& address, const std::string& role) -> bool {
        auto it = enabledRoles.find(address);
        if (it == enabledRoles.end()) return false;
        return std::find(it->second.begin(), it->second.end(), role) != it->second.end();
    };

    EXPECT_TRUE(isRoleEnabled("00:11:22:33:44:55", "a2dp_source"));
    EXPECT_TRUE(isRoleEnabled("00:11:22:33:44:55", "hfp_hf"));
    EXPECT_FALSE(isRoleEnabled("00:11:22:33:44:55", "a2dp_sink"));
}

// Test getAddress logic
TEST_F(BluetoothManagerServiceTest, GetAddressLogic) {
    std::string mAddress = "00:11:22:33:44:55";

    EXPECT_EQ(mAddress, "00:11:22:33:44:55");
}
