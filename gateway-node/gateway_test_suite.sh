#!/bin/bash

# ==================== GATEWAY DEMO TEST SUITE ====================
# Smart Farm Gateway v2.0 - Complete Testing Scripts
# Date: November 7, 2025
# Purpose: Comprehensive testing and demonstration scripts

echo "=========================================="
echo "  SMART FARM GATEWAY v2.0 TEST SUITE"
echo "=========================================="
echo

# Check if we're in the right directory
if [ ! -f "platformio.ini" ]; then
    echo "❌ Error: Not in gateway-node directory"
    echo "Please run: cd /home/admino/smart-farm-system/gateway-node"
    exit 1
fi

echo "🎯 Available Test Options:"
echo
echo "1. 📦 Build Test (Compile only)"
echo "2. 🔄 Build & Upload Test"
echo "3. 📺 Serial Monitor Test"
echo "4. 🌐 WiFi Configuration Test"
echo "5. 🔧 Component Diagnostic Test"
echo "6. 📊 Demo Data Simulation Test"
echo "7. 🏭 Production Mode Setup"
echo "8. 🔍 Full System Test (All components)"
echo "9. 🧹 Clean & Reset"
echo "0. ❌ Exit"
echo

read -p "Select test option (0-9): " choice

case $choice in
    1)
        echo "🔨 Building Gateway Firmware..."
        pio run
        echo "✅ Build test completed!"
        ;;
    2)
        echo "🔨 Building and Uploading..."
        pio run --target upload
        echo "✅ Upload completed! Check device status."
        ;;
    3)
        echo "📺 Starting Serial Monitor..."
        echo "Press Ctrl+C to exit monitor"
        pio device monitor --baud 115200
        ;;
    4)
        ./wifi_config_test.sh
        ;;
    5)
        ./component_diagnostic.sh
        ;;
    6)
        ./demo_data_test.sh
        ;;
    7)
        ./production_setup.sh
        ;;
    8)
        ./full_system_test.sh
        ;;
    9)
        echo "🧹 Cleaning build files..."
        pio run --target clean
        echo "✅ Clean completed!"
        ;;
    0)
        echo "👋 Goodbye!"
        exit 0
        ;;
    *)
        echo "❌ Invalid option. Please select 0-9."
        ;;
esac

echo
echo "📋 Test completed! Run './gateway_test_suite.sh' again for more tests."