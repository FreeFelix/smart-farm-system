#!/bin/bash

# ==================== ONE-CLICK WIFI FIX ====================
# Simple solution for the WiFi reconnection loop issue

clear
echo "🔧 Smart Farm Gateway - WiFi Loop Fix"
echo "====================================="
echo

echo "🎯 This script fixes the WiFi reconnection loop issue"
echo "📋 Problem: Gateway constantly tries to connect to 'SmartFarm_Demo'"
echo "✅ Solution: Switch to AP-only demo mode"
echo

echo "⚡ QUICK FIX OPTION:"
echo "1. Apply instant fix (recommended)"
echo "2. View detailed options"
echo "3. Exit"
echo

read -p "Select option (1-3): " choice

case $choice in
    1)
        echo "⚡ Applying instant WiFi fix..."
        
        # Backup config
        cp include/config.h include/config.h.backup
        
        # Apply the simplest fix - change SSID to trigger AP mode
        sed -i 's/SmartFarm_Demo/DEMO_MODE_AP_ONLY/' include/config.h
        sed -i 's/WIFI_TIMEOUT_MS 10000/WIFI_TIMEOUT_MS 1000/' include/config.h
        
        echo "✅ Configuration updated!"
        echo "📦 Building firmware..."
        
        if pio run; then
            echo "✅ Build successful!"
            echo
            echo "🚀 Ready to upload! Run: pio run --target upload"
            echo
            echo "📋 Expected result:"
            echo "• No more WiFi connection loops"
            echo "• Faster boot sequence"
            echo "• Stable Access Point operation"
            echo "• Demo data continues normally"
        else
            echo "❌ Build failed. Restoring backup..."
            cp include/config.h.backup include/config.h
        fi
        ;;
    2)
        echo "🔧 Launching detailed test suite..."
        ./gateway_test_suite.sh
        ;;
    3)
        echo "👋 Exiting..."
        exit 0
        ;;
    *)
        echo "❌ Invalid option"
        exit 1
        ;;
esac

echo
echo "✅ WiFi fix complete!"
echo "📋 Next steps:"
echo "1. Upload firmware: pio run --target upload"
echo "2. Monitor output: pio device monitor --baud 115200"
echo "3. Verify no more WiFi reconnection loops"
echo
echo "🎯 Your gateway should now run smoothly in demo mode!"