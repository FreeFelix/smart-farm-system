# Cow Node - Quick Start Guide

## What is This?
A wearable health monitoring device for dairy cattle that tracks:
- Body temperature
- Activity levels
- GPS location
- Digestion sounds
- Battery & solar charging

Data transmits to Device Manager via WiFi + LoRa (dual communication for reliability).

## Hardware Needed
- ESP32 dev board
- DS18B20 temperature sensor
- MPU6050 accelerometer
- NEO-6M GPS module
- MAX9814 microphone
- LoRa SX1276 module
- 18650 battery + TP4056 charger
- Solar panel (optional)
- 3 LEDs (red, yellow, green) + resistors
- Weatherproof enclosure

## Quick Setup (5 Steps)

### 1. Install Software
```bash
# Install PlatformIO
pip install platformio

# Navigate to project
cd cow-node
```

### 2. Wire Hardware
Connect sensors to ESP32 (see pin diagram in README.md):
- Temperature sensor → GPIO 4
- Accelerometer → I2C (GPIO 21/22)
- GPS → UART (GPIO 16/17)
- LoRa → SPI (GPIO 5/18/19/23)
- LEDs → GPIO 25/26/27
- Battery monitor → GPIO 35

### 3. Configure Device
```bash
# Run interactive setup
./configure.sh

# Enter when prompted:
- WiFi name and password
- Device Manager URL (e.g., http://192.168.1.100:8080/api/health)
- Farm ID (e.g., FARM001)
- Cow gender (female/male)
```

### 4. Upload Firmware
```bash
# Build and upload to ESP32
pio run --target upload

# Monitor output
pio device monitor
```

### 5. Deploy
1. Verify green LED lights (healthy status)
2. Check serial output shows data transmission
3. Seal in weatherproof enclosure
4. Attach to cow collar or harness
5. Monitor first 24 hours via Device Manager dashboard

## Understanding LED Status
- 🟢 **Green**: All healthy, data transmitting
- 🟡 **Yellow**: Warning (low activity or battery < 20%)
- 🔴 **Red**: Alert (fever, hypothermia, or battery critical)

## Data Transmission
- **WiFi**: Sends JSON every 60 seconds to Device Manager
- **LoRa**: Broadcasts every 60 seconds (backup/gateway)
- **Auto-reconnect**: WiFi retries every 30 seconds if disconnected

## Milk Production Note
This device does NOT track milk production. Milk is measured at the milk station when the cow is identified by RFID. This node only monitors health metrics.

## Testing
```bash
# Simulate device behavior
./simulate.sh

# View example health data output
# Shows GPS tracking, temperature, activity
```

## Troubleshooting

| Problem | Solution |
|---------|----------|
| WiFi won't connect | Check SSID/password in config, verify signal strength |
| Temperature shows 0°C | Check DS18B20 wiring, needs 4.7kΩ pull-up resistor |
| GPS no fix | Move to open area, wait 30-60 seconds |
| Battery draining fast | Check solar panel connection, reduce WiFi frequency |
| No data on server | Verify Device Manager URL, check network firewall |

## Next Steps
- Read **README.md** for complete hardware details
- Read **CONFIGURATION.md** for advanced settings
- Read **DEVELOPMENT.md** for code architecture

## Support
Monitor serial output at 115200 baud for diagnostic messages. All errors and status updates are logged.

---
**Build Status**: ✅ Builds successfully (75% flash, 15% RAM)  
**Battery Life**: ~60-70 hours (no solar), indefinite (with solar)  
**Range**: WiFi ~100m, LoRa ~2-15km (depends on settings)
