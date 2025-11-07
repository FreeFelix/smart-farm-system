#!/bin/bash

echo "🔄 Restarting Wokwi Simulator with TFT Display fixes..."
echo "=================================================="

# Kill any existing Wokwi processes
pkill -f wokwi || true

# Wait a moment for cleanup
sleep 2

# Start new simulation
echo "🚀 Starting Wokwi simulator..."
echo "Expected behavior:"
echo "  ✅ LEDs should flash during startup (green/red test sequence)"
echo "  ✅ TFT should show color sequence: RED → GREEN → BLUE → WHITE"
echo "  ✅ TFT should display 'SMART FARM GATEWAY v2.0' welcome screen"
echo ""

# Run simulator
wokwi-cli --timeout 0 .

echo "Simulation ended."