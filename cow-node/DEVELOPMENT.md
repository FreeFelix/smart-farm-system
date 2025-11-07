# Cow Node Development Guide

## Architecture Overview

The cow health monitoring node follows a modular architecture with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────┐
│                      Main Loop                          │
│  (Orchestrates health monitoring & communication)       │
└──────────┬──────────────────────────────────┬───────────┘
           │                                  │
┌──────────▼──────────┐            ┌─────────▼────────────┐
│  Health Monitor     │            │   Communication      │
│  - Check sensors    │            │   - WiFi HTTP POST   │
│  - Analyze data     │            │   - LoRa broadcast   │
│  - Trigger alerts   │            │   - Auto-reconnect   │
└──────────┬──────────┘            └─────────┬────────────┘
           │                                  │
┌──────────▼──────────┐            ┌─────────▼────────────┐
│     Sensors         │            │  Device Config       │
│  - Temperature      │            │  - NVS storage       │
│  - Accelerometer    │            │  - WiFi settings     │
│  - GPS              │            │  - LoRa settings     │
│  - Microphone       │            │  - Farm ID           │
│  - Battery/Solar    │            │  - Cow gender        │
└─────────────────────┘            └──────────────────────┘
```

## Module Descriptions

### Main Module (`src/main.cpp`)
**Purpose**: Main program loop and coordination

**Key Functions**:
- `setup()`: Initialize all subsystems
- `loop()`: Health monitoring cycle
- WiFi reconnection logic
- Status LED updates

**Dependencies**: All other modules

**Data Flow**:
1. Read sensors → Health data struct
2. Check health → Alerts/warnings
3. Transmit data → WiFi/LoRa
4. Update LEDs → Visual status
5. Repeat every 60 seconds

### Sensors Module (`src/sensors.cpp`)
**Purpose**: Hardware sensor interfaces

**Key Functions**:
- `initializeSensors()`: Setup all sensors
- `readTemperature()`: DS18B20 body temp
- `readActivity()`: MPU6050 accelerometer
- `readGPS()`: NEO-6M location
- `readMicrophone()`: Digestion sounds
- `readBattery()`: Battery/solar levels

**Hardware Interfaces**:
- OneWire (temperature)
- I2C (accelerometer)
- UART (GPS)
- ADC (microphone, battery)

**Error Handling**:
- Returns default values on sensor failure
- Logs errors to serial
- Sets error flags in health data

### Health Monitor (`src/health_monitor.cpp`)
**Purpose**: Analyze sensor data and trigger alerts

**Key Functions**:
- `checkHealthAlerts()`: Evaluate all thresholds
- `updateStatusLEDs()`: Control LED indicators
- `printHealthData()`: Format serial output

**Alert Logic**:
```cpp
// Temperature
if (temp < 37.5 || temp > 39.5) → RED LED
else if (temp >= 38.0 && temp <= 39.0) → GREEN LED

// Activity
if (activity < 50) → YELLOW LED
else → GREEN LED

// Battery
if (battery < 10%) → RED LED
else if (battery < 20%) → YELLOW LED
```

### Communication Module (`src/communication.cpp`)
**Purpose**: Dual WiFi + LoRa transmission

**Key Functions**:
- `initializeCommunication()`: Setup WiFi and LoRa
- `connectToWiFi()`: WiFi connection with timeout
- `transmitViaWiFi()`: HTTP POST JSON data
- `transmitLoRa()`: LoRa broadcast
- `transmitHealthData()`: Unified transmission (WiFi + LoRa)

**Communication Flow**:
```
1. Try WiFi HTTP POST
   ├─ Success: Send JSON to Device Manager
   └─ Failure: Log error, continue to LoRa

2. Send LoRa broadcast
   └─ Always send (gateway may relay to cloud)

3. Return success if either method worked
```

**Data Formats**:

WiFi (JSON):
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

LoRa (Pipe-delimited for compactness):
```
COW-001|FARM001|female|38.5|145|37.7749|-122.4194|85|1699380000
```

### Device Config Module (`src/device_config.cpp`)
**Purpose**: Persistent configuration management

**Key Functions**:
- `loadDeviceConfig()`: Load settings from NVS
- `saveDeviceConfig()`: Save settings to NVS
- `resetConfig()`: Factory reset

**NVS Structure**:
```cpp
Namespace: "cow_config"
Keys:
  - wifi_ssid (String)
  - wifi_pass (String)
  - server_url (String)
  - lora_freq (Long)
  - lora_bw (Long)
  - lora_sf (Int)
  - farm_id (String)
  - device_id (String)
  - cow_gender (String)
```

### System Init Module (`src/system_init.cpp`)
**Purpose**: Hardware and subsystem initialization

**Key Functions**:
- `initializeSystem()`: Master initialization
- `initializeLEDs()`: Setup status indicators
- `initializePower()`: Battery monitoring setup

**Initialization Sequence**:
1. Serial communication (115200 baud)
2. Device configuration (load from NVS)
3. Status LEDs (output pins)
4. Power monitoring (ADC)
5. Sensors (temperature, GPS, accelerometer)
6. Communication (WiFi, LoRa)

## Data Structures

### HealthData (`include/health_data.h`)
```cpp
struct HealthData {
    // Identity
    char deviceId[32];
    char farmId[32];
    char gender[16];
    
    // Vital signs
    float temperature;        // °C
    int activity;            // steps/hour
    
    // Location
    float gpsLat;            // latitude
    float gpsLon;            // longitude
    float gpsAlt;            // altitude (m)
    float gpsSpeed;          // speed (km/h)
    
    // Digestion
    int soundLevel;          // 0-1023 ADC
    
    // Power
    int batteryLevel;        // 0-100%
    int solarLevel;          // 0-100%
    bool charging;           // true if charging
    
    // Status
    unsigned long timestamp; // Unix timestamp
    bool wifiConnected;      // WiFi status
    bool loraAvailable;      // LoRa status
};
```

### DeviceConfig (`include/device_config.h`)
```cpp
struct DeviceConfig {
    // WiFi
    String wifiSSID;
    String wifiPassword;
    
    // Server
    String serverURL;
    
    // LoRa
    long loraFreq;
    long loraBandwidth;
    int loraSpreadingFactor;
    
    // Identity
    String farmId;
    String deviceId;
    String cowGender;
};
```

## Pin Definitions

All pins defined in `include/config.h`:

```cpp
// Sensors
#define TEMP_PIN 4           // DS18B20 (OneWire)
#define MPU_SDA 21           // MPU6050 I2C
#define MPU_SCL 22
#define GPS_RX 16            // NEO-6M UART
#define GPS_TX 17
#define MIC_PIN 34           // MAX9814 ADC

// Communication
#define LORA_CS 5            // LoRa SPI
#define LORA_MOSI 23
#define LORA_MISO 19
#define LORA_SCK 18
// WiFi is built-in

// Power
#define BATTERY_PIN 35       // Battery voltage ADC
#define SOLAR_PIN 32         // Solar voltage ADC

// Status LEDs
#define LED_RED 25           // Alert
#define LED_YELLOW 26        // Warning
#define LED_GREEN 27         // Healthy
```

## Development Workflow

### 1. Local Development
```bash
# Clone repository
git clone <repo-url>
cd smart-farm-system/cow-node

# Install dependencies
pio lib install

# Build
pio run

# Upload to device
pio run --target upload

# Monitor serial
pio device monitor
```

### 2. Testing
```bash
# Run simulation script
./simulate.sh

# Test individual components
# In serial monitor:
TEST_SENSORS
TEST_WIFI
TEST_LORA
```

### 3. Configuration
```bash
# Interactive configuration
./configure.sh

# Or manual via serial
SET WIFI_SSID YourNetwork
SET WIFI_PASS YourPassword
SET FARM_ID FARM001
SET COW_GENDER female
```

### 4. Deployment
1. Configure device settings
2. Test all sensors
3. Verify WiFi connection
4. Verify LoRa transmission
5. Check battery charging
6. Seal in weatherproof enclosure
7. Attach to cow collar/harness
8. Monitor first 24 hours

## Code Style Guide

### Naming Conventions
- **Functions**: camelCase (`readTemperature()`)
- **Variables**: camelCase (`batteryLevel`)
- **Constants**: UPPER_SNAKE_CASE (`TEMP_PIN`)
- **Structs**: PascalCase (`HealthData`)
- **Files**: snake_case (`health_monitor.cpp`)

### Documentation
```cpp
/**
 * Read body temperature from DS18B20 sensor
 * @return Temperature in Celsius, or 0.0 if error
 */
float readTemperature() {
    // Implementation
}
```

### Error Handling
```cpp
// Always check sensor initialization
if (!sensor.begin()) {
    Serial.println("ERROR: Sensor failed to initialize");
    return DEFAULT_VALUE;
}

// Log errors clearly
if (error) {
    Serial.print("ERROR [Module]: ");
    Serial.println(errorMessage);
}
```

## Performance Considerations

### Memory Usage
- **Flash**: 988,177 bytes (75.4% of 1,310,720)
- **RAM**: 48,172 bytes (14.7% of 327,680)
- **Headroom**: ~250KB flash, ~280KB RAM available

### Power Consumption
- **Active**: ~80-120 mA (WiFi transmit)
- **LoRa**: ~20-40 mA (transmit)
- **Idle**: ~15-20 mA (sensors + sleep)
- **Deep Sleep**: ~0.15 mA (future optimization)

### Battery Life Estimates
- **Battery**: 2000 mAh 18650
- **Solar**: 5V 200mA (daylight)
- **Average Draw**: ~30 mA
- **Runtime**: ~60-70 hours (no solar)
- **With Solar**: Indefinite (sunny conditions)

## Debugging

### Serial Output Levels
```cpp
// In config.h
#define DEBUG_LEVEL_NONE 0
#define DEBUG_LEVEL_ERROR 1
#define DEBUG_LEVEL_INFO 2
#define DEBUG_LEVEL_VERBOSE 3

#define DEBUG_LEVEL DEBUG_LEVEL_INFO
```

### Common Debug Commands
```
SHOW_CONFIG      # Display all settings
TEST_SENSORS     # Read all sensors
TEST_WIFI        # Test WiFi connection
TEST_LORA        # Test LoRa transmission
RESET_CONFIG     # Factory reset
SHOW_MEMORY      # Display memory usage
```

### Serial Monitor Setup
- **Baud Rate**: 115200
- **Line Ending**: Newline (\n)
- **Encoding**: UTF-8

### LED Debug Codes
- **Red Blink**: Critical alert (high temp, low battery)
- **Yellow Blink**: Warning (low activity, battery < 20%)
- **Green Solid**: All systems healthy
- **All Off**: Power issue or boot failure
- **All On**: Initialization in progress

## Future Enhancements

### Planned Features
1. **Deep Sleep Mode**: Reduce power during inactive periods
2. **OTA Updates**: Firmware updates over WiFi
3. **Data Buffering**: Store data during communication failure
4. **Estrus Detection**: Advanced activity pattern analysis
5. **Group Behavior**: Detect social isolation
6. **Predictive Health**: ML-based illness prediction

### Hardware Upgrades
- BLE for close-range communication
- Higher capacity battery (3000+ mAh)
- More efficient solar panel
- Temperature array (multiple sensors)
- Waterproof rating IP67+

## Contributing

### Code Review Checklist
- [ ] Follows naming conventions
- [ ] Includes error handling
- [ ] Documented with comments
- [ ] Tested on hardware
- [ ] Memory usage checked
- [ ] Serial output clear
- [ ] Configuration documented

### Pull Request Template
```
## Description
[What does this PR do?]

## Changes
- [List of changes]

## Testing
- [ ] Builds successfully
- [ ] Tested on hardware
- [ ] Serial output verified
- [ ] All sensors working

## Documentation
- [ ] README updated
- [ ] Comments added
- [ ] Configuration guide updated
```

## Troubleshooting Development Issues

### Build Errors
```bash
# Clean build
pio run --target clean

# Update libraries
pio lib update

# Check platformio.ini
pio check
```

### Upload Failures
```bash
# Check port
pio device list

# Try different baud rate
# In platformio.ini:
upload_speed = 115200

# Manual erase
pio run --target erase
```

### Sensor Issues
- Check wiring with multimeter
- Verify sensor power (3.3V or 5V)
- Test sensor with example sketch
- Check pull-up resistors (I2C, OneWire)

## Resources

### Documentation
- [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [PlatformIO Docs](https://docs.platformio.org/)
- [Arduino Reference](https://www.arduino.cc/reference/en/)

### Libraries
- [OneWire](https://github.com/PaulStoffregen/OneWire)
- [DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library)
- [TinyGPSPlus](https://github.com/mbed-ce/TinyGPSPlus)
- [LoRa](https://github.com/sandeepmistry/arduino-LoRa)
- [Adafruit MPU6050](https://github.com/adafruit/Adafruit_MPU6050)

### Community
- [ESP32 Forum](https://esp32.com/)
- [PlatformIO Community](https://community.platformio.org/)
- [Arduino Forum](https://forum.arduino.cc/)

## License
MIT License - Smart Farm System 2024
