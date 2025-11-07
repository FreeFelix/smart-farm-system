# Milk Station Node - Device Configuration Guide

## Overview
The Milk Station Node is a fixed milking station equipped with RFID reader, milk sensors, LCD display, and dual communication (WiFi + LoRa).

## Quick Start - Simulation

### 1. Run the Simulation
```bash
cd /home/admino/smart-farm-system/milk-station-node
./simulate.sh
```

This will simulate 3 complete milking sessions with:
- RFID tag scanning
- Milk production monitoring
- Quality analysis (temperature, pH, conductivity)
- LCD display updates
- LoRa data transmission

---

## Device Configuration Commands

### Available Serial Commands

When the device is running (via simulation or actual hardware), you can send these commands via Serial Monitor:

#### Display Help
```
HELP
```

#### Show Current Configuration
```
SHOW
```

#### Reset to Factory Defaults
```
RESET
```

#### Save Current Configuration
```
SAVE
```

---

## Configuration Options

### 1. Device Identity

#### Set Device Name
```
SET_NAME Milk Station A
```

#### Set Farm ID
```
SET_FARM FARM001
```

### 2. WiFi Configuration

#### Configure WiFi Credentials
```
SET_WIFI MyWiFiSSID MyPassword123
```

#### Enable/Disable WiFi
```
WIFI_ON
WIFI_OFF
```

#### Set Device Manager Server
```
SET_MANAGER 192.168.1.100 8080
```

### 3. LoRa Configuration

#### Configure LoRa Parameters
```
SET_LORA 915000000 0x12 7
```
- Frequency: 915 MHz (915000000 Hz)
- Sync Word: 0x12
- Spread Factor: 7

#### Enable/Disable LoRa
```
LORA_ON
LORA_OFF
```

---

## Example Configuration Workflow

### Initial Setup for a New Milk Station

1. **Power on the device** and connect via Serial Monitor (115200 baud)

2. **View current configuration**:
   ```
   SHOW
   ```

3. **Set device identity**:
   ```
   SET_NAME Milk Station North
   SET_FARM DAIRY_FARM_001
   ```

4. **Configure WiFi**:
   ```
   SET_WIFI FarmWiFi SecurePass2024
   SET_MANAGER 192.168.10.50 8080
   WIFI_ON
   ```

5. **Configure LoRa** (ensure all devices use same settings):
   ```
   SET_LORA 915000000 0x12 7
   LORA_ON
   ```

6. **Save configuration**:
   ```
   SAVE
   ```

7. **Verify settings**:
   ```
   SHOW
   ```

---

## Configuration Examples

### Example 1: WiFi-Only Station
```
SET_NAME Station WiFi Only
SET_WIFI CompanyNetwork Pass1234
SET_MANAGER 10.0.0.100 8080
WIFI_ON
LORA_OFF
SAVE
```

### Example 2: LoRa-Only Station (Remote Location)
```
SET_NAME Remote Station Alpha
WIFI_OFF
SET_LORA 915000000 0x12 8
LORA_ON
SAVE
```

### Example 3: Dual Communication (Recommended)
```
SET_NAME Main Milking Station
SET_FARM HIGHLAND_DAIRY
SET_WIFI FarmNetwork SecureKey789
SET_MANAGER 192.168.1.10 8080
SET_LORA 915000000 0x12 7
WIFI_ON
LORA_ON
SAVE
```

---

## Device Configuration Storage

Configuration is stored in ESP32's NVS (Non-Volatile Storage) and persists across reboots.

### Configuration Parameters Stored:
- Device ID (auto-generated from MAC address)
- Device Name
- Farm ID
- Device Type (Milk Station)
- WiFi SSID and Password
- Device Manager IP and Port
- LoRa Frequency, Sync Word, Spread Factor
- Enable/Disable flags for WiFi and LoRa

---

## Monitoring During Operation

### Serial Output During Milking Session

The device provides real-time feedback:

```
Starting milking session for cow: COW123
Solenoid valve OPENED

Volume: 2.35 L | Weight: 2.43 kg
Flow: 1.85 L/min | Temp: 36.8°C
pH: 6.65 | Cond: 4.92 mS/cm
Quality: 90%

Milking session complete
Solenoid valve CLOSED

Data sent to manager, response: 200
LoRa: Data transmitted successfully
```

---

## Troubleshooting

### WiFi Not Connecting
1. Check SSID and password with `SHOW` command
2. Verify WiFi is enabled: `WIFI_ON`
3. Reconfigure: `SET_WIFI YourSSID YourPassword`
4. Save: `SAVE`

### LoRa Not Transmitting
1. Verify LoRa is enabled: `LORA_ON`
2. Check frequency matches other devices
3. Ensure antenna is connected properly
4. Check sync word matches network: `SET_LORA 915000000 0x12 7`

### Configuration Lost After Reboot
1. Always run `SAVE` after making changes
2. If still occurring, try `RESET` then reconfigure
3. Check NVS partition in platformio.ini

---

## Hardware Pin Configuration

As defined in `include/config.h`:

### RFID Reader (SPI)
- SCK: GPIO 18
- MISO: GPIO 19
- MOSI: GPIO 23
- SS: GPIO 5
- RST: GPIO 22

### LCD Display (I2C)
- SDA: GPIO 21
- SCL: GPIO 22
- Address: 0x27

### Milk Sensors
- Flow Meter: GPIO 4
- Weight Sensor (HX711): DOUT=GPIO 16, SCK=GPIO 17
- Temperature (DS18B20): GPIO 15
- pH Sensor: GPIO 34
- Conductivity: GPIO 35

### LoRa Module (SPI)
- Uses same SPI bus as RFID
- SS: GPIO 27
- RST: GPIO 14
- DIO0: GPIO 26

### Control & Indicators
- Solenoid Valve: GPIO 25
- Status LED: GPIO 2
- Alert LED: GPIO 13

---

## Next Steps

1. **Run Simulation**: `./simulate.sh` to see the system in action
2. **Upload to Hardware**: `pio run --target upload`
3. **Monitor Serial**: `pio device monitor` to configure the device
4. **Test RFID**: Place a tag near the reader
5. **Monitor Sessions**: Watch LCD display and serial output

---

## Network Integration

### WiFi Mode
- Device connects to local WiFi network
- Sends data to Device Manager via HTTP POST
- Endpoint: `http://<manager-ip>:<port>/api/milk-data`

### LoRa Mode
- Broadcasts data to Gateway Node
- Gateway forwards to Device Manager
- Lower power consumption
- Longer range (up to 10km)

### Dual Mode (Recommended)
- Primary: WiFi for fast, reliable transmission
- Backup: LoRa when WiFi unavailable
- Automatic failover

---

## Data Format

### LoRa Transmission Format
```
MILK|COW123|2.45|2.53|6.67|36.5|4.85|0.92
```

Fields:
1. Type: MILK
2. Cow ID: COW123
3. Volume (L): 2.45
4. Weight (kg): 2.53
5. pH: 6.67
6. Temperature (°C): 36.5
7. Conductivity (mS/cm): 4.85
8. Quality Score: 0.92 (0-1)

### WiFi JSON Format
```json
{
  "deviceID": "MILK-a3f2b1c0",
  "farmID": "FARM001",
  "data": "MILK|COW123|2.45|2.53|6.67|36.5|4.85|0.92"
}
```

---

## Support

For issues or questions:
1. Check this guide
2. Review serial output for error messages
3. Use `SHOW` command to verify configuration
4. Try `RESET` and reconfigure if needed
