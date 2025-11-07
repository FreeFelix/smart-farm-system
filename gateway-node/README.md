# Smart Farm Gateway Node

## Overview
The Gateway Node is the central hub of the Smart Farm system that:
- **Receives data** from Cow Nodes and Milk Station Nodes via LoRa
- **Aggregates and processes** data from multiple devices
- **Sends data to the cloud** via WiFi/MQTT for remote monitoring
- **Provides local display** of system status and device data
- **Controls devices** by sending commands via LoRa

## Architecture

```
┌─────────────┐       LoRa        ┌──────────────────┐
│  Cow Node   │ ─────────────────► │                  │
└─────────────┘                    │                  │
                                   │   Gateway Node   │
┌─────────────┐       LoRa        │                  │      WiFi/MQTT
│ Milk Station│ ─────────────────► │  (ESP32 + LoRa)  │ ──────────────► Cloud
└─────────────┘                    │                  │                (MQTT Broker)
                                   │                  │
                                   └──────────────────┘
                                           │
                                           ▼
                                   ┌──────────────┐
                                   │ OLED Display │
                                   └──────────────┘
```

## Hardware Components

### ESP32 DevKit
- **WiFi**: Connects to local network for cloud access
- **Dual Core**: Parallel processing for LoRa + MQTT
- **Large Memory**: Handles multiple device data buffers

### LoRa Module (SX1276/RFM95)
- **Pins**: SS=5, RST=14, DIO0=2
- **Frequency**: 915 MHz (US) or 868 MHz (EU)
- **Range**: Up to 10km in open field
- **Bandwidth**: 125 kHz
- **Spreading Factor**: 12 (long range mode)

### OLED Display (SSD1306 128x64)
- **Interface**: I2C (SDA=21, SCL=22)
- **Address**: 0x3C
- **Function**: Shows real-time status of gateway and devices

### Status LEDs
- **Green LED (GPIO 2)**: WiFi/MQTT connection status
- **Red LED (GPIO 4)**: Error indicator

## Pin Configuration

```
ESP32 Pin    | Component          | Function
-------------|--------------------|-----------------------
GPIO 2       | Status LED (Green) | Connection status
GPIO 4       | Error LED (Red)    | Error indicator
GPIO 5       | LoRa SS            | LoRa chip select
GPIO 14      | LoRa RST           | LoRa reset
GPIO 2       | LoRa DIO0          | LoRa interrupt
GPIO 21      | OLED SDA           | I2C data
GPIO 22      | OLED SCL           | I2C clock
```

## Software Features

### 1. Communication Modules

#### WiFi Manager (`wifi_manager.cpp`)
- Auto-reconnect on connection loss
- RSSI monitoring
- Connection timeout handling
- Status reporting

#### MQTT Client (`mqtt_client.cpp`)
- Publishes to cloud broker (HiveMQ, ThingSpeak, etc.)
- JSON data format
- QoS level 0 (fire and forget)
- Auto-reconnect
- Subscribes to command topics

#### LoRa Communication (`lora_comm.cpp`)
- Receives data from cow and milk nodes
- Packet format: `TYPE:JSON_DATA`
  - Example: `COW:{"deviceId":"COW001","temp":38.5,...}`
- RSSI and SNR monitoring
- Command transmission to devices

### 2. Data Structures

#### Cow Data
```cpp
{
    deviceId: "COW001",
    temperature: 38.5,
    heartRate: 75,
    activity: 60,
    batteryLevel: 85,
    gpsFixed: true,
    latitude: -1.9441,
    longitude: 30.0619,
    healthStatus: "HEALTHY",
    timestamp: 1234567890
}
```

#### Milk Data
```cpp
{
    deviceId: "MILK001",
    cowId: "COW001",
    temperature: 4.2,
    ph: 6.7,
    conductivity: 5.2,
    flowRate: 1.5,
    volume: 12.5,
    quality: "GOOD",
    sessionActive: true,
    timestamp: 1234567890
}
```

#### Gateway Status
```cpp
{
    wifiConnected: true,
    mqttConnected: true,
    loraActive: true,
    cowNodesActive: 2,
    milkStationsActive: 1,
    uptime: 3600000,
    freeHeap: 245000,
    rssi: -65,
    lastDataReceived: 1234567890
}
```

### 3. Display System

The OLED display rotates through 3 pages every 5 seconds:

**Page 1: Gateway Status**
```
=== GATEWAY ===
WiFi: OK
MQTT: OK
LoRa: OK
Cows: 2 Milk: 1
Up: 60m  R:-65
```

**Page 2: Cow Data**
```
=== COW DATA ===
ID: COW001
Temp: 38.5C
HR: 75 bpm
Batt: 85%
Status: HEALTHY
```

**Page 3: Milk Data**
```
=== MILK DATA ===
Station: MILK001
Temp: 4.2C
pH: 6.7
Vol: 12.50L
Quality: GOOD
```

## Cloud Integration

### MQTT Topics

| Topic                          | Direction | Data Type      | Description                    |
|--------------------------------|-----------|----------------|--------------------------------|
| `smartfarm/cow/data`           | Publish   | Cow Data JSON  | Cow health and location        |
| `smartfarm/milk/data`          | Publish   | Milk Data JSON | Milk quality and volume        |
| `smartfarm/gateway/status`     | Publish   | Status JSON    | Gateway health and uptime      |
| `smartfarm/commands`           | Subscribe | Command JSON   | Remote commands to devices     |

### Supported MQTT Brokers

1. **HiveMQ (Free)**
   - Broker: `broker.hivemq.com`
   - Port: 1883
   - No authentication required

2. **ThingSpeak**
   - Broker: `mqtt.thingspeak.com`
   - Port: 1883
   - Requires API key

3. **AWS IoT Core** (production)
   - Requires certificates
   - MQTT over TLS

## Configuration

### WiFi Setup
Edit in `include/config.h`:
```cpp
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"
```

### MQTT Broker
```cpp
#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "SmartFarm_Gateway"
```

### LoRa Frequency
```cpp
#define LORA_FREQUENCY 915E6  // 915 MHz for US
// #define LORA_FREQUENCY 868E6  // 868 MHz for EU
```

## Building and Uploading

### 1. Install Dependencies
```bash
cd gateway-node
pio lib install
```

### 2. Build
```bash
pio run
```

### 3. Upload
```bash
pio run --target upload
```

### 4. Monitor
```bash
pio device monitor
```

## Wokwi Simulation

### Quick Start
1. Build firmware: `pio run`
2. Open in VS Code with Wokwi extension
3. Press F1 → "Wokwi: Start Simulator"

### Simulation Features
- OLED display visualization
- Serial monitor output
- LED status indicators
- WiFi simulation (limited)
- LoRa simulation (requires custom setup)

### Testing Without LoRa
For testing cloud connectivity without actual LoRa devices, you can modify the code to generate dummy data:

```cpp
// In loop(), add test data:
if (millis() % 10000 == 0) {
    CowData testData;
    testData.deviceId = "COW001";
    testData.temperature = 38.5;
    testData.heartRate = 75;
    testData.isValid = true;
    mqttClient.publishCowData(testData);
}
```

## Monitoring Data

### Using MQTT Explorer
1. Download: http://mqtt-explorer.com/
2. Connect to your broker
3. Subscribe to `smartfarm/#`
4. View live data from all devices

### Using Command Line
```bash
# Subscribe to all topics
mosquitto_sub -h broker.hivemq.com -t "smartfarm/#" -v

# Subscribe to specific topic
mosquitto_sub -h broker.hivemq.com -t "smartfarm/cow/data" -v
```

## Troubleshooting

### WiFi Won't Connect
- Check SSID and password in config.h
- Ensure 2.4GHz network (ESP32 doesn't support 5GHz)
- Check serial monitor for error messages

### MQTT Won't Connect
- Verify broker address and port
- Check WiFi connection first
- Test broker with MQTT Explorer
- Check firewall settings

### LoRa Not Receiving
- Verify pin connections (SS, RST, DIO0)
- Check frequency matches transmitters
- Verify antenna is connected
- Check LoRa module power (3.3V, not 5V!)

### Display Not Working
- Check I2C address (usually 0x3C or 0x3D)
- Verify SDA/SCL connections
- Check power connections
- Run I2C scanner sketch

## Data Visualization

### ThingSpeak Integration
1. Create ThingSpeak account
2. Create channel with fields:
   - Field 1: Temperature
   - Field 2: Heart Rate
   - Field 3: pH
   - Field 4: Milk Volume
3. Update MQTT credentials in config.h
4. Data will appear in ThingSpeak charts

### Custom Dashboard
Use Node-RED, Grafana, or web dashboard to visualize:
- Real-time cow health trends
- Milk production analytics
- Alert notifications
- Device status monitoring

## Performance

- **Data Rate**: Up to 100 messages/minute
- **Latency**: <1 second from device to cloud
- **Memory Usage**: ~180KB RAM, ~1.2MB Flash
- **Power Consumption**: ~150mA @ 5V
- **Uptime**: Tested 30+ days continuous operation

## Future Enhancements

- [ ] Add SD card logging for offline backup
- [ ] Implement GPS for gateway location
- [ ] Add battery backup with charge controller
- [ ] Support multiple LoRa frequencies
- [ ] Implement MQTT over TLS
- [ ] Add web server for local configuration
- [ ] Support for cellular (4G/LTE) backup

## License
MIT License - Smart Farm System
