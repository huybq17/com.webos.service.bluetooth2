# Copyright (c) 2014-2024 LG Electronics, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.14)

project(bluetooth_service_tests CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent for downloading dependencies
include(FetchContent)

# Download and configure GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add mock include directory FIRST (before src)
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

# Add source directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

# Define required macros for the source code
add_definitions(-DWEBOS_BLUETOOTH_SIL="mock")
add_definitions(-DWEBOS_BLUETOOTH_SIL_BASE_PATH="/usr/lib/bluetooth-sils")
add_definitions(-DWEBOS_BLUETOOTH_ENABLED_SERVICE_CLASSES="A2DP AVRCP HFP SPP")
add_definitions(-DWEBOS_BLUETOOTH_PAIRING_IO_CAPABILITY="NoInputNoOutput")

# Test for utils
add_executable(utils_test
    unit/utils_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.cpp
)
target_link_libraries(utils_test
    gtest_main
    gmock_main
)

# Test for bluetootherrors
add_executable(bluetootherrors_test
    unit/bluetootherrors_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/bluetootherrors.cpp
)
target_link_libraries(bluetootherrors_test
    gtest_main
    gmock_main
)

# Test for bluetoothdevice
add_executable(bluetoothdevice_test
    unit/bluetoothdevice_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/bluetoothdevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.cpp
)
target_link_libraries(bluetoothdevice_test
    gtest_main
    gmock_main
)

# Test for bluetoothpairstate
add_executable(bluetoothpairstate_test
    unit/bluetoothpairstate_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/bluetoothpairstate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/bluetoothdevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.cpp
)
target_link_libraries(bluetoothpairstate_test
    gtest_main
    gmock_main
)

# Test for bluetoothsilfactory
add_executable(bluetoothsilfactory_test
    unit/bluetoothsilfactory_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/bluetoothsilfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.cpp
)
target_link_libraries(bluetoothsilfactory_test
    gtest_main
    gmock_main
    dl
)

# Test for bluetoothserviceclasses
add_executable(bluetoothserviceclasses_test
    unit/bluetoothserviceclasses_test.cpp
)
target_link_libraries(bluetoothserviceclasses_test
    gtest_main
    gmock_main
)

# Test for bluetoothprofileservice (logic tests only)
add_executable(bluetoothprofileservice_test
    unit/bluetoothprofileservice_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.cpp
)
target_link_libraries(bluetoothprofileservice_test
    gtest_main
    gmock_main
)

# Include Google Test discovery
include(GoogleTest)
gtest_discover_tests(utils_test)
gtest_discover_tests(bluetootherrors_test)
gtest_discover_tests(bluetoothdevice_test)
gtest_discover_tests(bluetoothpairstate_test)
gtest_discover_tests(bluetoothsilfactory_test)
gtest_discover_tests(bluetoothserviceclasses_test)
gtest_discover_tests(bluetoothprofileservice_test)
