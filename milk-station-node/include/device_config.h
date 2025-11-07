// Device Configuration Module - Header
// Universal configuration for all node types (cow, milk-station, gateway)

#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include <Arduino.h>

// Device Types
enum DeviceType {
    DEVICE_COW_NODE = 1,
    DEVICE_MILK_STATION = 2,
    DEVICE_GATEWAY = 3
};

// Device Configuration Structure
struct DeviceConfig {
    String deviceID;           // Unique device identifier
    DeviceType deviceType;     // Type of device
    String deviceName;         // Human-readable name
    String farmID;             // Farm identification
    
    // WiFi Configuration
    bool wifiEnabled;
    String wifiSSID;
    String wifiPassword;
    String deviceManagerIP;    // IP address of device manager
    uint16_t deviceManagerPort; // Port for device manager
    
    // LoRa Configuration
    bool loraEnabled;
    long loraFrequency;        // LoRa frequency (e.g., 915E6)
    uint8_t loraSyncWord;      // Sync word for network
    uint8_t loraSpreadFactor;  // Spreading factor (7-12)
    
    // Cow-specific config (only for cow nodes)
    uint8_t cowGender;         // 0=Male, 1=Female
    bool milkMonitoring;       // Enable milk monitoring
    
    // Timestamps
    unsigned long lastConfigUpdate;
    bool configured;
};

// Global device configuration
extern DeviceConfig deviceConfig;

// Configuration functions
void initDeviceConfig();
void loadDeviceConfig();
void saveDeviceConfig();
void resetDeviceConfig();
void printDeviceConfig();

// Configuration setters
void setDeviceID(const String &id);
void setDeviceName(const String &name);
void setFarmID(const String &farmID);
void setWiFiCredentials(const String &ssid, const String &password);
void setDeviceManager(const String &ip, uint16_t port);
void setLoRaConfig(long frequency, uint8_t syncWord, uint8_t spreadFactor);

// For cow nodes only
void setCowGender(uint8_t gender);
void setMilkMonitoring(bool enabled);

// Serial command processor
void processConfigCommand(const String &command);

#endif
