# Smart Farm System - Source Code

This directory contains the source code for all three main components of the Smart Farm System.

## Directory Structure

```
src/
├── cow-node/           # Wearable cow health monitoring device
├── gateway-node/       # Central communication hub with display
└── milk-station-node/  # Milk quality analysis station
```

## Components Overview

### 🐄 Cow Node
- **Purpose**: Individual cow health monitoring
- **Sensors**: Temperature, GPS, accelerometer, heart rate, RFID
- **Power**: Solar + battery
- **Communication**: LoRa
- **Housing**: Weatherproof collar-mounted device

### 🏠 Gateway Node
- **Purpose**: Central farm coordination and display
- **Display**: 3.5" TFT color screen
- **Communication**: WiFi + LoRa + Ethernet
- **Features**: Status monitoring, data aggregation, cloud sync

### 🥛 Milk Station Node
- **Purpose**: Milk quality analysis at milking points
- **Sensors**: pH, conductivity, temperature, RFID reader
- **Display**: 16x2 LCD with keypad
- **Communication**: LoRa
- **Features**: Quality grading, session management

## Build Instructions

1. Install PlatformIO IDE
2. Open each component directory
3. Install dependencies: `pio lib install`
4. Build: `pio run`
5. Upload: `pio run --target upload`

## Hardware Requirements

- ESP32 development boards
- Sensor modules (see hardware budget)
- Communication modules (LoRa, WiFi)
- Power management circuits
- Weatherproof enclosures

## Configuration

Each node supports serial configuration commands for:
- Device ID assignment
- Network parameters
- Sensor calibration
- Alert thresholds

---

**Powered by ThereTech.rw**