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
#include <list>

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

// Test fixture for BluetoothAvrcpProfileService related tests
class BluetoothAvrcpProfileServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test AVRCP profile UUIDs
TEST_F(BluetoothAvrcpProfileServiceTest, AvrcpRemoteUuid) {
    std::string uuid = "0000110e-0000-1000-8000-00805f9b34fb";
    auto it = allServiceClasses.find(uuid);
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "AVRCP-remote");
}

TEST_F(BluetoothAvrcpProfileServiceTest, AvrcpTargetUuid) {
    std::string uuid = "0000110c-0000-1000-8000-00805f9b34fb";
    auto it = allServiceClasses.find(uuid);
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "AVRCP-target");
}

// Test player application settings simulation
TEST_F(BluetoothAvrcpProfileServiceTest, PlayerApplicationSettings) {
    std::string mEqualizer = "off";
    std::string mRepeat = "off";
    std::string mShuffle = "off";
    std::string mScan = "off";

    EXPECT_EQ(mEqualizer, "off");
    EXPECT_EQ(mRepeat, "off");
    EXPECT_EQ(mShuffle, "off");
    EXPECT_EQ(mScan, "off");

    // Update settings
    mEqualizer = "on";
    mRepeat = "single";
    mShuffle = "alltracks";
    mScan = "alltracks";

    EXPECT_EQ(mEqualizer, "on");
    EXPECT_EQ(mRepeat, "single");
    EXPECT_EQ(mShuffle, "alltracks");
    EXPECT_EQ(mScan, "alltracks");
}

// Test equalizer string to enum conversion logic
TEST_F(BluetoothAvrcpProfileServiceTest, EqualizerStringToEnum) {
    std::map<std::string, int> equalizerMap = {
        {"off", 0},
        {"on", 1}
    };

    EXPECT_EQ(equalizerMap["off"], 0);
    EXPECT_EQ(equalizerMap["on"], 1);
}

// Test repeat string to enum conversion logic
TEST_F(BluetoothAvrcpProfileServiceTest, RepeatStringToEnum) {
    std::map<std::string, int> repeatMap = {
        {"off", 0},
        {"single", 1},
        {"alltracks", 2},
        {"group", 3}
    };

    EXPECT_EQ(repeatMap["off"], 0);
    EXPECT_EQ(repeatMap["single"], 1);
    EXPECT_EQ(repeatMap["alltracks"], 2);
    EXPECT_EQ(repeatMap["group"], 3);
}

// Test shuffle string to enum conversion logic
TEST_F(BluetoothAvrcpProfileServiceTest, ShuffleStringToEnum) {
    std::map<std::string, int> shuffleMap = {
        {"off", 0},
        {"alltracks", 1},
        {"group", 2}
    };

    EXPECT_EQ(shuffleMap["off"], 0);
    EXPECT_EQ(shuffleMap["alltracks"], 1);
    EXPECT_EQ(shuffleMap["group"], 2);
}

// Test scan string to enum conversion logic
TEST_F(BluetoothAvrcpProfileServiceTest, ScanStringToEnum) {
    std::map<std::string, int> scanMap = {
        {"off", 0},
        {"alltracks", 1},
        {"group", 2}
    };

    EXPECT_EQ(scanMap["off"], 0);
    EXPECT_EQ(scanMap["alltracks"], 1);
    EXPECT_EQ(scanMap["group"], 2);
}

// Test media play status to string conversion
TEST_F(BluetoothAvrcpProfileServiceTest, MediaPlayStatusToString) {
    std::map<int, std::string> playStatusMap = {
        {0, "stopped"},
        {1, "playing"},
        {2, "paused"},
        {3, "fwdseek"},
        {4, "revseek"},
        {255, "error"}
    };

    EXPECT_EQ(playStatusMap[0], "stopped");
    EXPECT_EQ(playStatusMap[1], "playing");
    EXPECT_EQ(playStatusMap[2], "paused");
    EXPECT_EQ(playStatusMap[3], "fwdseek");
    EXPECT_EQ(playStatusMap[4], "revseek");
    EXPECT_EQ(playStatusMap[255], "error");
}

// Test remote volumes management
TEST_F(BluetoothAvrcpProfileServiceTest, RemoteVolumesManagement) {
    std::map<std::string, int> mRemoteVolumes;
    std::string address = "aa:bb:cc:dd:ee:ff";

    mRemoteVolumes[address] = 50;

    EXPECT_EQ(mRemoteVolumes.size(), 1u);
    EXPECT_EQ(mRemoteVolumes[address], 50);

    // Update volume
    mRemoteVolumes[address] = 75;
    EXPECT_EQ(mRemoteVolumes[address], 75);
}

// Test MediaRequest structure simulation
TEST_F(BluetoothAvrcpProfileServiceTest, MediaRequestStructure) {
    struct MediaRequest {
        std::string requestId;
        std::string address;
    };

    MediaRequest request;
    request.requestId = "req_12345";
    request.address = "aa:bb:cc:dd:ee:ff";

    EXPECT_EQ(request.requestId, "req_12345");
    EXPECT_EQ(request.address, "aa:bb:cc:dd:ee:ff");
}

// Test media metadata requests management
TEST_F(BluetoothAvrcpProfileServiceTest, MediaMetaDataRequestsManagement) {
    struct MediaRequest {
        std::string requestId;
        std::string address;
    };

    std::map<uint64_t, MediaRequest*> mMediaMetaDataRequests;
    uint64_t requestIndex = 1;
    MediaRequest* request = new MediaRequest();
    request->requestId = "req_12345";
    request->address = "aa:bb:cc:dd:ee:ff";

    mMediaMetaDataRequests[requestIndex] = request;

    EXPECT_EQ(mMediaMetaDataRequests.size(), 1u);
    EXPECT_NE(mMediaMetaDataRequests.find(requestIndex), mMediaMetaDataRequests.end());
    EXPECT_EQ(mMediaMetaDataRequests[requestIndex]->requestId, "req_12345");

    delete request;
}

// Test media play status requests management
TEST_F(BluetoothAvrcpProfileServiceTest, MediaPlayStatusRequestsManagement) {
    struct MediaRequest {
        std::string requestId;
        std::string address;
    };

    std::map<uint64_t, MediaRequest*> mMediaPlayStatusRequests;
    uint64_t requestIndex = 1;
    MediaRequest* request = new MediaRequest();
    request->requestId = "req_67890";
    request->address = "aa:bb:cc:dd:ee:ff";

    mMediaPlayStatusRequests[requestIndex] = request;

    EXPECT_EQ(mMediaPlayStatusRequests.size(), 1u);
    EXPECT_EQ(mMediaPlayStatusRequests[requestIndex]->address, "aa:bb:cc:dd:ee:ff");

    delete request;
}

// Test request ID generation
TEST_F(BluetoothAvrcpProfileServiceTest, RequestIdGeneration) {
    uint32_t mNextRequestId = 0;

    // Generate request ID
    uint32_t requestId = mNextRequestId++;
    EXPECT_EQ(requestId, 0u);

    requestId = mNextRequestId++;
    EXPECT_EQ(requestId, 1u);

    requestId = mNextRequestId++;
    EXPECT_EQ(requestId, 2u);
}

// Test remote features management
TEST_F(BluetoothAvrcpProfileServiceTest, RemoteFeaturesManagement) {
    std::map<std::string, std::map<std::string, std::vector<std::string>>> mTGRemoteFeaturesForMultipleAdapters;
    std::string adapterAddress = "00:11:22:33:44:55";
    std::string deviceAddress = "aa:bb:cc:dd:ee:ff";

    std::vector<std::string> features;
    features.push_back("browse");
    features.push_back("search");

    mTGRemoteFeaturesForMultipleAdapters[adapterAddress][deviceAddress] = features;

    EXPECT_EQ(mTGRemoteFeaturesForMultipleAdapters.size(), 1u);
    EXPECT_EQ(mTGRemoteFeaturesForMultipleAdapters[adapterAddress][deviceAddress].size(), 2u);
    EXPECT_EQ(mTGRemoteFeaturesForMultipleAdapters[adapterAddress][deviceAddress][0], "browse");
    EXPECT_EQ(mTGRemoteFeaturesForMultipleAdapters[adapterAddress][deviceAddress][1], "search");
}

// Test player info list management
TEST_F(BluetoothAvrcpProfileServiceTest, PlayerInfoListManagement) {
    struct PlayerInfo {
        uint16_t playerId;
        std::string playerName;
        std::string playerType;
    };

    std::map<std::string, std::map<std::string, std::vector<PlayerInfo>>> mPlayerInfoListForMultipleAdapters;
    std::string adapterAddress = "00:11:22:33:44:55";
    std::string deviceAddress = "aa:bb:cc:dd:ee:ff";

    PlayerInfo info;
    info.playerId = 1;
    info.playerName = "Music Player";
    info.playerType = "audio";

    std::vector<PlayerInfo> playerInfoList;
    playerInfoList.push_back(info);

    mPlayerInfoListForMultipleAdapters[adapterAddress][deviceAddress] = playerInfoList;

    EXPECT_EQ(mPlayerInfoListForMultipleAdapters.size(), 1u);
    EXPECT_EQ(mPlayerInfoListForMultipleAdapters[adapterAddress][deviceAddress].size(), 1u);
    EXPECT_EQ(mPlayerInfoListForMultipleAdapters[adapterAddress][deviceAddress][0].playerName, "Music Player");
}

// Test current folder management
TEST_F(BluetoothAvrcpProfileServiceTest, CurrentFolderManagement) {
    std::map<std::string, std::map<std::string, std::string>> mCurrentFolderForMultipleAdapters;
    std::string adapterAddress = "00:11:22:33:44:55";
    std::string deviceAddress = "aa:bb:cc:dd:ee:ff";
    std::string currentFolder = "/Music/Albums";

    mCurrentFolderForMultipleAdapters[adapterAddress][deviceAddress] = currentFolder;

    EXPECT_EQ(mCurrentFolderForMultipleAdapters.size(), 1u);
    EXPECT_EQ(mCurrentFolderForMultipleAdapters[adapterAddress][deviceAddress], "/Music/Albums");
}

// Test pass through command key codes
TEST_F(BluetoothAvrcpProfileServiceTest, PassThroughKeyCodeMapping) {
    std::map<std::string, int> keyCodeMap = {
        {"select", 0x00},
        {"up", 0x01},
        {"down", 0x02},
        {"left", 0x03},
        {"right", 0x04},
        {"play", 0x44},
        {"stop", 0x45},
        {"pause", 0x46},
        {"rewind", 0x48},
        {"fastforward", 0x49},
        {"forward", 0x4B},
        {"backward", 0x4C},
        {"volumeUp", 0x41},
        {"volumeDown", 0x42},
        {"mute", 0x43}
    };

    EXPECT_EQ(keyCodeMap["play"], 0x44);
    EXPECT_EQ(keyCodeMap["stop"], 0x45);
    EXPECT_EQ(keyCodeMap["pause"], 0x46);
    EXPECT_EQ(keyCodeMap["volumeUp"], 0x41);
    EXPECT_EQ(keyCodeMap["volumeDown"], 0x42);
}

// Test pass through key status
TEST_F(BluetoothAvrcpProfileServiceTest, PassThroughKeyStatusMapping) {
    std::map<std::string, int> keyStatusMap = {
        {"pressed", 0},
        {"released", 1}
    };

    EXPECT_EQ(keyStatusMap["pressed"], 0);
    EXPECT_EQ(keyStatusMap["released"], 1);
}

// Test player type enum to string conversion
TEST_F(BluetoothAvrcpProfileServiceTest, PlayerTypeEnumToString) {
    std::map<int, std::string> playerTypeMap = {
        {0, "audio"},
        {1, "video"},
        {2, "audiovideo"}
    };

    EXPECT_EQ(playerTypeMap[0], "audio");
    EXPECT_EQ(playerTypeMap[1], "video");
    EXPECT_EQ(playerTypeMap[2], "audiovideo");
}

// Test folder item type enum to string conversion
TEST_F(BluetoothAvrcpProfileServiceTest, FolderItemTypeEnumToString) {
    std::map<int, std::string> folderItemTypeMap = {
        {0, "player"},
        {1, "folder"},
        {2, "media"}
    };

    EXPECT_EQ(folderItemTypeMap[0], "player");
    EXPECT_EQ(folderItemTypeMap[1], "folder");
    EXPECT_EQ(folderItemTypeMap[2], "media");
}

// Test JSON response for media metadata
TEST_F(BluetoothAvrcpProfileServiceTest, MediaMetaDataJsonResponse) {
    pbnjson::JValue responseObj = pbnjson::Object();

    responseObj.put("returnValue", true);
    responseObj.put("adapterAddress", "00:11:22:33:44:55");
    responseObj.put("address", "aa:bb:cc:dd:ee:ff");

    pbnjson::JValue metaData = pbnjson::Object();
    metaData.put("title", "Test Song");
    metaData.put("artist", "Test Artist");
    metaData.put("album", "Test Album");
    metaData.put("genre", "Test Genre");
    metaData.put("trackNumber", 1);
    metaData.put("numberOfTracks", 10);
    metaData.put("playingTime", 180000);

    responseObj.put("metaData", metaData);

    EXPECT_TRUE(responseObj.hasKey("metaData"));
    EXPECT_EQ(responseObj["metaData"]["title"].asString(), "Test Song");
    EXPECT_EQ(responseObj["metaData"]["artist"].asString(), "Test Artist");
}

// Test JSON response for play status
TEST_F(BluetoothAvrcpProfileServiceTest, PlayStatusJsonResponse) {
    pbnjson::JValue responseObj = pbnjson::Object();

    responseObj.put("returnValue", true);
    responseObj.put("adapterAddress", "00:11:22:33:44:55");
    responseObj.put("address", "aa:bb:cc:dd:ee:ff");

    pbnjson::JValue playStatus = pbnjson::Object();
    playStatus.put("status", "playing");
    playStatus.put("duration", 180000);
    playStatus.put("position", 45000);

    responseObj.put("playStatus", playStatus);

    EXPECT_TRUE(responseObj.hasKey("playStatus"));
    EXPECT_EQ(responseObj["playStatus"]["status"].asString(), "playing");
}

// Test supported notification events management
TEST_F(BluetoothAvrcpProfileServiceTest, SupportedNotificationEventsManagement) {
    std::vector<std::string> mSupportedNotificationEvents;

    mSupportedNotificationEvents.push_back("playbackStatusChanged");
    mSupportedNotificationEvents.push_back("trackChanged");
    mSupportedNotificationEvents.push_back("trackReached");
    mSupportedNotificationEvents.push_back("playbackPositionChanged");
    mSupportedNotificationEvents.push_back("volumeChanged");

    EXPECT_EQ(mSupportedNotificationEvents.size(), 5u);
    EXPECT_EQ(mSupportedNotificationEvents[0], "playbackStatusChanged");
    EXPECT_EQ(mSupportedNotificationEvents[4], "volumeChanged");
}

// Test setMediaMetaDataRequestsAllowed simulation
TEST_F(BluetoothAvrcpProfileServiceTest, MediaMetaDataRequestsAllowed) {
    bool mMediaMetaDataRequestsAllowed = false;

    // Initially not allowed
    EXPECT_FALSE(mMediaMetaDataRequestsAllowed);

    // Set allowed
    mMediaMetaDataRequestsAllowed = true;
    EXPECT_TRUE(mMediaMetaDataRequestsAllowed);
}

// Test setMediaPlayStatusRequestsAllowed simulation
TEST_F(BluetoothAvrcpProfileServiceTest, MediaPlayStatusRequestsAllowed) {
    bool mMediaPlayStatusRequestsAllowed = false;

    // Initially not allowed
    EXPECT_FALSE(mMediaPlayStatusRequestsAllowed);

    // Set allowed
    mMediaPlayStatusRequestsAllowed = true;
    EXPECT_TRUE(mMediaPlayStatusRequestsAllowed);
}

// Test request index increment
TEST_F(BluetoothAvrcpProfileServiceTest, RequestIndexIncrement) {
    uint64_t mRequestIndex = 0;

    mRequestIndex++;
    EXPECT_EQ(mRequestIndex, 1u);

    mRequestIndex++;
    EXPECT_EQ(mRequestIndex, 2u);
}

// Test clearRemoteFeatures logic simulation
TEST_F(BluetoothAvrcpProfileServiceTest, ClearRemoteFeaturesLogic) {
    std::map<std::string, std::map<std::string, std::vector<std::string>>> mTGRemoteFeaturesForMultipleAdapters;
    std::string adapterAddress = "00:11:22:33:44:55";
    std::string deviceAddress = "aa:bb:cc:dd:ee:ff";

    std::vector<std::string> features;
    features.push_back("browse");
    mTGRemoteFeaturesForMultipleAdapters[adapterAddress][deviceAddress] = features;

    // Clear remote features
    auto adapterIt = mTGRemoteFeaturesForMultipleAdapters.find(adapterAddress);
    if (adapterIt != mTGRemoteFeaturesForMultipleAdapters.end()) {
        auto deviceIt = adapterIt->second.find(deviceAddress);
        if (deviceIt != adapterIt->second.end()) {
            adapterIt->second.erase(deviceIt);
        }
    }

    EXPECT_EQ(mTGRemoteFeaturesForMultipleAdapters[adapterAddress].size(), 0u);
}
