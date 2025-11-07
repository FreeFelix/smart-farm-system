# Gateway Node - Quick Setup Guide

## 📋 Prerequisites

### Hardware Required
- ESP32 DevKit V1
- LoRa Module (SX1276 / RFM95W)
- OLED Display (SSD1306 128x64 I2C)
- 2x LEDs (Green + Red)
- 2x 220Ω Resistors
- Breadboard and jumper wires
- WiFi network access

### Software Required
- VS Code with PlatformIO extension
- Git (optional)

## 🔌 Hardware Connections

### LoRa Module → ESP32
```
LoRa Pin    ESP32 Pin
--------    ---------
VCC    →    3.3V
GND    →    GND
MISO   →    GPIO 19 (auto)
MOSI   →    GPIO 23 (auto)
SCK    →    GPIO 18 (auto)
NSS    →    GPIO 5
RST    →    GPIO 14
DIO0   →    GPIO 2
```

### OLED Display → ESP32
```
OLED Pin    ESP32 Pin
--------    ---------
VCC    →    3.3V
GND    →    GND
SDA    →    GPIO 21
SCL    →    GPIO 22
```

### Status LEDs → ESP32
```
Component        ESP32 Pin
---------        ---------
Green LED (+) → GPIO 2
Green LED (-) → 220Ω → GND
Red LED (+)   → GPIO 4
Red LED (-)   → 220Ω → GND
```

## ⚙️ Software Configuration

### Step 1: Configure WiFi

Edit `include/config.h`:

```cpp
#define WIFI_SSID "YourNetworkName"
#define WIFI_PASSWORD "YourNetworkPassword"
```

### Step 2: Configure MQTT Broker

For **testing** (free broker):
```cpp
#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
```

For **ThingSpeak**:
```cpp
#define MQTT_BROKER "mqtt.thingspeak.com"
#define MQTT_PORT 1883
#define MQTT_USERNAME "YourChannelID"
#define MQTT_PASSWORD "YourWriteAPIKey"
```

### Step 3: Configure LoRa Frequency

For **US/Americas**:
```cpp
#define LORA_FREQUENCY 915E6  // 915 MHz
```

For **Europe/Africa**:
```cpp
#define LORA_FREQUENCY 868E6  // 868 MHz
```

## 🚀 Building and Uploading

### 1. Open Project
```bash
cd gateway-node
code .
```

### 2. Install Dependencies
PlatformIO will automatically install:
- WiFi libraries
- PubSubClient (MQTT)
- ArduinoJson
- Adafruit SSD1306
- Adafruit GFX
- LoRa library

### 3. Build
```bash
pio run
```

### 4. Upload to ESP32
```bash
pio run --target upload
```

### 5. Monitor Serial Output
```bash
pio device monitor
```

## 📊 Expected Serial Output

```
========================================
   SMART FARM GATEWAY SYSTEM v1.0
========================================

Step 1/4: Initializing Display...
✓ OLED Display Initialized!

Step 2/4: Initializing LoRa...
✓ LoRa Initialized!
Frequency: 915.0 MHz

Step 3/4: Connecting to WiFi...
Connecting to WiFi...
SSID: YourNetwork
....
✓ WiFi Connected!
IP Address: 192.168.1.100
RSSI: -65 dBm

Step 4/4: Connecting to MQTT Broker...
Connecting to MQTT broker: broker.hivemq.com
✓ MQTT Connected!
Subscribed to: smartfarm/commands

========================================
   GATEWAY SYSTEM READY!
========================================

Waiting for data from devices...
```

## 📱 Testing the System

### Test 1: Check Display
The OLED should show:
```
=== GATEWAY ===
WiFi: OK
MQTT: OK
LoRa: OK
Cows: 0 Milk: 0
Up: 0m  R:-65
```

### Test 2: Check Status LEDs
- **Green LED**: Blinking = connected to WiFi + MQTT
- **Red LED**: Off when everything OK

### Test 3: Check MQTT Connection

Using **MQTT Explorer**:
1. Download from http://mqtt-explorer.com/
2. Connect to `broker.hivemq.com`
3. Subscribe to `smartfarm/#`
4. You should see gateway status messages

Using **Command Line**:
```bash
mosquitto_sub -h broker.hivemq.com -t "smartfarm/gateway/status" -v
```

### Test 4: Simulate Receiving Data

**Option A**: Wait for cow-node or milk-station to send data via LoRa

**Option B**: Add test data to `main.cpp` for testing:

```cpp
// Add this in loop() for testing
static unsigned long lastTest = 0;
if (millis() - lastTest > 10000) {
    lastTest = millis();
    
    // Create test cow data
    CowData testCow;
    testCow.deviceId = "COW001";
    testCow.temperature = 38.5;
    testCow.heartRate = 75;
    testCow.activity = 60;
    testCow.batteryLevel = 85;
    testCow.healthStatus = "HEALTHY";
    testCow.isValid = true;
    
    mqttClient.publishCowData(testCow);
    Serial.println("Test data published!");
}
```

## 🔍 Monitoring Cloud Data

### Using MQTT Explorer
1. Connect to your MQTT broker
2. Subscribe to topics:
   - `smartfarm/cow/data`
   - `smartfarm/milk/data`
   - `smartfarm/gateway/status`
3. Watch live JSON data appear

### Using Web Dashboard

**ThingSpeak** (built-in charts):
1. Create account at https://thingspeak.com
2. Create new channel
3. Configure MQTT as shown in Step 2
4. View automatic charts

**Node-RED** (custom dashboard):
```bash
npm install -g node-red
node-red
# Open http://localhost:1880
# Add MQTT input nodes for smartfarm topics
# Add chart nodes for visualization
```

## 🐛 Troubleshooting

### ❌ WiFi Won't Connect
**Symptoms**: Serial shows "WiFi Connection Failed"

**Solutions**:
- Check SSID and password (case-sensitive!)
- Ensure 2.4 GHz network (ESP32 doesn't support 5 GHz)
- Move closer to router
- Check if network uses captive portal (won't work)

### ❌ MQTT Won't Connect
**Symptoms**: Serial shows "MQTT Connection Failed, rc=-2"

**Solutions**:
- Verify WiFi is connected first
- Check broker address
- Test broker with MQTT Explorer first
- For ThingSpeak, verify API keys
- Check firewall settings

### ❌ Display Shows Nothing
**Symptoms**: OLED is blank

**Solutions**:
- Check I2C connections (SDA, SCL)
- Verify power (3.3V or 5V depending on module)
- Try changing I2C address to 0x3D in config.h
- Run I2C scanner to find address

### ❌ LoRa Not Receiving
**Symptoms**: No data from cow/milk nodes

**Solutions**:
- Verify LoRa module connections
- Check frequency matches sender (915 vs 868 MHz)
- Ensure antenna is connected
- Check LoRa module power (must be 3.3V!)
- Verify sending devices are powered on
- Check LoRa spreading factor and bandwidth match

### ❌ Red LED Blinking Fast
**Symptoms**: Error LED flashing rapidly

**Solutions**:
- Check serial monitor for errors
- WiFi or MQTT connection lost
- Restart ESP32
- Verify configuration

## 📈 Performance Metrics

**Normal Operation**:
- WiFi RSSI: -30 to -70 dBm (good)
- Free Heap: 200KB - 250KB
- Uptime: Days to weeks
- Packet Loss: <1%

**Warning Signs**:
- WiFi RSSI: < -80 dBm (move closer)
- Free Heap: < 100KB (memory leak?)
- Frequent reconnections (power issue?)

## 🔄 System Integration

### Data Flow
```
Cow Node → LoRa → Gateway → MQTT → Cloud Dashboard
                     ↓
                   OLED Display
                     
Milk Station → LoRa → Gateway → MQTT → Cloud Dashboard
```

### Next Steps
1. ✅ Configure cow-node to send data
2. ✅ Configure milk-station-node to send data
3. ✅ Set up cloud dashboard
4. ✅ Configure alerts and notifications
5. ✅ Deploy in farm environment

## 📚 Additional Resources

- **Full Documentation**: See `README.md`
- **API Reference**: Check header files in `include/`
- **Example Dashboards**: See `examples/` (to be created)
- **Troubleshooting**: See README.md Troubleshooting section

## 🆘 Support

If you encounter issues:
1. Check serial monitor output
2. Verify all connections
3. Test each component individually
4. Review configuration in `config.h`
5. Check README.md for detailed troubleshooting

## ✅ System Checklist

- [ ] Hardware connected correctly
- [ ] WiFi credentials configured
- [ ] MQTT broker configured
- [ ] LoRa frequency set correctly
- [ ] Firmware built successfully
- [ ] Firmware uploaded successfully
- [ ] WiFi connected
- [ ] MQTT connected
- [ ] LoRa initialized
- [ ] Display working
- [ ] Can see data in MQTT Explorer
- [ ] Receiving data from cow/milk nodes

Once all items are checked, your gateway is **FULLY OPERATIONAL**! 🎉
