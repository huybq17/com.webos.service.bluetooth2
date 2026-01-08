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
#include <cstdlib>

// Mock global variables for glib
bool g_file_test_mock_result = true;
bool g_file_test_symlink_result = false;
std::string g_file_test_expected_path;

// Mock PmLogContext
#include "PmLogLib.h"
PmLogContext logContext = nullptr;

#include "bluetooth-sil-api.h"
#include "bluetoothsilfactory.h"

class BluetoothSILFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clear any environment variables
        unsetenv("WEBOS_BLUETOOTH_SIL");
        unsetenv("WEBOS_BLUETOOTH_SIL_BASE_PATH");
    }

    void TearDown() override {
        // Clean up environment
        unsetenv("WEBOS_BLUETOOTH_SIL");
        unsetenv("WEBOS_BLUETOOTH_SIL_BASE_PATH");
        // Free SIL handle if it was created
        BluetoothSILFactory::freeSILHandle();
    }
};

// Test create with non-existent SIL
TEST_F(BluetoothSILFactoryTest, CreateWithNonExistentSIL) {
    // The default "mock" SIL won't exist in the test environment
    BluetoothSIL* sil = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);

    // Should return null since the SIL file doesn't exist
    EXPECT_EQ(sil, nullptr);
}

// Test create with environment variable override for SIL name
TEST_F(BluetoothSILFactoryTest, CreateWithEnvOverrideSIL) {
    setenv("WEBOS_BLUETOOTH_SIL", "nonexistent_sil", 1);

    BluetoothSIL* sil = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);

    // Should return null since the SIL file doesn't exist
    EXPECT_EQ(sil, nullptr);
}

// Test create with environment variable override for base path
TEST_F(BluetoothSILFactoryTest, CreateWithEnvOverrideBasePath) {
    setenv("WEBOS_BLUETOOTH_SIL_BASE_PATH", "/tmp/nonexistent", 1);

    BluetoothSIL* sil = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);

    // Should return null since the SIL file doesn't exist
    EXPECT_EQ(sil, nullptr);
}

// Test create with both environment variables overridden
TEST_F(BluetoothSILFactoryTest, CreateWithBothEnvOverrides) {
    setenv("WEBOS_BLUETOOTH_SIL", "test_sil", 1);
    setenv("WEBOS_BLUETOOTH_SIL_BASE_PATH", "/tmp/test", 1);

    BluetoothSIL* sil = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);

    // Should return null since the SIL file doesn't exist
    EXPECT_EQ(sil, nullptr);
}

// Test freeSILHandle when no handle exists
TEST_F(BluetoothSILFactoryTest, FreeSILHandleWhenNoHandle) {
    // Should not crash
    BluetoothSILFactory::freeSILHandle();
    SUCCEED();
}

// Test create with different IO capabilities
TEST_F(BluetoothSILFactoryTest, CreateWithDisplayYesNo) {
    BluetoothSIL* sil = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_YES_NO);
    EXPECT_EQ(sil, nullptr);
}

TEST_F(BluetoothSILFactoryTest, CreateWithDisplayOnly) {
    BluetoothSIL* sil = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_ONLY);
    EXPECT_EQ(sil, nullptr);
}

TEST_F(BluetoothSILFactoryTest, CreateWithKeyboardDisplay) {
    BluetoothSIL* sil = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_DISPLAY);
    EXPECT_EQ(sil, nullptr);
}

TEST_F(BluetoothSILFactoryTest, CreateWithKeyboardOnly) {
    BluetoothSIL* sil = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_ONLY);
    EXPECT_EQ(sil, nullptr);
}

// Test create with different API versions
TEST_F(BluetoothSILFactoryTest, CreateWithVersion0) {
    BluetoothSIL* sil = BluetoothSILFactory::create(0, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    EXPECT_EQ(sil, nullptr);
}

TEST_F(BluetoothSILFactoryTest, CreateWithVersion2) {
    BluetoothSIL* sil = BluetoothSILFactory::create(2, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    EXPECT_EQ(sil, nullptr);
}

TEST_F(BluetoothSILFactoryTest, CreateWithHighVersion) {
    BluetoothSIL* sil = BluetoothSILFactory::create(100, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    EXPECT_EQ(sil, nullptr);
}

// Test multiple create calls
TEST_F(BluetoothSILFactoryTest, MultipleCreateCalls) {
    BluetoothSIL* sil1 = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    EXPECT_EQ(sil1, nullptr);

    BluetoothSIL* sil2 = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    EXPECT_EQ(sil2, nullptr);
}

// Test freeSILHandle multiple times
TEST_F(BluetoothSILFactoryTest, FreeSILHandleMultipleTimes) {
    BluetoothSILFactory::freeSILHandle();
    BluetoothSILFactory::freeSILHandle();
    BluetoothSILFactory::freeSILHandle();
    SUCCEED();
}

// Test create after freeSILHandle
TEST_F(BluetoothSILFactoryTest, CreateAfterFreeSILHandle) {
    BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    BluetoothSILFactory::freeSILHandle();

    BluetoothSIL* sil = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    EXPECT_EQ(sil, nullptr);
}

// Test with SIL name containing spaces (from config macro)
TEST_F(BluetoothSILFactoryTest, CreateWithSpaceSeparatedSILNames) {
    // The WEBOS_BLUETOOTH_SIL macro might contain space-separated names
    // In this case, only the first name should be used
    setenv("WEBOS_BLUETOOTH_SIL", "first second third", 1);

    BluetoothSIL* sil = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    EXPECT_EQ(sil, nullptr);
}

// Test with empty SIL name
TEST_F(BluetoothSILFactoryTest, CreateWithEmptySILName) {
    setenv("WEBOS_BLUETOOTH_SIL", "", 1);

    // Empty name will be used, should still fail gracefully
    BluetoothSIL* sil = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    EXPECT_EQ(sil, nullptr);
}

// Test with empty base path
TEST_F(BluetoothSILFactoryTest, CreateWithEmptyBasePath) {
    setenv("WEBOS_BLUETOOTH_SIL_BASE_PATH", "", 1);

    BluetoothSIL* sil = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    EXPECT_EQ(sil, nullptr);
}

// Test sequential create and free operations
TEST_F(BluetoothSILFactoryTest, SequentialCreateAndFree) {
    for (int i = 0; i < 5; i++) {
        BluetoothSIL* sil = BluetoothSILFactory::create(1, BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
        EXPECT_EQ(sil, nullptr);
        BluetoothSILFactory::freeSILHandle();
    }
}
