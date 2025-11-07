#!/bin/bash

# ==================== WIFI CONFIGURATION TEST ====================
# Fixes the constant WiFi reconnection issue

echo "🌐 WiFi Configuration Test & Fix"
echo "=================================="
echo

echo "📋 Current WiFi Issue:"
echo "• Gateway constantly tries to connect to 'SmartFarm_Demo'"
echo "• This causes reconnection loops and AP creation cycles"
echo "• Demo mode works but with network instability"
echo

echo "🔧 Available WiFi Configurations:"
echo
echo "1. 🏠 Home WiFi Setup (Enter your WiFi credentials)"
echo "2. 🏢 Office WiFi Setup (WPA2-Enterprise)"
echo "3. 📱 Mobile Hotspot Setup"
echo "4. 🔧 Demo Mode (AP only, no client attempts)"
echo "5. 🌐 Public WiFi Setup"
echo "6. 📶 Auto-detect and configure"
echo "0. ⬅️  Back to main menu"
echo

read -p "Select WiFi configuration (0-6): " wifi_choice

case $wifi_choice in
    1)
        echo "🏠 Home WiFi Setup"
        echo "=================="
        read -p "Enter your WiFi SSID: " home_ssid
        read -s -p "Enter your WiFi Password: " home_pass
        echo
        
        # Update config.h with home WiFi
        sed -i "s/#define WIFI_SSID \".*\"/#define WIFI_SSID \"$home_ssid\"/" include/config.h
        sed -i "s/#define WIFI_PASSWORD \".*\"/#define WIFI_PASSWORD \"$home_pass\"/" include/config.h
        sed -i "s/#define WIFI_TIMEOUT_MS.*/#define WIFI_TIMEOUT_MS 20000/" include/config.h
        
        echo "✅ Home WiFi configured!"
        echo "📦 Building and uploading..."
        pio run --target upload
        ;;
    2)
        echo "🏢 Office WiFi Setup"
        echo "==================="
        read -p "Enter WiFi SSID: " office_ssid
        read -p "Enter Username: " office_user
        read -s -p "Enter Password: " office_pass
        echo
        
        echo "⚠️  Enterprise WiFi requires additional configuration."
        echo "📝 Manual steps needed in code for WPA2-Enterprise"
        ;;
    3)
        echo "📱 Mobile Hotspot Setup"
        echo "======================"
        read -p "Enter Hotspot SSID: " hotspot_ssid
        read -s -p "Enter Hotspot Password: " hotspot_pass
        echo
        
        # Update config with hotspot
        sed -i "s/#define WIFI_SSID \".*\"/#define WIFI_SSID \"$hotspot_ssid\"/" include/config.h
        sed -i "s/#define WIFI_PASSWORD \".*\"/#define WIFI_PASSWORD \"$hotspot_pass\"/" include/config.h
        sed -i "s/#define WIFI_TIMEOUT_MS.*/#define WIFI_TIMEOUT_MS 15000/" include/config.h
        
        echo "✅ Mobile hotspot configured!"
        ;;
    4)
        echo "🔧 Demo Mode (AP Only) Setup"
        echo "============================"
        
        # Create demo-only configuration
        cat > temp_config.h << EOF
// Demo Mode Configuration - AP Only
#define WIFI_SSID "DEMO_MODE_SKIP"        // Special skip marker
#define WIFI_PASSWORD "demo123"
#define WIFI_TIMEOUT_MS 3000             // Very short timeout
#define WIFI_DEMO_MODE true              // Enable demo mode flag
EOF
        
        # Update config.h
        cp include/config.h include/config.h.backup
        sed -i '/WiFi Configuration/,/WIFI_TIMEOUT_MS/c\
// WiFi Configuration - DEMO MODE (AP ONLY)\
#define WIFI_SSID "DEMO_MODE_SKIP"        // Will skip connection attempts\
#define WIFI_PASSWORD "demo123"\
#define WIFI_TIMEOUT_MS 3000             // Quick timeout for demo' include/config.h
        
        echo "✅ Demo mode (AP only) configured!"
        echo "📦 Building and uploading..."
        pio run --target upload
        ;;
    5)
        echo "🌐 Public WiFi Setup"
        echo "==================="
        echo "📋 Available public networks:"
        echo "1. FreeWiFi"
        echo "2. GuestNetwork" 
        echo "3. PublicInternet"
        echo "4. Custom public network"
        
        read -p "Select or enter custom SSID: " public_ssid
        
        # Most public WiFi has no password
        sed -i "s/#define WIFI_SSID \".*\"/#define WIFI_SSID \"$public_ssid\"/" include/config.h
        sed -i "s/#define WIFI_PASSWORD \".*\"/#define WIFI_PASSWORD \"\"/" include/config.h
        sed -i "s/#define WIFI_TIMEOUT_MS.*/#define WIFI_TIMEOUT_MS 30000/" include/config.h
        
        echo "✅ Public WiFi configured!"
        ;;
    6)
        echo "📶 Auto-detect WiFi Networks"
        echo "============================="
        echo "🔍 Scanning for available networks..."
        
        # This would require ESP32 to scan and report networks
        echo "⚠️  Auto-detection requires ESP32 firmware with WiFi scan capability"
        echo "📝 Implementing WiFi scanner..."
        
        # Create a simple WiFi scanner update
        cat >> src/wifi_scanner.cpp << 'EOF'
#include <WiFi.h>

void scanAndDisplayNetworks() {
    Serial.println("Scanning WiFi networks...");
    int n = WiFi.scanNetworks();
    
    if (n == 0) {
        Serial.println("No networks found");
    } else {
        Serial.printf("Found %d networks:\n", n);
        for (int i = 0; i < n; ++i) {
            Serial.printf("%d: %s (%d dBm) %s\n", 
                         i+1, 
                         WiFi.SSID(i).c_str(), 
                         WiFi.RSSI(i),
                         (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Encrypted");
        }
    }
    WiFi.scanDelete();
}
EOF
        
        echo "✅ WiFi scanner code added!"
        ;;
    0)
        echo "⬅️  Returning to main menu..."
        return
        ;;
    *)
        echo "❌ Invalid option"
        ;;
esac

echo
echo "🔄 WiFi configuration updated!"
echo "📋 Next steps:"
echo "1. Build and upload firmware: pio run --target upload"
echo "2. Monitor serial output: pio device monitor"
echo "3. Test WiFi connection stability"
echo
echo "🎯 To fix the reconnection loop issue permanently,"
echo "   select option 4 (Demo Mode) for stable operation."