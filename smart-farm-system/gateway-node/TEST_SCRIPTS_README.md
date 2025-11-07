# 🧪 Gateway Demo Test Scripts

**Smart Farm Gateway v2.0** - Complete testing and demonstration suite

## 🚀 Quick Start

### Fix WiFi Reconnection Loops (Most Common Issue)
```bash
./fix_wifi_loops.sh
```
**One-click solution** for the constant WiFi reconnection issue.

---

## 📋 Available Test Scripts

### 🎯 **Main Test Suite**
```bash
./gateway_test_suite.sh
```
**Comprehensive testing menu** with all options:
- Build tests
- Upload tests  
- Serial monitoring
- Component diagnostics
- Full system tests

### 🌐 **WiFi Configuration Tests**
```bash
./wifi_config_test.sh
```
**WiFi setup and configuration:**
- Home WiFi setup
- Office/Enterprise WiFi
- Mobile hotspot configuration
- Demo mode (AP only)
- Public WiFi setup
- Auto-detection

### 🔧 **Component Diagnostics**
```bash
./component_diagnostic.sh
```
**Individual component testing:**
- TFT Display analysis
- SD Card diagnostics
- LoRa module testing
- WiFi stability check
- MQTT connectivity
- Automatic fixes

### 📊 **Demo Data Testing**
```bash
./demo_data_test.sh
```
**Demo data generation validation:**
- Real-time data monitoring
- Data range validation
- Generation timing tests
- Memory usage analysis
- Custom scenarios

### ⚡ **Quick Fixes**
```bash
./quick_wifi_fix.sh      # Comprehensive WiFi fix
./fix_wifi_loops.sh      # Simple one-click WiFi fix
```

---

## 🎯 Common Issues & Solutions

### Issue 1: WiFi Reconnection Loops
**Symptoms:**
```
WiFi lost. Reconnecting...
✗ WiFi Connection Failed - Creating Access Point
✓ Access Point Created!
[Repeats constantly]
```

**Solution:**
```bash
./fix_wifi_loops.sh
```

### Issue 2: SD Card Failures (Normal in Simulation)
**Symptoms:**
```
✗ SD Card initialization failed!
f_mount failed: (3) The physical drive cannot work
```

**Solution:** This is expected in Wokwi simulation. Use real SD card for hardware testing.

### Issue 3: LoRa Module Not Found (Normal in Simulation)
**Symptoms:**
```
✗ LoRa initialization failed!
```

**Solution:** This is expected in Wokwi simulation. Demo mode automatically activates.

### Issue 4: No Demo Data Generated
**Solution:**
```bash
./demo_data_test.sh
# Select option 1 for real-time monitoring
```

---

## 📊 Expected Demo Behavior

### Normal Boot Sequence:
```
========================================
   SMART FARM GATEWAY v2.0 (ESP32)
========================================

Step 1/5: Initializing TFT Display...
✓ TFT Display Initialized!

Step 2/5: Initializing SD Card...
✗ SD Card still failed - continuing without storage

Step 3/5: Initializing LoRa...
✗ LoRa initialization failed!
   This is normal in simulation environment

Step 4/5: Connecting to WiFi...
✓ Access Point Created!
AP IP: 192.168.4.1

Step 5/5: Connecting to MQTT Broker...
Warning: MQTT connection failed - local operation only

========================================
   GATEWAY SYSTEM READY!
========================================

--- Demo Cow Data Generated ---
Device: DEMO_COW_1
Temperature: 38.70 °C
Heart Rate: 65.00 bpm
[... more demo data every 15 seconds]
```

### Demo Data Pattern:
- **Every 15 seconds**: New cow or milk data
- **3 Cow Nodes**: DEMO_COW_1, DEMO_COW_2, DEMO_COW_3
- **2 Milk Stations**: DEMO_MILK_1, DEMO_MILK_2
- **Realistic Values**: Temperature, pH, heart rate within normal ranges

---

## 🔧 Hardware Testing

### For Real Hardware:
1. **Update WiFi credentials** in `include/config.h`
2. **Connect components** according to pin diagrams
3. **Upload firmware**: `pio run --target upload`
4. **Monitor operation**: `pio device monitor --baud 115200`

### Pin Connections:
```
ESP32 → ILI9341 TFT:
  GPIO 23 → MOSI
  GPIO 19 → MISO  
  GPIO 18 → SCK
  GPIO 5  → CS
  GPIO 2  → DC
  GPIO 4  → RST
  GPIO 15 → BL (Backlight)

Status LEDs:
  GPIO 13 → Green LED (System OK)
  GPIO 12 → Red LED (Error)
```

---

## 🚀 Production Deployment

### 1. Configure for Your Network:
```bash
./wifi_config_test.sh
# Select option 1 (Home WiFi) and enter your credentials
```

### 2. Build and Upload:
```bash
pio run --target upload
```

### 3. Monitor Operation:
```bash
pio device monitor --baud 115200
```

### 4. Connect to Gateway:
- **WiFi SSID**: SmartFarm_Gateway
- **Password**: smartfarm123
- **Web Interface**: http://192.168.4.1

---

## 🎯 Success Indicators

### ✅ Working System:
- TFT display shows colorful boot sequence
- Access Point created (if no WiFi network)
- Demo data generates every 15 seconds
- Status LEDs provide visual feedback
- Memory usage stable (~23% RAM)

### ❌ Issues to Check:
- Constant WiFi reconnection loops → Run `./fix_wifi_loops.sh`
- No demo data → Check LoRa is disabled (normal in simulation)
- Build failures → Check PlatformIO installation
- No serial output → Check baud rate (115200)

---

## 📞 Support

### Test Files Location:
```
gateway-node/
├── gateway_test_suite.sh      # Main menu
├── fix_wifi_loops.sh          # Quick WiFi fix
├── wifi_config_test.sh        # WiFi configuration
├── component_diagnostic.sh    # Hardware diagnostics  
├── demo_data_test.sh         # Demo data validation
├── quick_wifi_fix.sh         # Advanced WiFi fix
└── TEST_SCRIPTS_README.md    # This file
```

### Build Information:
- **Platform**: ESP32
- **Framework**: Arduino
- **Build Time**: ~15 seconds
- **Memory Usage**: 23.6% RAM, 67.5% Flash
- **Libraries**: TFT_eSPI, LoRa, WiFi, ArduinoJson

---

**🎉 Your Smart Farm Gateway v2.0 is ready for demonstration!**

Use the test scripts to validate functionality and fix any issues. The system is designed to work gracefully even with missing hardware components.