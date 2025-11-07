// Device Configuration Module - Implementation
// Universal configuration with WiFi and LoRa support

#include <Preferences.h>
#include <WiFi.h>
#include "device_config.h"

// Global configuration instance
DeviceConfig deviceConfig;

// Preferences storage
Preferences preferences;

void initDeviceConfig() {
    // Initialize with defaults
    deviceConfig.deviceType = DEVICE_MILK_STATION; // Change per node type
    deviceConfig.deviceID = "MILK-" + String((uint32_t)ESP.getEfuseMac(), HEX);
    deviceConfig.deviceName = "Milk Station 1";
    deviceConfig.farmID = "FARM001";
    
    // WiFi defaults
    deviceConfig.wifiEnabled = true;
    deviceConfig.wifiSSID = "";
    deviceConfig.wifiPassword = "";
    deviceConfig.deviceManagerIP = "192.168.1.100";
    deviceConfig.deviceManagerPort = 8080;
    
    // LoRa defaults
    deviceConfig.loraEnabled = true;
    deviceConfig.loraFrequency = 915E6;
    deviceConfig.loraSyncWord = 0x12;
    deviceConfig.loraSpreadFactor = 7;
    
    // Cow-specific (not used in milk station)
    deviceConfig.cowGender = 1; // Female
    deviceConfig.milkMonitoring = false;
    
    deviceConfig.lastConfigUpdate = millis();
    deviceConfig.configured = false;
    
    Serial.println("Device configuration initialized with defaults");
}

void loadDeviceConfig() {
    preferences.begin("device", false);
    
    // Load configuration
    deviceConfig.configured = preferences.getBool("configured", false);
    
    if (deviceConfig.configured) {
        deviceConfig.deviceID = preferences.getString("deviceID", deviceConfig.deviceID);
        deviceConfig.deviceName = preferences.getString("name", deviceConfig.deviceName);
        deviceConfig.farmID = preferences.getString("farmID", deviceConfig.farmID);
        deviceConfig.deviceType = (DeviceType)preferences.getUChar("type", DEVICE_MILK_STATION);
        
        // WiFi config
        deviceConfig.wifiEnabled = preferences.getBool("wifiEnabled", true);
        deviceConfig.wifiSSID = preferences.getString("wifiSSID", "");
        deviceConfig.wifiPassword = preferences.getString("wifiPass", "");
        deviceConfig.deviceManagerIP = preferences.getString("managerIP", "192.168.1.100");
        deviceConfig.deviceManagerPort = preferences.getUShort("managerPort", 8080);
        
        // LoRa config
        deviceConfig.loraEnabled = preferences.getBool("loraEnabled", true);
        deviceConfig.loraFrequency = preferences.getLong("loraFreq", 915E6);
        deviceConfig.loraSyncWord = preferences.getUChar("loraSyncWord", 0x12);
        deviceConfig.loraSpreadFactor = preferences.getUChar("loraSF", 7);
        
        // Cow-specific
        deviceConfig.cowGender = preferences.getUChar("cowGender", 1);
        deviceConfig.milkMonitoring = preferences.getBool("milkEnabled", false);
        
        Serial.println("Configuration loaded from storage");
    } else {
        Serial.println("No saved configuration found, using defaults");
    }
    
    preferences.end();
}

void saveDeviceConfig() {
    preferences.begin("device", false);
    
    preferences.putBool("configured", true);
    preferences.putString("deviceID", deviceConfig.deviceID);
    preferences.putString("name", deviceConfig.deviceName);
    preferences.putString("farmID", deviceConfig.farmID);
    preferences.putUChar("type", deviceConfig.deviceType);
    
    // WiFi
    preferences.putBool("wifiEnabled", deviceConfig.wifiEnabled);
    preferences.putString("wifiSSID", deviceConfig.wifiSSID);
    preferences.putString("wifiPass", deviceConfig.wifiPassword);
    preferences.putString("managerIP", deviceConfig.deviceManagerIP);
    preferences.putUShort("managerPort", deviceConfig.deviceManagerPort);
    
    // LoRa
    preferences.putBool("loraEnabled", deviceConfig.loraEnabled);
    preferences.putLong("loraFreq", deviceConfig.loraFrequency);
    preferences.putUChar("loraSyncWord", deviceConfig.loraSyncWord);
    preferences.putUChar("loraSF", deviceConfig.loraSpreadFactor);
    
    // Cow-specific
    preferences.putUChar("cowGender", deviceConfig.cowGender);
    preferences.putBool("milkEnabled", deviceConfig.milkMonitoring);
    
    preferences.end();
    
    deviceConfig.configured = true;
    deviceConfig.lastConfigUpdate = millis();
    
    Serial.println("Configuration saved to storage");
}

void resetDeviceConfig() {
    preferences.begin("device", false);
    preferences.clear();
    preferences.end();
    
    Serial.println("Configuration reset to defaults");
    initDeviceConfig();
}

void printDeviceConfig() {
    Serial.println("\n========================================");
    Serial.println("      DEVICE CONFIGURATION");
    Serial.println("========================================");
    Serial.print("Device ID: "); Serial.println(deviceConfig.deviceID);
    Serial.print("Device Name: "); Serial.println(deviceConfig.deviceName);
    Serial.print("Farm ID: "); Serial.println(deviceConfig.farmID);
    Serial.print("Device Type: ");
    switch(deviceConfig.deviceType) {
        case DEVICE_COW_NODE: Serial.println("COW NODE"); break;
        case DEVICE_MILK_STATION: Serial.println("MILK STATION"); break;
        case DEVICE_GATEWAY: Serial.println("GATEWAY"); break;
        default: Serial.println("UNKNOWN");
    }
    
    Serial.println("\n--- WiFi Configuration ---");
    Serial.print("WiFi Enabled: "); Serial.println(deviceConfig.wifiEnabled ? "YES" : "NO");
    Serial.print("WiFi SSID: "); Serial.println(deviceConfig.wifiSSID);
    Serial.print("Manager IP: "); Serial.println(deviceConfig.deviceManagerIP);
    Serial.print("Manager Port: "); Serial.println(deviceConfig.deviceManagerPort);
    
    Serial.println("\n--- LoRa Configuration ---");
    Serial.print("LoRa Enabled: "); Serial.println(deviceConfig.loraEnabled ? "YES" : "NO");
    Serial.print("Frequency: "); Serial.print(deviceConfig.loraFrequency / 1E6); Serial.println(" MHz");
    Serial.print("Sync Word: 0x"); Serial.println(deviceConfig.loraSyncWord, HEX);
    Serial.print("Spread Factor: "); Serial.println(deviceConfig.loraSpreadFactor);
    
    Serial.println("========================================\n");
}

// Setters
void setDeviceID(const String &id) {
    deviceConfig.deviceID = id;
    saveDeviceConfig();
}

void setDeviceName(const String &name) {
    deviceConfig.deviceName = name;
    saveDeviceConfig();
}

void setFarmID(const String &farmID) {
    deviceConfig.farmID = farmID;
    saveDeviceConfig();
}

void setWiFiCredentials(const String &ssid, const String &password) {
    deviceConfig.wifiSSID = ssid;
    deviceConfig.wifiPassword = password;
    deviceConfig.wifiEnabled = true;
    saveDeviceConfig();
    Serial.println("WiFi credentials updated");
}

void setDeviceManager(const String &ip, uint16_t port) {
    deviceConfig.deviceManagerIP = ip;
    deviceConfig.deviceManagerPort = port;
    saveDeviceConfig();
    Serial.println("Device manager updated");
}

void setLoRaConfig(long frequency, uint8_t syncWord, uint8_t spreadFactor) {
    deviceConfig.loraFrequency = frequency;
    deviceConfig.loraSyncWord = syncWord;
    deviceConfig.loraSpreadFactor = spreadFactor;
    deviceConfig.loraEnabled = true;
    saveDeviceConfig();
    Serial.println("LoRa configuration updated");
}

void setCowGender(uint8_t gender) {
    deviceConfig.cowGender = gender;
    saveDeviceConfig();
}

void setMilkMonitoring(bool enabled) {
    deviceConfig.milkMonitoring = enabled;
    saveDeviceConfig();
}

void processConfigCommand(const String &command) {
    String cmd = command;
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "HELP") {
        Serial.println("\n========== CONFIGURATION COMMANDS ==========");
        Serial.println("SHOW           - Show current configuration");
        Serial.println("RESET          - Reset to factory defaults");
        Serial.println("SET_NAME <name>         - Set device name");
        Serial.println("SET_FARM <id>           - Set farm ID");
        Serial.println("SET_WIFI <ssid> <pass>  - Configure WiFi");
        Serial.println("SET_MANAGER <ip> <port> - Set device manager");
        Serial.println("SET_LORA <freq> <sync> <sf> - Configure LoRa");
        Serial.println("WIFI_ON/OFF    - Enable/disable WiFi");
        Serial.println("LORA_ON/OFF    - Enable/disable LoRa");
        Serial.println("SAVE           - Save current configuration");
        Serial.println("==========================================\n");
    }
    else if (cmd == "SHOW") {
        printDeviceConfig();
    }
    else if (cmd == "RESET") {
        resetDeviceConfig();
    }
    else if (cmd == "SAVE") {
        saveDeviceConfig();
    }
    else if (cmd == "WIFI_ON") {
        deviceConfig.wifiEnabled = true;
        saveDeviceConfig();
        Serial.println("WiFi enabled");
    }
    else if (cmd == "WIFI_OFF") {
        deviceConfig.wifiEnabled = false;
        saveDeviceConfig();
        Serial.println("WiFi disabled");
    }
    else if (cmd == "LORA_ON") {
        deviceConfig.loraEnabled = true;
        saveDeviceConfig();
        Serial.println("LoRa enabled");
    }
    else if (cmd == "LORA_OFF") {
        deviceConfig.loraEnabled = false;
        saveDeviceConfig();
        Serial.println("LoRa disabled");
    }
    else if (cmd.startsWith("SET_NAME ")) {
        String name = command.substring(9);
        name.trim();
        setDeviceName(name);
    }
    else if (cmd.startsWith("SET_FARM ")) {
        String farm = command.substring(9);
        farm.trim();
        setFarmID(farm);
    }
    else {
        Serial.println("Unknown command. Type HELP for available commands.");
    }
}
