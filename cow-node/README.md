# Cow Health Monitoring Node

## Overview
Wearable ESP32-based health monitoring device for dairy cattle. This weatherproof node tracks cow health metrics and transmits data via dual communication (WiFi + LoRa) to the central Device Manager.

## Key Features
- **Wearable Design**: Compact, weatherproof device worn by cattle
- **Dual Communication**: WiFi (primary) + LoRa (backup/gateway)
- **Health Monitoring**: Temperature, activity, GPS location, digestion sounds
- **Power Management**: Battery with solar charging support
- **No Display**: Designed for autonomous operation without user interface
- **RFID Integration**: Cow identification at milk stations (milk production tracked at station, not on cow)

## Hardware Requirements

### Microcontroller
- ESP32 Development Board (240MHz, 4MB Flash, 320KB RAM)

### Sensors
- **DS18B20**: Body temperature sensor (waterproof probe)
- **MPU6050**: 6-axis accelerometer/gyroscope (activity tracking)
- **NEO-6M GPS**: Location tracking module
- **MAX9814**: Electret microphone (digestion sounds)

### Communication
- **LoRa SX1276/78**: 915 MHz radio module
- **Built-in WiFi**: ESP32 integrated WiFi

### Power
- **18650 Battery**: Rechargeable lithium-ion
- **Solar Panel**: 5V charging panel (optional)
- **TP4056**: Battery charging controller

### Status Indicators
- **LEDs**: Red (alerts), Yellow (warnings), Green (healthy)

## Pin Configuration

### Sensors
| Sensor | Pin | Description |
|--------|-----|-------------|
| DS18B20 | GPIO 4 | Temperature (OneWire) |
| MPU6050 | GPIO 21 (SDA), GPIO 22 (SCL) | Activity (I2C) |
| NEO-6M GPS | GPIO 16 (RX), GPIO 17 (TX) | Location (UART) |
| MAX9814 | GPIO 34 | Digestion sounds (ADC) |

### Communication
| Module | Pin | Description |
|--------|-----|-------------|
| LoRa SX1276 | GPIO 5 (CS), GPIO 23 (MOSI), GPIO 19 (MISO), GPIO 18 (SCK) | LoRa radio (SPI) |
| WiFi | Built-in | ESP32 integrated WiFi |

### Power & Status
| Component | Pin | Description |
|-----------|-----|-------------|
| Battery Monitor | GPIO 35 | Battery voltage (ADC) |
| Solar Monitor | GPIO 32 | Solar voltage (ADC) |
| LED Red | GPIO 25 | Health alert indicator |
| LED Yellow | GPIO 26 | Warning indicator |
| LED Green | GPIO 27 | Healthy status indicator |

## Quick Start

### 1. Hardware Assembly
1. Connect DS18B20 temperature sensor to GPIO 4 (with 4.7kΩ pull-up)
2. Connect MPU6050 to I2C (SDA: GPIO 21, SCL: GPIO 22)
3. Connect GPS module to UART (RX: GPIO 16, TX: GPIO 17)
4. Connect microphone to GPIO 34
5. Wire LoRa module to SPI pins
6. Connect status LEDs with current-limiting resistors (220Ω)
7. Set up battery and solar charging circuit

### 2. Software Setup
```bash
# Install PlatformIO
pip install platformio

# Navigate to project
cd cow-node

# Configure device settings
./configure.sh

# Build and upload
pio run --target upload

# Monitor serial output
pio device monitor
```

### 3. Configuration
Device settings are stored in ESP32 NVS (Non-Volatile Storage):

```cpp
// WiFi Settings
WiFi SSID: "YourFarmWiFi"
WiFi Password: "YourPassword"

// Device Manager
Server: "http://192.168.1.100:8080/api/health"

// LoRa Settings
Frequency: 915 MHz
Bandwidth: 125 kHz
Spreading Factor: 7

// Farm Settings
Farm ID: "FARM001"
Cow Gender: "female" or "male"
```

## System Architecture

### Health Monitoring
The node continuously monitors:
- **Temperature**: Body temperature (normal: 38-39°C)
- **Activity**: Movement patterns, lying time
- **Location**: GPS coordinates
- **Digestion**: Rumination sounds
- **Battery**: Power levels and charging status

### Communication Flow
1. **WiFi Primary**: HTTP POST JSON data every 60 seconds
2. **LoRa Backup**: Broadcast pipe-delimited data if WiFi fails
3. **Auto-Reconnect**: WiFi reconnection every 30 seconds if disconnected

### Data Format

**WiFi (JSON)**:
```json
{
  "deviceId": "COW-001",
  "farmId": "FARM001",
  "gender": "female",
  "temperature": 38.5,
  "activity": 145,
  "gpsLat": 37.7749,
  "gpsLon": -122.4194,
  "batteryLevel": 85,
  "timestamp": 1699380000
}
```

**LoRa (Pipe-delimited)**:
```
COW-001|FARM001|female|38.5|145|37.7749|-122.4194|85|1699380000
```

## Health Alerts

### Temperature Alerts
- **Low**: < 37.5°C (Red LED blinks)
- **High**: > 39.5°C (Red LED blinks)
- **Normal**: 38-39°C (Green LED solid)

### Activity Alerts
- **Low**: < 50 steps/hour (Yellow LED blinks - possible illness)
- **Normal**: > 50 steps/hour (Green LED solid)

### Battery Alerts
- **Low**: < 20% (Yellow LED blinks)
- **Critical**: < 10% (Red LED blinks)

## File Structure
```
cow-node/
├── include/          # Header files
│   ├── config.h              # Global configuration
│   ├── device_config.h       # Device settings (NVS)
│   ├── sensors.h             # Sensor interfaces
│   ├── health_data.h         # Health data structure
│   ├── health_monitor.h      # Health checking logic
│   ├── communication.h       # WiFi + LoRa
│   └── system_init.h         # System initialization
├── src/              # Implementation files
│   ├── main.cpp              # Main program loop
│   ├── sensors.cpp           # Sensor reading
│   ├── health_monitor.cpp    # Health analysis
│   ├── communication.cpp     # WiFi + LoRa transmission
│   ├── device_config.cpp     # Configuration management
│   └── system_init.cpp       # Hardware initialization
├── lib/              # Library modules (organized)
├── platformio.ini    # PlatformIO configuration
├── configure.sh      # Interactive configuration tool
├── simulate.sh       # Device simulation script
└── README.md         # This file
```

## Design Philosophy

### Wearable & Weatherproof
- No display (reduces power consumption, improves weatherproofing)
- Compact design for comfortable cow wear
- Sealed enclosure protects against water, dust, manure
- Rechargeable battery with solar charging

### Separation of Concerns
- **Cow Node**: Health monitoring only
- **Milk Station**: Milk production tracking via RFID
- Cow identified at station by RFID, milk data linked to cow ID in database

### Dual Communication
- **WiFi**: Fast, high-bandwidth, direct to Device Manager
- **LoRa**: Long-range, low-power, works through gateway if WiFi unavailable
- Automatic failover ensures data never lost

## Troubleshooting

### WiFi Not Connecting
- Check SSID/password in configuration
- Verify router is accessible from cow location
- Check WiFi LED status (should blink during connection)

### LoRa Not Transmitting
- Verify LoRa module connections (SPI pins)
- Check frequency matches gateway (915 MHz)
- Ensure antenna is properly connected

### Temperature Reading 0°C or -127°C
- Check DS18B20 connections
- Verify 4.7kΩ pull-up resistor on data line
- Ensure sensor is waterproof probe touching cow skin

### GPS Not Getting Fix
- Move device to open area (clear sky view)
- Wait 30-60 seconds for initial fix
- Check antenna connection

### Battery Draining Quickly
- Check solar panel charging (monitor serial output)
- Reduce WiFi transmission frequency
- Verify all sensors initialized properly (failed sensor draws power)

## Build Information
- **Platform**: Espressif 32 (ESP32)
- **Framework**: Arduino
- **Flash Usage**: ~75% (988,177 bytes)
- **RAM Usage**: ~15% (48,172 bytes)
- **Upload Speed**: 921600 baud

## License
MIT License - Smart Farm System 2024

## Support
For technical support or questions, contact the farm IT administrator.
