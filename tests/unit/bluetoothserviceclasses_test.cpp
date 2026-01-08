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

// Mock PmLogContext
#include "PmLogLib.h"
PmLogContext logContext = nullptr;

#include "bluetoothserviceclasses.h"

class BluetoothServiceClassesTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test BluetoothServiceClassInfo constructor with only mnemonic
TEST_F(BluetoothServiceClassesTest, ConstructorWithMnemonicOnly) {
    BluetoothServiceClassInfo info("TestProfile");

    EXPECT_EQ(info.getMnemonic(), "TestProfile");
    EXPECT_EQ(info.getMethodCategory(), "");
    EXPECT_FALSE(info.isSupported());
}

// Test BluetoothServiceClassInfo constructor with mnemonic and category
TEST_F(BluetoothServiceClassesTest, ConstructorWithMnemonicAndCategory) {
    BluetoothServiceClassInfo info("TestProfile", "/test");

    EXPECT_EQ(info.getMnemonic(), "TestProfile");
    EXPECT_EQ(info.getMethodCategory(), "/test");
    EXPECT_TRUE(info.isSupported());
}

// Test copy constructor
TEST_F(BluetoothServiceClassesTest, CopyConstructor) {
    BluetoothServiceClassInfo original("A2DP", "/a2dp");
    BluetoothServiceClassInfo copy(original);

    EXPECT_EQ(copy.getMnemonic(), "A2DP");
    EXPECT_EQ(copy.getMethodCategory(), "/a2dp");
    EXPECT_TRUE(copy.isSupported());
}

// Test isSupported with empty category
TEST_F(BluetoothServiceClassesTest, IsSupportedWithEmptyCategory) {
    BluetoothServiceClassInfo info("HSP-HS", "");
    EXPECT_FALSE(info.isSupported());
}

// Test isSupported with non-empty category
TEST_F(BluetoothServiceClassesTest, IsSupportedWithCategory) {
    BluetoothServiceClassInfo info("HFP-HF", "/hfp");
    EXPECT_TRUE(info.isSupported());
}

// Test allServiceClasses map contains expected entries
TEST_F(BluetoothServiceClassesTest, AllServiceClassesContainsHFP) {
    auto it = allServiceClasses.find("0000111e-0000-1000-8000-00805f9b34fb");
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "HFP-HF");
    EXPECT_EQ(it->second.getMethodCategory(), "/hfp");
}

TEST_F(BluetoothServiceClassesTest, AllServiceClassesContainsA2DP) {
    auto it = allServiceClasses.find("0000110a-0000-1000-8000-00805f9b34fb");
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "A2DP-source");
    EXPECT_EQ(it->second.getMethodCategory(), "/a2dp");
}

TEST_F(BluetoothServiceClassesTest, AllServiceClassesContainsAVRCP) {
    auto it = allServiceClasses.find("0000110e-0000-1000-8000-00805f9b34fb");
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "AVRCP-remote");
    EXPECT_EQ(it->second.getMethodCategory(), "/avrcp");
}

TEST_F(BluetoothServiceClassesTest, AllServiceClassesContainsGATT) {
    auto it = allServiceClasses.find("00001801-0000-1000-8000-00805f9b34fb");
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "GATT");
    EXPECT_EQ(it->second.getMethodCategory(), "/gatt");
}

TEST_F(BluetoothServiceClassesTest, AllServiceClassesContainsSPP) {
    auto it = allServiceClasses.find("00001101-0000-1000-8000-00805f9b34fb");
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "SPP");
    EXPECT_EQ(it->second.getMethodCategory(), "/spp");
}

TEST_F(BluetoothServiceClassesTest, AllServiceClassesContainsPBAP) {
    auto it = allServiceClasses.find("00001130-0000-1000-8000-00805f9b34fb");
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "PBAP");
    EXPECT_EQ(it->second.getMethodCategory(), "/pbap");
}

TEST_F(BluetoothServiceClassesTest, AllServiceClassesContainsOPP) {
    auto it = allServiceClasses.find("00001105-0000-1000-8000-00805f9b34fb");
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "OPP");
    EXPECT_EQ(it->second.getMethodCategory(), "/opp");
}

TEST_F(BluetoothServiceClassesTest, AllServiceClassesContainsFTP) {
    auto it = allServiceClasses.find("00001106-0000-1000-8000-00805f9b34fb");
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "FTP");
    EXPECT_EQ(it->second.getMethodCategory(), "/ftp");
}

TEST_F(BluetoothServiceClassesTest, AllServiceClassesContainsMAP) {
    auto it = allServiceClasses.find("00001134-0000-1000-8000-00805f9b34fb");
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "MAP");
    EXPECT_EQ(it->second.getMethodCategory(), "/map");
}

TEST_F(BluetoothServiceClassesTest, AllServiceClassesContainsMESH) {
    auto it = allServiceClasses.find("00001827-0000-1000-8000-00805f9b34fb");
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "MESH");
    EXPECT_EQ(it->second.getMethodCategory(), "/mesh");
}

TEST_F(BluetoothServiceClassesTest, AllServiceClassesContainsPAN) {
    auto it = allServiceClasses.find("00001115-0000-1000-8000-00805f9b34fb");
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "PANU");
    EXPECT_EQ(it->second.getMethodCategory(), "/pan");
}

TEST_F(BluetoothServiceClassesTest, AllServiceClassesContainsHID) {
    auto it = allServiceClasses.find("00000011-0000-1000-8000-00805f9b34fb");
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "HID-host");
    EXPECT_EQ(it->second.getMethodCategory(), "/hid");
}

// Test looking up non-existent UUID
TEST_F(BluetoothServiceClassesTest, AllServiceClassesNotFound) {
    auto it = allServiceClasses.find("00000000-0000-0000-0000-000000000000");
    EXPECT_EQ(it, allServiceClasses.end());
}

// Test that unsupported services have empty category
TEST_F(BluetoothServiceClassesTest, UnsupportedServiceHasEmptyCategory) {
    auto it = allServiceClasses.find("00001203-0000-1000-8000-00805f9b34fb"); // GAVDP
    ASSERT_NE(it, allServiceClasses.end());
    EXPECT_EQ(it->second.getMnemonic(), "GAVDP");
    EXPECT_EQ(it->second.getMethodCategory(), "");
    // Note: Can't call isSupported() directly on const reference from map
    // Create a copy to test isSupported()
    BluetoothServiceClassInfo copy(it->second);
    EXPECT_FALSE(copy.isSupported());
}
