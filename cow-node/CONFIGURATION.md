# Cow Node Configuration Guide

## Overview
The cow health monitoring node uses ESP32 NVS (Non-Volatile Storage) to persist configuration across power cycles. This guide covers all configurable parameters and how to set them.

## Configuration Methods

### Method 1: Interactive Script (Recommended)
```bash
./configure.sh
```

The interactive script guides you through all settings:
1. WiFi credentials
2. Device Manager server details
3. LoRa radio parameters
4. Farm identification
5. Cow-specific settings

### Method 2: Serial Monitor
Connect via serial (115200 baud) and use configuration commands:
```
# WiFi Settings
SET WIFI_SSID YourFarmNetwork
SET WIFI_PASS SecurePassword123

# Server Settings
SET SERVER_URL http://192.168.1.100:8080/api/health

# LoRa Settings
SET LORA_FREQ 915000000
SET LORA_BW 125000
SET LORA_SF 7

# Farm Settings
SET FARM_ID FARM001
SET COW_GENDER female
```

### Method 3: Direct Code Modification
Edit `src/device_config.cpp` and hardcode defaults (not recommended for production):
```cpp
void setDefaultConfig() {
    prefs.putString("wifi_ssid", "YourSSID");
    prefs.putString("wifi_pass", "YourPassword");
    // ... etc
}
```

## Configuration Parameters

### WiFi Settings

#### WIFI_SSID
- **Description**: WiFi network name
- **Type**: String (max 32 characters)
- **Example**: `"SmartFarm_WiFi"`
- **Default**: `""`
- **Required**: Yes (for WiFi communication)

#### WIFI_PASS
- **Description**: WiFi network password
- **Type**: String (max 64 characters)
- **Example**: `"SecureFarmPass123"`
- **Default**: `""`
- **Required**: Yes (for WiFi communication)

### Device Manager Settings

#### SERVER_URL
- **Description**: HTTP endpoint for health data
- **Type**: String (full URL)
- **Example**: `"http://192.168.1.100:8080/api/health"`
- **Default**: `"http://192.168.1.1:8080/api/health"`
- **Required**: Yes (for WiFi transmission)
- **Format**: `http://[IP]:[PORT]/[PATH]`

### LoRa Radio Settings

#### LORA_FREQ
- **Description**: LoRa transmission frequency
- **Type**: Long integer (Hz)
- **Example**: `915000000` (915 MHz, US)
- **Alternatives**: 
  - `868000000` (868 MHz, EU)
  - `433000000` (433 MHz, Asia)
- **Default**: `915000000`
- **Required**: Yes
- **Note**: Must match gateway frequency

#### LORA_BW
- **Description**: LoRa signal bandwidth
- **Type**: Long integer (Hz)
- **Options**: 
  - `7800` (7.8 kHz)
  - `10400` (10.4 kHz)
  - `15600` (15.6 kHz)
  - `20800` (20.8 kHz)
  - `31250` (31.25 kHz)
  - `41700` (41.7 kHz)
  - `62500` (62.5 kHz)
  - `125000` (125 kHz, recommended)
  - `250000` (250 kHz)
  - `500000` (500 kHz)
- **Default**: `125000`
- **Trade-off**: Lower = longer range, slower data rate

#### LORA_SF
- **Description**: LoRa spreading factor
- **Type**: Integer
- **Range**: 6-12
- **Example**: `7` (recommended for short range, fast)
- **Default**: `7`
- **Trade-off**: Higher = longer range, more power, slower
- **Typical Values**:
  - `7`: Fast, short range (~2km)
  - `10`: Balanced (~10km)
  - `12`: Maximum range (~15km), very slow

### Farm Identification

#### FARM_ID
- **Description**: Unique farm identifier
- **Type**: String (max 16 characters)
- **Example**: `"FARM001"`, `"DAIRY-WEST"`
- **Default**: `"FARM001"`
- **Required**: Yes
- **Format**: Alphanumeric, no spaces

#### DEVICE_ID
- **Description**: Unique cow/device identifier
- **Type**: String (max 16 characters)
- **Example**: `"COW-001"`, `"BESSIE"`
- **Default**: Auto-generated from MAC address
- **Required**: Yes (auto-set if blank)
- **Format**: Alphanumeric, hyphen allowed

### Cow-Specific Settings

#### COW_GENDER
- **Description**: Biological gender of cow
- **Type**: String
- **Options**: 
  - `"female"` (dairy cow, produces milk)
  - `"male"` (bull, no milk production)
- **Default**: `"female"`
- **Required**: Yes
- **Usage**: Used for health threshold adjustments

**Note**: Milk production is NOT tracked on cow node. When cow visits milk station, RFID reader identifies cow and milk data is recorded at the station. This field helps with health analysis only.

### Monitoring Intervals

#### HEALTH_CHECK_INTERVAL
- **Description**: How often to check health metrics
- **Type**: Unsigned long (milliseconds)
- **Default**: `60000` (60 seconds)
- **Range**: 10000-300000 (10 sec - 5 min)
- **Code Location**: `include/config.h`

#### WIFI_RECONNECT_INTERVAL
- **Description**: How often to attempt WiFi reconnect
- **Type**: Unsigned long (milliseconds)
- **Default**: `30000` (30 seconds)
- **Code Location**: `src/main.cpp`

#### LORA_TRANSMIT_INTERVAL
- **Description**: How often to send LoRa broadcasts
- **Type**: Unsigned long (milliseconds)
- **Default**: `60000` (60 seconds)
- **Code Location**: `src/communication.cpp`

## Health Thresholds

### Temperature (Celsius)
```cpp
// In include/config.h
#define TEMP_LOW_THRESHOLD 37.5   // Below this = hypothermia alert
#define TEMP_HIGH_THRESHOLD 39.5  // Above this = fever alert
#define TEMP_NORMAL_MIN 38.0      // Normal range minimum
#define TEMP_NORMAL_MAX 39.0      // Normal range maximum
```

### Activity (Steps/Hour)
```cpp
#define ACTIVITY_LOW_THRESHOLD 50  // Below this = possible illness
#define ACTIVITY_NORMAL_MIN 50     // Normal activity threshold
```

### Battery (Percent)
```cpp
#define BATTERY_LOW 20             // Warning level
#define BATTERY_CRITICAL 10        // Critical alert level
```

### GPS Accuracy
```cpp
#define GPS_HDOP_THRESHOLD 2.0     // Maximum HDOP for valid fix
```

## Configuration Workflow

### Initial Setup (New Device)
1. **Power on device** - LEDs will blink (no config)
2. **Connect serial monitor** (115200 baud)
3. **Run configuration script**: `./configure.sh`
4. **Enter WiFi credentials** when prompted
5. **Enter Device Manager URL**
6. **Set LoRa frequency** (915 MHz for US)
7. **Enter Farm ID**
8. **Select cow gender**
9. **Device reboots** with new config
10. **Verify connection** - Green LED = WiFi connected

### Updating Existing Configuration
1. **Connect serial monitor**
2. **Run**: `./configure.sh`
3. **Select setting to update**
4. **Enter new value**
5. **Device saves to NVS** (persists across reboots)

### Factory Reset
```bash
# Via serial monitor
RESET_CONFIG

# Or reflash firmware
pio run --target upload
```

## Verification

### Check Current Configuration
Connect to serial monitor and send:
```
SHOW_CONFIG
```

Output example:
```
=== Current Configuration ===
WiFi SSID: SmartFarm_WiFi
WiFi Connected: Yes
Server URL: http://192.168.1.100:8080/api/health
LoRa Frequency: 915 MHz
LoRa Bandwidth: 125 kHz
LoRa Spreading Factor: 7
Farm ID: FARM001
Device ID: COW-001
Cow Gender: female
============================
```

### Test Communication
```
TEST_WIFI     # Test WiFi HTTP POST
TEST_LORA     # Test LoRa transmission
TEST_SENSORS  # Read all sensors
```

## Common Configuration Scenarios

### Scenario 1: Indoor Barn with Strong WiFi
```
WiFi: Enabled (primary)
LoRa: Enabled (backup)
Health Check: 60 seconds
WiFi Reconnect: 30 seconds
```

### Scenario 2: Remote Pasture (WiFi Unreliable)
```
WiFi: Enabled (attempt connection)
LoRa: Enabled (primary - longer range)
Health Check: 120 seconds (save battery)
LoRa SF: 10 (longer range)
```

### Scenario 3: Battery Conservation Mode
```
Health Check: 300 seconds (5 minutes)
WiFi Reconnect: 60 seconds
LoRa SF: 7 (faster, less power)
Disable GPS if not needed
```

## Troubleshooting Configuration

### Config Not Saving
- **Symptom**: Settings reset after reboot
- **Cause**: NVS partition issue
- **Fix**: Erase flash and re-upload
```bash
pio run --target erase
pio run --target upload
```

### WiFi Not Connecting
- **Check**: SSID spelling (case-sensitive)
- **Check**: Password special characters
- **Check**: WiFi router signal strength
- **Check**: Router MAC filtering (add ESP32 MAC)

### LoRa Not Working
- **Check**: Frequency matches gateway
- **Check**: Antenna connected
- **Check**: SPI wiring (CS, MOSI, MISO, SCK)
- **Check**: LoRa module power supply (3.3V stable)

### Invalid Device ID
- **Symptom**: Device ID shows as "unknown"
- **Cause**: MAC address read failure
- **Fix**: Manually set DEVICE_ID
```
SET DEVICE_ID COW-001
```

## Security Considerations

### WiFi Password Storage
- Passwords stored in NVS (encrypted if enabled)
- Enable flash encryption for production:
```ini
# In platformio.ini
board_build.flash_mode = dio
board_build.partitions = default_16MB.csv
```

### Device Manager Authentication
- Use HTTPS instead of HTTP for production
- Implement API key authentication:
```cpp
client.addHeader("X-API-Key", "YourSecretKey");
```

### LoRa Data Encryption
- LoRa broadcasts are unencrypted by default
- For sensitive data, implement AES encryption
- Share encryption key between nodes and gateway

## Best Practices

1. **Document your settings** - Keep a configuration spreadsheet
2. **Use consistent naming** - FARM001, FARM002, etc.
3. **Test before deployment** - Verify all sensors and communication
4. **Backup configurations** - Save serial output of SHOW_CONFIG
5. **Label devices physically** - Match device labels to DEVICE_ID
6. **Monitor battery levels** - Replace/recharge before critical
7. **Regular updates** - Check firmware updates monthly

## Configuration File Reference

All settings stored in ESP32 NVS namespace "cow_config":

| Key | Type | Max Size | Required |
|-----|------|----------|----------|
| wifi_ssid | String | 32 | Yes |
| wifi_pass | String | 64 | Yes |
| server_url | String | 128 | Yes |
| lora_freq | Long | 8 | Yes |
| lora_bw | Long | 8 | Yes |
| lora_sf | Int | 4 | Yes |
| farm_id | String | 16 | Yes |
| device_id | String | 16 | Auto |
| cow_gender | String | 8 | Yes |

## Support
For configuration assistance, contact your farm IT administrator.
