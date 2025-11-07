#!/bin/bash

# ==================== QUICK WIFI FIX SCRIPT ====================
# Specifically addresses the WiFi reconnection loop issue

echo "🔧 Quick WiFi Loop Fix"
echo "======================"
echo

echo "🎯 Problem: Gateway constantly tries to reconnect to 'SmartFarm_Demo'"
echo "📋 Issue: Network doesn't exist, causing reconnection loops"
echo "✅ Solution: Configure demo mode with AP-only operation"
echo

echo "Applying quick fix..."

# Backup current config
cp include/config.h include/config.h.backup.$(date +%s)

# Apply the fix directly
cat > include/config_fixed.h << 'EOF'
#ifndef CONFIG_H
#define CONFIG_H

// ==================== GATEWAY CONFIGURATION ====================
// Hardware: ESP32 DevKit with ILI9341 TFT Display
// Display: ILI9341 2.8" 320x240 TFT Color Screen

// WiFi Configuration - DEMO MODE (AP ONLY - NO CLIENT ATTEMPTS)
#define WIFI_SSID "DEMO_MODE_AP_ONLY"     // Special marker to skip client mode
#define WIFI_PASSWORD "smartfarm123"
#define WIFI_TIMEOUT_MS 1000              // Very short timeout

// MQTT Broker Configuration
#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "SmartFarm_Gateway"
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""

// MQTT Topics
#define TOPIC_COW_DATA "smartfarm/cow/data"
#define TOPIC_MILK_DATA "smartfarm/milk/data"
#define TOPIC_GATEWAY_STATUS "smartfarm/gateway/status"
#define TOPIC_COMMANDS "smartfarm/commands"

// LoRa Configuration
#define LORA_FREQUENCY 915E6
#define LORA_SS_PIN 16
#define LORA_RST_PIN 14
#define LORA_DIO0_PIN 26

// TFT Display Configuration (ILI9341 320x240)
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   5
#define TFT_DC   2
#define TFT_RST  4
#define TFT_BL   15

// Touch Screen (Optional)
#define TOUCH_CS 21
#define TOUCH_IRQ 22

// SD Card Configuration
#define SD_CS 17

// Device IDs
#define DEVICE_ID_COW "COW001"
#define DEVICE_ID_MILK "MILK001"

// Status LED
#define STATUS_LED 13
#define ERROR_LED 12

// Timing Configuration
#define DATA_SEND_INTERVAL 30000
#define STATUS_UPDATE_INTERVAL 1000
#define RECONNECT_INTERVAL 5000
#define WATCHDOG_TIMEOUT 60000
#define DATA_SAVE_INTERVAL 60000

// Data Buffer and Storage
#define MAX_DEVICES 50
#define MAX_COW_NODES 30
#define MAX_MILK_STATIONS 20
#define DATA_BUFFER_SIZE 1024
#define HISTORY_SIZE 100

// Database Configuration
#define DB_FILENAME "/sd/farmdata.db"
#define LOG_FILENAME "/sd/system.log"

// Web Server Configuration
#define WEB_SERVER_PORT 80
#define WEB_USERNAME "admin"
#define WEB_PASSWORD "smartfarm2025"

// Debug Configuration
#define DEBUG_SERIAL true
#define DEBUG_VERBOSE false

#endif // CONFIG_H
EOF

# Replace config.h with fixed version
mv include/config_fixed.h include/config.h

# Create improved WiFi manager
cat > src/wifi_manager_fixed.cpp << 'EOF'
#include "wifi_manager.h"

WiFiManager::WiFiManager() : connected(false), lastConnectAttempt(0), reconnectAttempts(0) {}

bool WiFiManager::begin() {
    // Check if we should skip WiFi client mode
    if (strcmp(WIFI_SSID, "DEMO_MODE_AP_ONLY") == 0) {
        Serial.println("Demo mode detected - creating AP only");
        return createAccessPoint();
    }
    
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    return connect();
}

bool WiFiManager::connect() {
    if (connected) return true;
    
    // Skip connection if in demo mode
    if (strcmp(WIFI_SSID, "DEMO_MODE_AP_ONLY") == 0) {
        return createAccessPoint();
    }
    
    Serial.println("Connecting to WiFi...");
    Serial.print("SSID: ");
    Serial.println(WIFI_SSID);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    unsigned long startTime = millis();
    int dotCount = 0;
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT_MS) {
        delay(100);  // Shorter delay
        Serial.print(".");
        dotCount++;
        if (dotCount > 10) break;  // Limit connection attempts
    }
    Serial.println();
    
    connected = (WiFi.status() == WL_CONNECTED);
    
    if (connected) {
        Serial.println("✓ WiFi Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("RSSI: ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
        reconnectAttempts = 0;
    } else {
        Serial.println("✗ WiFi Connection Failed - Creating Access Point");
        return createAccessPoint();
    }
    
    lastConnectAttempt = millis();
    return connected;
}

bool WiFiManager::createAccessPoint() {
    WiFi.disconnect(true);
    delay(100);
    
    WiFi.mode(WIFI_AP);
    bool apStarted = WiFi.softAP("SmartFarm_Gateway", "smartfarm123");
    
    if (apStarted) {
        Serial.println("✓ Access Point Created!");
        Serial.print("AP IP: ");
        Serial.println(WiFi.softAPIP());
        Serial.println("Connect to 'SmartFarm_Gateway' with password: smartfarm123");
        connected = true;
    } else {
        Serial.println("✗ Failed to create Access Point!");
        connected = false;
    }
    
    return apStarted;
}

bool WiFiManager::isConnected() {
    // In AP mode, always consider connected
    if (WiFi.getMode() == WIFI_AP) {
        return true;
    }
    
    connected = (WiFi.status() == WL_CONNECTED);
    return connected;
}

void WiFiManager::disconnect() {
    WiFi.disconnect();
    connected = false;
}

void WiFiManager::handleConnection() {
    // In demo mode (AP only), don't try to reconnect
    if (strcmp(WIFI_SSID, "DEMO_MODE_AP_ONLY") == 0) {
        return;
    }
    
    if (!connected && millis() - lastConnectAttempt > RECONNECT_INTERVAL) {
        Serial.println("WiFi lost. Reconnecting...");
        connect();
    }
}

int WiFiManager::getRSSI() {
    if (WiFi.getMode() == WIFI_AP) {
        return WiFi.softAPgetStationNum() > 0 ? -30 : -100;  // Fake RSSI for AP mode
    }
    return WiFi.RSSI();
}
EOF

# Backup and replace wifi_manager.cpp
mv src/wifi_manager.cpp src/wifi_manager.cpp.backup
mv src/wifi_manager_fixed.cpp src/wifi_manager.cpp

# Add createAccessPoint declaration to header
if ! grep -q "createAccessPoint" include/wifi_manager.h; then
    sed -i '/public:/a\    bool createAccessPoint();' include/wifi_manager.h
fi

echo "✅ Quick fix applied!"
echo
echo "📋 Changes made:"
echo "• Config: Set WIFI_SSID to 'DEMO_MODE_AP_ONLY'"
echo "• WiFi Manager: Skip client connection attempts in demo mode"
echo "• Timeout: Reduced to 1 second for faster startup"
echo "• Behavior: Creates AP immediately, no reconnection loops"
echo

echo "📦 Building with fixes..."
pio run

if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    echo
    echo "🚀 Expected behavior after upload:"
    echo "• Boot sequence will be much faster"
    echo "• No WiFi connection attempts to non-existent network"
    echo "• Access Point created immediately"
    echo "• No reconnection loops"
    echo "• Demo data generation continues normally"
    echo
    read -p "Upload fixed firmware now? (y/n): " upload_now
    
    if [ "$upload_now" = "y" ]; then
        echo "📤 Uploading firmware..."
        pio run --target upload
        echo "✅ Upload complete!"
        echo "📺 Monitor with: pio device monitor --baud 115200"
    fi
else
    echo "❌ Build failed! Restoring backup..."
    cp include/config.h.backup.* include/config.h
    cp src/wifi_manager.cpp.backup src/wifi_manager.cpp
fi

echo
echo "🎯 WiFi loop issue should now be resolved!"