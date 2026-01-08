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

#ifndef BLUETOOTH_SIL_API_H_
#define BLUETOOTH_SIL_API_H_

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <cstdint>
#include <memory>

// Error codes
enum BluetoothError {
    BLUETOOTH_ERROR_NONE = 0,
    BLUETOOTH_ERROR_FAIL,
    BLUETOOTH_ERROR_NOT_READY,
    BLUETOOTH_ERROR_NOMEM,
    BLUETOOTH_ERROR_BUSY,
    BLUETOOTH_ERROR_UNSUPPORTED,
    BLUETOOTH_ERROR_PARAM_INVALID,
    BLUETOOTH_ERROR_UNHANDLED,
    BLUETOOTH_ERROR_UNKNOWN_DEVICE_ADDR,
    BLUETOOTH_ERROR_AUTHENTICATION_CANCELED,
    BLUETOOTH_ERROR_AUTHENTICATION_FAILED,
    BLUETOOTH_ERROR_AUTHENTICATION_REJECTED,
    BLUETOOTH_ERROR_AUTHENTICATION_TIMEOUT,
    BLUETOOTH_ERROR_DEVICE_ALREADY_PAIRED,
    BLUETOOTH_ERROR_DEVICE_NOT_PAIRED,
    BLUETOOTH_ERROR_DEVICE_ALREADY_CONNECTED,
    BLUETOOTH_ERROR_DEVICE_NOT_CONNECTED,
    BLUETOOTH_ERROR_NOT_ALLOWED,
    BLUETOOTH_ERROR_ABORTED,
    BLUETOOTH_ERROR_TETHERING_ALREADY_ENABLED,
    BLUETOOTH_ERROR_TETHERING_ALREADY_DISABLED,
    BLUETOOTH_ERROR_ALREADY_CONNECTED_ONE_DEVICE,
    BLUETOOTH_ERROR_PBAP_CALL_SELECT_FOLDER_TYPE,
    BLUETOOTH_ERROR_AVRCP_ITEM_NOT_PLAYABLE,
    BLUETOOTH_ERROR_AVRCP_NOT_A_FOLDER,
    BLUETOOTH_ERROR_MAP_FOLDER_NOT_FOUND,
    BLUETOOTH_ERROR_MAP_MESSAGE_HANDLE_NOT_FOUND,
    BLUETOOTH_ERROR_MESH_NET_KEY_INDEX_DOES_NOT_EXIST,
    BLUETOOTH_ERROR_MESH_APP_KEY_INDEX_ALREADY_EXISTS,
    BLUETOOTH_ERROR_MESH_APP_KEY_INDEX_DOES_NOT_EXIST,
    BLUETOOTH_ERROR_MESH_NO_RESPONSE,
    BLUETOOTH_ERROR_MESH_BAD_PDU,
    BLUETOOTH_ERROR_MESH_PROV_CONFIRMATION_FAILED,
    BLUETOOTH_ERROR_MESH_CANNOT_ASSIGN_ADDRESS,
    BLUETOOTH_ERROR_MESH_DEVICE_NOT_FOUND,
    BLUETOOTH_ERROR_MESH_NODE_NOT_AVAILABLE,
    BLUETOOTH_ERROR_MESH_INVALID_DEST_ADDRESS,
    BLUETOOTH_ERROR_MESH_NETWORK_EXISTS,
    BLUETOOTH_ERROR_MESH_CANNOT_UPDATE_APPKEY,
    BLUETOOTH_ERROR_MESH_NO_RESPONSE_FROM_NODE,
    BLUETOOTH_ERROR_MESH_INVALID_COMMAND,
    BLUETOOTH_ERROR_MESH_TIMEOUT,
    BLUETOOTH_ERROR_MESH_CONFIRMATION_FAILED,
    BLUETOOTH_ERROR_MESH_OUT_OF_RESOURCES,
    BLUETOOTH_ERROR_MESH_DECRYPTION_ERROR,
    BLUETOOTH_ERROR_MESH_UNEXPECTED_ERROR,
    BLUETOOTH_ERROR_MESH_CANNOT_ASSIGN_ADDRESSES,
    BLUETOOTH_ERROR_MESH_NETKEY_UPDATE_FAILED
};

// Device types
enum BluetoothDeviceType {
    BLUETOOTH_DEVICE_TYPE_UNKNOWN = 0,
    BLUETOOTH_DEVICE_TYPE_BREDR,
    BLUETOOTH_DEVICE_TYPE_BLE,
    BLUETOOTH_DEVICE_TYPE_DUAL
};

// Device roles
enum BluetoothDeviceRole {
    BLUETOOTH_DEVICE_ROLE = 0,
    BLUETOOTH_DEVICE_ROLE_HFP_HF = 1 << 0,
    BLUETOOTH_DEVICE_ROLE_HFP_AG = 1 << 1,
    BLUETOOTH_DEVICE_ROLE_A2DP_SRC = 1 << 2,
    BLUETOOTH_DEVICE_ROLE_A2DP_SINK = 1 << 3,
    BLUETOOTH_DEVICE_ROLE_AVRCP_RMT = 1 << 4,
    BLUETOOTH_DEVICE_ROLE_AVRCP_TGT = 1 << 5
};

// Inquiry access codes
enum class InquiryAccessCode {
    BT_ACCESS_CODE_NONE = 0,
    BT_ACCESS_CODE_GIAC = 1,
    BT_ACCESS_CODE_LIAC = 2
};

// Pairing IO capabilities
enum BluetoothPairingIOCapability {
    BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT = 0,
    BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_YES_NO,
    BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_ONLY,
    BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_DISPLAY,
    BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_ONLY
};

// Pairing secret types
enum BluetoothPairingSecretType {
    BLUETOOTH_PAIRING_SECRET_TYPE_PASSKEY = 0,
    BLUETOOTH_PAIRING_SECRET_TYPE_PIN
};

// Passkey type
typedef uint32_t BluetoothPasskey;

// Link key type
typedef std::vector<int32_t> BluetoothLinkKey;

// Low energy data types
typedef std::vector<uint8_t> BluetoothLowEnergyData;
typedef std::vector<uint8_t> BluetoothLowEnergyMask;

// Bluetooth Property
class BluetoothProperty {
public:
    enum class Type {
        NAME,
        BDADDR,
        UUIDS,
        CLASS_OF_DEVICE,
        TYPE_OF_DEVICE,
        PAIRED,
        CONNECTED,
        TRUSTED,
        BLOCKED,
        RSSI,
        ROLE,
        MANUFACTURER_DATA,
        INQUIRY_ACCESS_CODE,
        SCAN_RECORD,
        ALIAS,
        INTERFACE_NAME,
        STACK_NAME,
        STACK_VERSION,
        FIRMWARE_VERSION,
        DISCOVERY_TIMEOUT,
        DISCOVERABLE,
        DISCOVERABLE_TIMEOUT,
        PAIRABLE,
        PAIRABLE_TIMEOUT,
        MAP_INSTANCES_NAME,
        MAP_SUPPORTED_MESSAGE_TYPE
    };

    BluetoothProperty() : mType(Type::NAME) {}
    BluetoothProperty(Type type) : mType(type) {}

    template<typename T>
    BluetoothProperty(Type type, T value) : mType(type) {
        setValue(value);
    }

    Type getType() const { return mType; }

    template<typename T>
    T getValue() const;

    template<typename T>
    void setValue(T value);

private:
    Type mType;
    std::string mStringValue;
    uint32_t mUint32Value = 0;
    int32_t mInt32Value = 0;
    bool mBoolValue = false;
    std::vector<std::string> mStringVectorValue;
    std::vector<uint8_t> mUint8VectorValue;
    std::map<std::string, std::vector<std::string>> mMapValue;
};

// Template specializations
template<>
inline std::string BluetoothProperty::getValue<std::string>() const {
    return mStringValue;
}

template<>
inline uint32_t BluetoothProperty::getValue<uint32_t>() const {
    return mUint32Value;
}

template<>
inline int BluetoothProperty::getValue<int>() const {
    return mInt32Value;
}

template<>
inline bool BluetoothProperty::getValue<bool>() const {
    return mBoolValue;
}

template<>
inline std::vector<std::string> BluetoothProperty::getValue<std::vector<std::string>>() const {
    return mStringVectorValue;
}

template<>
inline std::vector<uint8_t> BluetoothProperty::getValue<std::vector<uint8_t>>() const {
    return mUint8VectorValue;
}

template<>
inline std::map<std::string, std::vector<std::string>> BluetoothProperty::getValue<std::map<std::string, std::vector<std::string>>>() const {
    return mMapValue;
}

template<>
inline void BluetoothProperty::setValue<std::string>(std::string value) {
    mStringValue = value;
}

template<>
inline void BluetoothProperty::setValue<uint32_t>(uint32_t value) {
    mUint32Value = value;
}

template<>
inline void BluetoothProperty::setValue<bool>(bool value) {
    mBoolValue = value;
}

template<>
inline void BluetoothProperty::setValue<int>(int value) {
    mInt32Value = value;
}

template<>
inline void BluetoothProperty::setValue<std::vector<std::string>>(std::vector<std::string> value) {
    mStringVectorValue = value;
}

template<>
inline void BluetoothProperty::setValue<std::vector<uint8_t>>(std::vector<uint8_t> value) {
    mUint8VectorValue = value;
}

template<>
inline void BluetoothProperty::setValue<std::map<std::string, std::vector<std::string>>>(std::map<std::string, std::vector<std::string>> value) {
    mMapValue = value;
}

// Properties list
typedef std::vector<BluetoothProperty> BluetoothPropertiesList;

// Callback types
typedef std::function<void(BluetoothError)> BluetoothResultCallback;
typedef std::function<void(BluetoothError, const BluetoothProperty&)> BluetoothPropertyResultCallback;

// Profile status observer interface
class BluetoothProfileStatusObserver {
public:
    virtual ~BluetoothProfileStatusObserver() = default;
    virtual void propertiesChanged(const std::string &address, BluetoothPropertiesList properties) {}
    virtual void propertiesChanged(const std::string &adapterAddress, const std::string &address, BluetoothPropertiesList properties) {}
};

// Adapter status observer interface
class BluetoothAdapterStatusObserver {
public:
    virtual ~BluetoothAdapterStatusObserver() = default;
    virtual void adapterStateChanged(bool powered) {}
    virtual void adapterHciTimeoutOccurred() {}
    virtual void discoveryStateChanged(bool active) {}
    virtual void adapterPropertiesChanged(BluetoothPropertiesList properties) {}
    virtual void adapterKeepAliveStateChanged(bool enabled) {}
    virtual void deviceFound(BluetoothPropertiesList properties) {}
    virtual void deviceFound(const std::string &address, BluetoothPropertiesList properties) {}
    virtual void deviceRemoved(const std::string &address) {}
    virtual void devicePropertiesChanged(const std::string &address, BluetoothPropertiesList properties) {}
    virtual void leDeviceFound(const std::string &address, BluetoothPropertiesList properties) {}
    virtual void leDeviceRemoved(const std::string &address) {}
    virtual void leDevicePropertiesChanged(const std::string &address, BluetoothPropertiesList properties) {}
    virtual void leDeviceFoundByScanId(uint32_t scanId, BluetoothPropertiesList properties) {}
    virtual void leDeviceRemovedByScanId(uint32_t scanId, const std::string &address) {}
    virtual void leDevicePropertiesChangedByScanId(uint32_t scanId, const std::string &address, BluetoothPropertiesList properties) {}
    virtual void deviceLinkKeyCreated(const std::string &address, BluetoothLinkKey linkKey) {}
    virtual void deviceLinkKeyDestroyed(const std::string &address, BluetoothLinkKey linkKey) {}
    virtual void requestPairingSecret(const std::string &address, BluetoothPairingSecretType type) {}
    virtual void displayPairingSecret(const std::string &address, const std::string &pin) {}
    virtual void displayPairingSecret(const std::string &address, BluetoothPasskey passkey) {}
    virtual void displayPairingConfirmation(const std::string &address, BluetoothPasskey passkey) {}
    virtual void pairingCanceled() {}
    virtual void leConnectionRequest(const std::string &address, bool state) {}
};

// LE Discovery filter classes
class BluetoothLeServiceUuid {
public:
    void setUuid(const std::string &uuid) { mUuid = uuid; }
    void setMask(const std::string &mask) { mMask = mask; }
    std::string getUuid() const { return mUuid; }
    std::string getMask() const { return mMask; }
private:
    std::string mUuid;
    std::string mMask;
};

class BluetoothLeServiceData {
public:
    void setUuid(const std::string &uuid) { mUuid = uuid; }
    void setData(const BluetoothLowEnergyData &data) { mData = data; }
    void setMask(const BluetoothLowEnergyMask &mask) { mMask = mask; }
private:
    std::string mUuid;
    BluetoothLowEnergyData mData;
    BluetoothLowEnergyMask mMask;
};

class BluetoothManufacturerData {
public:
    void setId(int32_t id) { mId = id; }
    void setData(const BluetoothLowEnergyData &data) { mData = data; }
    void setMask(const BluetoothLowEnergyMask &mask) { mMask = mask; }
private:
    int32_t mId = 0;
    BluetoothLowEnergyData mData;
    BluetoothLowEnergyMask mMask;
};

class BluetoothLeDiscoveryFilter {
public:
    void setAddress(const std::string &address) { mAddress = address; }
    void setName(const std::string &name) { mName = name; }
    void setServiceUuid(const BluetoothLeServiceUuid &serviceUuid) { mServiceUuid = serviceUuid; }
    void setServiceData(const BluetoothLeServiceData &serviceData) { mServiceData = serviceData; }
    void setManufacturerData(const BluetoothManufacturerData &manufacturerData) { mManufacturerData = manufacturerData; }
private:
    std::string mAddress;
    std::string mName;
    BluetoothLeServiceUuid mServiceUuid;
    BluetoothLeServiceData mServiceData;
    BluetoothManufacturerData mManufacturerData;
};

// BluetoothProfile base class
class BluetoothProfile {
public:
    virtual ~BluetoothProfile() = default;
    virtual void registerObserver(BluetoothProfileStatusObserver *observer) {}
    virtual void connect(const std::string &address, BluetoothResultCallback callback) {}
    virtual void disconnect(const std::string &address, BluetoothResultCallback callback) {}
    virtual void getProperty(const std::string &address, BluetoothProperty::Type type, BluetoothPropertyResultCallback callback) {}
    virtual void enable(const std::string &uuid, BluetoothResultCallback callback) {}
    virtual void disable(const std::string &uuid, BluetoothResultCallback callback) {}
};

// A2DP Profile
class BluetoothA2dpProfile : public BluetoothProfile {
public:
    virtual ~BluetoothA2dpProfile() = default;
};

// Bluetooth Adapter interface
class BluetoothAdapter {
public:
    virtual ~BluetoothAdapter() = default;
    virtual BluetoothError enable() { return BLUETOOTH_ERROR_NONE; }
    virtual BluetoothError disable() { return BLUETOOTH_ERROR_NONE; }
    virtual BluetoothError startDiscovery() { return BLUETOOTH_ERROR_NONE; }
    virtual void cancelDiscovery(BluetoothResultCallback callback) {}
    virtual BluetoothError startLeDiscovery() { return BLUETOOTH_ERROR_NONE; }
    virtual void cancelLeDiscovery() {}
    virtual int32_t addLeDiscoveryFilter(const BluetoothLeDiscoveryFilter &filter) { return 1; }
    virtual void removeLeDiscoveryFilter(uint32_t scanId) {}
    virtual void matchLeDiscoveryFilterDevices(const BluetoothLeDiscoveryFilter &filter, uint32_t scanId) {}
    virtual void setAdapterProperties(const BluetoothPropertiesList &properties, BluetoothResultCallback callback) {}
    virtual void setDeviceProperties(const std::string &address, const BluetoothPropertiesList &properties, BluetoothResultCallback callback) {}
    virtual void pair(const std::string &address, BluetoothResultCallback callback) {}
    virtual void cancelPairing(const std::string &address, BluetoothResultCallback callback) {}
    virtual void unpair(const std::string &address, BluetoothResultCallback callback) {}
    virtual BluetoothError supplyPairingSecret(const std::string &address, BluetoothPasskey passkey) { return BLUETOOTH_ERROR_NONE; }
    virtual BluetoothError supplyPairingSecret(const std::string &address, const std::string &pin) { return BLUETOOTH_ERROR_NONE; }
    virtual BluetoothError supplyPairingConfirmation(const std::string &address, bool accept) { return BLUETOOTH_ERROR_NONE; }
    virtual BluetoothProfile* getProfile(const std::string &name) { return nullptr; }
    virtual void registerObserver(BluetoothAdapterStatusObserver *observer) {}
};

// Bluetooth SIL interface
class BluetoothSIL {
public:
    virtual ~BluetoothSIL() = default;
    virtual std::vector<BluetoothAdapter*> getAdapters() { return std::vector<BluetoothAdapter*>(); }
    virtual BluetoothAdapter* getDefaultAdapter() { return nullptr; }
};

#endif // BLUETOOTH_SIL_API_H_
