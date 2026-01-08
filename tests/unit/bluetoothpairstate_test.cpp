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

// Mock global variables for glib
bool g_file_test_mock_result = true;
bool g_file_test_symlink_result = false;
std::string g_file_test_expected_path;

// Mock PmLogContext
#include "PmLogLib.h"
PmLogContext logContext = nullptr;

#include "bluetoothpairstate.h"
#include "bluetoothdevice.h"

class BluetoothPairStateTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test default constructor
TEST_F(BluetoothPairStateTest, DefaultConstructor) {
    BluetoothPairState pairState;

    EXPECT_FALSE(pairState.isPairable());
    EXPECT_FALSE(pairState.isPairing());
    EXPECT_EQ(pairState.getPairableTimeout(), 30u);
    EXPECT_EQ(pairState.getDevice(), nullptr);
    EXPECT_FALSE(pairState.isIncoming());
    EXPECT_TRUE(pairState.isOutgoing()); // Default is outgoing (not incoming)
}

// Test destructor
TEST_F(BluetoothPairStateTest, Destructor) {
    BluetoothPairState* pairState = new BluetoothPairState();
    delete pairState;
    // If we reach here without crash, the destructor works
    SUCCEED();
}

// Test isPairable
TEST_F(BluetoothPairStateTest, IsPairableDefault) {
    BluetoothPairState pairState;
    EXPECT_FALSE(pairState.isPairable());
}

TEST_F(BluetoothPairStateTest, IsPairableAfterSet) {
    BluetoothPairState pairState;
    pairState.setPairable(true);
    EXPECT_TRUE(pairState.isPairable());
}

TEST_F(BluetoothPairStateTest, IsPairableToggle) {
    BluetoothPairState pairState;
    pairState.setPairable(true);
    EXPECT_TRUE(pairState.isPairable());
    pairState.setPairable(false);
    EXPECT_FALSE(pairState.isPairable());
}

// Test getPairableTimeout
TEST_F(BluetoothPairStateTest, GetPairableTimeoutDefault) {
    BluetoothPairState pairState;
    EXPECT_EQ(pairState.getPairableTimeout(), 30u);
}

TEST_F(BluetoothPairStateTest, GetPairableTimeoutAfterSet) {
    BluetoothPairState pairState;
    pairState.setPairableTimeout(60);
    EXPECT_EQ(pairState.getPairableTimeout(), 60u);
}

TEST_F(BluetoothPairStateTest, GetPairableTimeoutZero) {
    BluetoothPairState pairState;
    pairState.setPairableTimeout(0);
    EXPECT_EQ(pairState.getPairableTimeout(), 0u);
}

TEST_F(BluetoothPairStateTest, GetPairableTimeoutMax) {
    BluetoothPairState pairState;
    pairState.setPairableTimeout(UINT32_MAX);
    EXPECT_EQ(pairState.getPairableTimeout(), UINT32_MAX);
}

// Test isPairing
TEST_F(BluetoothPairStateTest, IsPairingDefault) {
    BluetoothPairState pairState;
    EXPECT_FALSE(pairState.isPairing());
}

// Test isIncoming and isOutgoing
TEST_F(BluetoothPairStateTest, IsIncomingDefault) {
    BluetoothPairState pairState;
    EXPECT_FALSE(pairState.isIncoming());
}

TEST_F(BluetoothPairStateTest, IsOutgoingDefault) {
    BluetoothPairState pairState;
    EXPECT_TRUE(pairState.isOutgoing());
}

TEST_F(BluetoothPairStateTest, MarkAsIncoming) {
    BluetoothPairState pairState;
    pairState.markAsIncoming();
    EXPECT_TRUE(pairState.isIncoming());
    EXPECT_FALSE(pairState.isOutgoing());
}

TEST_F(BluetoothPairStateTest, MarkAsOutgoing) {
    BluetoothPairState pairState;
    pairState.markAsIncoming();
    pairState.markAsOutgoing();
    EXPECT_FALSE(pairState.isIncoming());
    EXPECT_TRUE(pairState.isOutgoing());
}

TEST_F(BluetoothPairStateTest, ToggleIncomingOutgoing) {
    BluetoothPairState pairState;

    pairState.markAsIncoming();
    EXPECT_TRUE(pairState.isIncoming());

    pairState.markAsOutgoing();
    EXPECT_TRUE(pairState.isOutgoing());

    pairState.markAsIncoming();
    EXPECT_TRUE(pairState.isIncoming());
}

// Test startPairing with nullptr device
TEST_F(BluetoothPairStateTest, StartPairingWithNullDevice) {
    BluetoothPairState pairState;
    pairState.startPairing(nullptr);

    EXPECT_TRUE(pairState.isPairing());
    EXPECT_EQ(pairState.getDevice(), nullptr);
}

// Test startPairing with valid device
TEST_F(BluetoothPairStateTest, StartPairingWithValidDevice) {
    BluetoothPairState pairState;
    BluetoothDevice device;

    pairState.startPairing(&device);

    EXPECT_TRUE(pairState.isPairing());
    EXPECT_EQ(pairState.getDevice(), &device);
    EXPECT_TRUE(device.getPairing());
}

// Test stopPairing with nullptr device
TEST_F(BluetoothPairStateTest, StopPairingWithNullDevice) {
    BluetoothPairState pairState;
    pairState.startPairing(nullptr);
    pairState.stopPairing();

    EXPECT_FALSE(pairState.isPairing());
    EXPECT_EQ(pairState.getDevice(), nullptr);
}

// Test stopPairing with valid device
TEST_F(BluetoothPairStateTest, StopPairingWithValidDevice) {
    BluetoothPairState pairState;
    BluetoothDevice device;

    pairState.startPairing(&device);
    EXPECT_TRUE(device.getPairing());

    pairState.stopPairing();

    EXPECT_FALSE(pairState.isPairing());
    EXPECT_EQ(pairState.getDevice(), nullptr);
    EXPECT_FALSE(device.getPairing());
}

// Test complete pairing workflow
TEST_F(BluetoothPairStateTest, CompletePairingWorkflow) {
    BluetoothPairState pairState;
    BluetoothDevice device;

    // Initial state
    EXPECT_FALSE(pairState.isPairing());
    EXPECT_FALSE(device.getPairing());

    // Start outgoing pairing
    pairState.markAsOutgoing();
    pairState.startPairing(&device);

    EXPECT_TRUE(pairState.isPairing());
    EXPECT_TRUE(pairState.isOutgoing());
    EXPECT_TRUE(device.getPairing());
    EXPECT_EQ(pairState.getDevice(), &device);

    // Stop pairing
    pairState.stopPairing();

    EXPECT_FALSE(pairState.isPairing());
    EXPECT_FALSE(device.getPairing());
    EXPECT_EQ(pairState.getDevice(), nullptr);
}

// Test incoming pairing workflow
TEST_F(BluetoothPairStateTest, IncomingPairingWorkflow) {
    BluetoothPairState pairState;
    BluetoothDevice device;

    // Start incoming pairing
    pairState.markAsIncoming();
    pairState.startPairing(&device);

    EXPECT_TRUE(pairState.isPairing());
    EXPECT_TRUE(pairState.isIncoming());
    EXPECT_FALSE(pairState.isOutgoing());
    EXPECT_TRUE(device.getPairing());
    EXPECT_EQ(pairState.getDevice(), &device);

    // Stop pairing - incoming should be reset to false
    pairState.stopPairing();

    EXPECT_FALSE(pairState.isPairing());
    EXPECT_FALSE(pairState.isIncoming());
    EXPECT_TRUE(pairState.isOutgoing()); // Resets to outgoing after stop
}

// Test setPairable method
TEST_F(BluetoothPairStateTest, SetPairableTrue) {
    BluetoothPairState pairState;
    pairState.setPairable(true);
    EXPECT_TRUE(pairState.isPairable());
}

TEST_F(BluetoothPairStateTest, SetPairableFalse) {
    BluetoothPairState pairState;
    pairState.setPairable(true);
    pairState.setPairable(false);
    EXPECT_FALSE(pairState.isPairable());
}

// Test setPairableTimeout method
TEST_F(BluetoothPairStateTest, SetPairableTimeout) {
    BluetoothPairState pairState;

    pairState.setPairableTimeout(120);
    EXPECT_EQ(pairState.getPairableTimeout(), 120u);

    pairState.setPairableTimeout(0);
    EXPECT_EQ(pairState.getPairableTimeout(), 0u);

    pairState.setPairableTimeout(300);
    EXPECT_EQ(pairState.getPairableTimeout(), 300u);
}

// Test getDevice method
TEST_F(BluetoothPairStateTest, GetDeviceBeforePairing) {
    BluetoothPairState pairState;
    EXPECT_EQ(pairState.getDevice(), nullptr);
}

TEST_F(BluetoothPairStateTest, GetDeviceDuringPairing) {
    BluetoothPairState pairState;
    BluetoothDevice device;

    pairState.startPairing(&device);
    EXPECT_EQ(pairState.getDevice(), &device);
}

TEST_F(BluetoothPairStateTest, GetDeviceAfterPairing) {
    BluetoothPairState pairState;
    BluetoothDevice device;

    pairState.startPairing(&device);
    pairState.stopPairing();
    EXPECT_EQ(pairState.getDevice(), nullptr);
}

// Test edge case: calling stopPairing when not pairing
TEST_F(BluetoothPairStateTest, StopPairingWhenNotPairing) {
    BluetoothPairState pairState;

    // Should not crash
    pairState.stopPairing();

    EXPECT_FALSE(pairState.isPairing());
    EXPECT_EQ(pairState.getDevice(), nullptr);
}

// Test edge case: calling startPairing multiple times
TEST_F(BluetoothPairStateTest, StartPairingMultipleTimes) {
    BluetoothPairState pairState;
    BluetoothDevice device1;
    BluetoothDevice device2;

    pairState.startPairing(&device1);
    EXPECT_EQ(pairState.getDevice(), &device1);
    EXPECT_TRUE(device1.getPairing());

    // Start pairing with another device (overwrites)
    pairState.startPairing(&device2);
    EXPECT_EQ(pairState.getDevice(), &device2);
    EXPECT_TRUE(device2.getPairing());
    // Note: device1's pairing status is not reset by this
}

// Test all state combinations
TEST_F(BluetoothPairStateTest, StateTransitions) {
    BluetoothPairState pairState;

    // Initial state
    EXPECT_FALSE(pairState.isPairable());
    EXPECT_FALSE(pairState.isPairing());
    EXPECT_FALSE(pairState.isIncoming());
    EXPECT_TRUE(pairState.isOutgoing());

    // Set pairable
    pairState.setPairable(true);
    EXPECT_TRUE(pairState.isPairable());

    // Mark as incoming
    pairState.markAsIncoming();
    EXPECT_TRUE(pairState.isIncoming());

    // Start pairing
    BluetoothDevice device;
    pairState.startPairing(&device);
    EXPECT_TRUE(pairState.isPairing());

    // Stop pairing
    pairState.stopPairing();
    EXPECT_FALSE(pairState.isPairing());
    EXPECT_FALSE(pairState.isIncoming());

    // Pairable state should remain unchanged
    EXPECT_TRUE(pairState.isPairable());
}

// Test with pairable timeout variations
TEST_F(BluetoothPairStateTest, PairableTimeoutVariations) {
    BluetoothPairState pairState;

    // Test various timeout values
    uint32_t testValues[] = {0, 1, 30, 60, 120, 300, 600, 1800, 3600};

    for (uint32_t value : testValues) {
        pairState.setPairableTimeout(value);
        EXPECT_EQ(pairState.getPairableTimeout(), value);
    }
}
