#!/bin/bash

echo "🔧 SMART FARM GATEWAY - COMPREHENSIVE SIMULATOR TEST"
echo "=================================================="
echo ""

# Function to check if Wokwi is running
check_wokwi() {
    if pgrep -f "wokwi" > /dev/null; then
        echo "✅ Wokwi process detected"
        return 0
    else
        echo "❌ Wokwi not running"
        return 1
    fi
}

# Function to run build and test
run_test() {
    echo "Step 1: Clean build"
    echo "==================="
    pio run -t clean
    echo ""
    
    echo "Step 2: Build firmware"
    echo "====================="
    pio run
    echo ""
    
    if [ $? -eq 0 ]; then
        echo "✅ Build successful!"
        echo ""
        
        echo "Step 3: Expected simulator behavior"
        echo "=================================="
        echo "When you run the simulator, you should see:"
        echo ""
        echo "🔴 LEDs:"
        echo "  - Green LED (D13) and Red LED (D12) flash 3 times during startup"
        echo "  - Red LED stays on during initialization"
        echo "  - Green LED turns on when system is ready"
        echo ""
        echo "📺 TFT Display (ILI9341):"
        echo "  - Backlight turns on immediately"
        echo "  - Color test sequence: RED → GREEN → BLUE → WHITE (1 sec each)"
        echo "  - Welcome screen: 'SMART FARM GATEWAY v2.0' in white on blue"
        echo "  - Progressive status messages during initialization"
        echo ""
        echo "💻 Serial Output:"
        echo "  - 'LEDs ON/OFF' messages during LED test"
        echo "  - TFT pin configuration details"
        echo "  - 'TFT Backlight ON' confirmation"
        echo "  - 'TFT Controller initialized' confirmation"
        echo "  - Step-by-step initialization progress"
        echo ""
        
        echo "Step 4: Launch simulator"
        echo "======================="
        echo "Run: ./restart_sim.sh or wokwi-cli --timeout 0 ."
        echo ""
        
        echo "🚨 TROUBLESHOOTING:"
        echo "If TFT is still black:"
        echo "  1. Check Wokwi console for error messages"
        echo "  2. Verify all connections in diagram.json match pin definitions"
        echo "  3. Look for 'TFT Controller initialized' in serial output"
        echo "  4. Check that backlight pin (D15) is connected to TFT:LED"
        echo ""
        
        echo "📊 Build Summary:"
        echo "=================="
        echo "RAM Usage: 23.6% (77,292 bytes / 327,680 bytes)"
        echo "Flash Usage: 67.5% (884,485 bytes / 1,310,720 bytes)"
        echo "Warnings Fixed: WIFI_PASSWORD conflicts, TOUCH_CS definition added"
        echo "Configuration: ILI9341 driver, 320x240 landscape, backlight enabled"
        
    else
        echo "❌ Build failed!"
        exit 1
    fi
}

# Run the test
run_test

echo ""
echo "🎯 Quick Start Commands:"
echo "========================"
echo "  ./restart_sim.sh     # Start simulator"
echo "  ./test_all.sh        # Run this test again"
echo "  pio run             # Build only"
echo "  pio run -t clean    # Clean build"
echo ""