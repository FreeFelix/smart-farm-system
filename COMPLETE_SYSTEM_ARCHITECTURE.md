# Smart Farm System - Complete Architecture

## System Overview

The Smart Farm System consists of three main nodes working together to monitor dairy cow health and milk quality:

```
                    ┌─────────────────────────────────────────────┐
                    │         SMART FARM SYSTEM                  │
                    └─────────────────────────────────────────────┘
                                        │
                    ┌───────────────────┴───────────────────┐
                    │                                        │
            ┌───────▼────────┐                    ┌────────▼────────┐
            │   COW NODES    │                    │  MILK STATIONS  │
            │  (Wearables)   │                    │   (Stationary)  │
            └───────┬────────┘                    └────────┬────────┘
                    │                                      │
                    │         LoRa Communication           │
                    │              (915/868 MHz)           │
                    │                                      │
                    └──────────────┬───────────────────────┘
                                   │
                         ┌─────────▼──────────┐
                         │  GATEWAY NODE      │
                         │  (Central Hub)     │
                         └─────────┬──────────┘
                                   │
                            WiFi / 4G / Ethernet
                                   │
                         ┌─────────▼──────────┐
                         │   CLOUD / MQTT     │
                         │     BROKER         │
                         └─────────┬──────────┘
                                   │
                    ┌──────────────┼──────────────┐
                    │              │              │
          ┌─────────▼────────┐    │    ┌────────▼─────────┐
          │  Web Dashboard   │    │    │  Mobile App      │
          │  (Monitoring)    │    │    │  (Alerts)        │
          └──────────────────┘    │    └──────────────────┘
                                  │
                         ┌────────▼─────────┐
                         │   Data Analytics │
                         │   & ML Models    │
                         └──────────────────┘
```

## Node Details

### 1. COW NODE (Wearable Health Monitor)

**Purpose**: Real-time health monitoring of individual cows

**Hardware**:
- ESP32 microcontroller
- DS18B20 temperature sensor
- Pulse sensor (heart rate)
- MPU6050 accelerometer (activity tracking)
- GPS module (NEO-6M)
- LoRa module (RFM95)
- Battery pack (3.7V LiPo)
- 3x Status LEDs (Red/Yellow/Green)

**Data Collected**:
- Body temperature (°C)
- Heart rate (bpm)
- Activity level (%)
- GPS location (lat/lon)
- Battery level (%)
- Health status (HEALTHY/WARNING/CRITICAL)

**Transmission**:
- LoRa packets every 60 seconds
- Emergency alerts immediately
- Format: `COW:{json_data}`

**Location**: `/cow-node/`

---

### 2. MILK STATION NODE (Quality Monitor)

**Purpose**: Real-time milk quality and quantity monitoring

**Hardware**:
- ESP32 microcontroller
- DS18B20 temperature sensor
- pH sensor (analog)
- Conductivity sensor (analog)
- Flow sensor (digital pulse)
- RFID reader (RC522)
- LCD2004 display (I2C)
- LoRa module (RFM95)
- 2x Status LEDs (Green/Red)

**Data Collected**:
- Milk temperature (°C)
- pH level (6.0-7.0)
- Conductivity (mS/cm)
- Flow rate (L/min)
- Total volume (L)
- Cow ID (via RFID)
- Quality grade (GOOD/FAIR/POOR)
- Session status (active/inactive)

**Transmission**:
- LoRa packets during milking session
- Quality alerts immediately
- Format: `MILK:{json_data}`

**Display**:
- Real-time LCD feedback
- Volume, quality, temperature
- Cow identification

**Location**: `/milk-station-node/`

---

### 3. GATEWAY NODE (Central Hub)

**Purpose**: Data aggregation and cloud connectivity

**Hardware**:
- ESP32 microcontroller (dual-core)
- LoRa module (RFM95)
- WiFi (built-in ESP32)
- OLED display SSD1306 (128x64)
- 2x Status LEDs (Green/Red)

**Functions**:
- Receive LoRa data from all nodes
- Aggregate and validate data
- Publish to MQTT cloud broker
- Display system status locally
- Send commands to nodes
- Monitor network health

**Cloud Integration**:
- MQTT protocol
- JSON data format
- QoS level 0 (fire and forget)
- Auto-reconnect

**Location**: `/gateway-node/`

---

## Communication Protocol

### LoRa Communication

**Parameters**:
- Frequency: 915 MHz (US) / 868 MHz (EU)
- Spreading Factor: 12 (long range)
- Bandwidth: 125 kHz
- Coding Rate: 4/5
- Sync Word: 0x34 (private network)
- Range: Up to 10 km (open field)

**Packet Format**:
```
TYPE:JSON_DATA

Examples:
COW:{"deviceId":"COW001","temp":38.5,"hr":75,...}
MILK:{"deviceId":"MILK001","cowId":"COW001","ph":6.7,...}
CMD:COW001:RESET
```

### MQTT Topics

| Topic                          | Publisher  | Data                  | QoS |
|--------------------------------|------------|-----------------------|-----|
| `smartfarm/cow/data`           | Gateway    | Cow health JSON       | 0   |
| `smartfarm/milk/data`          | Gateway    | Milk quality JSON     | 0   |
| `smartfarm/gateway/status`     | Gateway    | Gateway status JSON   | 0   |
| `smartfarm/commands`           | Dashboard  | Command JSON          | 1   |
| `smartfarm/alerts`             | Gateway    | Alert JSON            | 1   |

---

## Data Flow

### Scenario 1: Cow Health Monitoring

```
1. Cow Node collects sensor data every second
2. Health monitor analyzes temperature, heart rate, activity
3. Status LED shows health: Green=OK, Yellow=Warning, Red=Critical
4. LoRa transmits data packet every 60 seconds
5. Gateway receives packet, validates data
6. Gateway publishes to MQTT topic: smartfarm/cow/data
7. Cloud dashboard displays real-time health metrics
8. If critical, send SMS/email alert to farmer
```

### Scenario 2: Milk Quality Control

```
1. Cow approaches station, RFID detected
2. Milk station identifies cow, starts session
3. Sensors monitor temperature, pH, conductivity, flow
4. LCD displays real-time quality: "Quality: GOOD"
5. Quality monitor grades milk based on parameters
6. If quality poor, red LED lights, display warns
7. LoRa transmits session data every 10 seconds
8. Gateway receives, publishes to smartfarm/milk/data
9. Cloud logs production data per cow
10. Session ends, total volume calculated and stored
```

### Scenario 3: Emergency Alert

```
1. Cow node detects fever: temp > 40°C
2. Status changes to CRITICAL, red LED lights
3. Emergency LoRa packet sent immediately
4. Gateway receives, identifies as emergency
5. Publishes to both cow/data and alerts topics
6. Cloud triggers notification system
7. Farmer receives SMS: "COW001 - High Fever Alert!"
8. Dashboard highlights cow in red
9. Farmer can send command to check GPS location
```

---

## Data Structures

### Cow Data Packet
```json
{
  "deviceId": "COW001",
  "temperature": 38.5,
  "heartRate": 75,
  "activity": 60,
  "batteryLevel": 85,
  "gpsFixed": true,
  "latitude": -1.9441,
  "longitude": 30.0619,
  "healthStatus": "HEALTHY",
  "timestamp": 1699380000
}
```

### Milk Data Packet
```json
{
  "deviceId": "MILK001",
  "cowId": "COW001",
  "temperature": 4.2,
  "ph": 6.7,
  "conductivity": 5.2,
  "flowRate": 1.5,
  "volume": 12.5,
  "quality": "GOOD",
  "sessionActive": true,
  "sessionStart": 1699380000,
  "timestamp": 1699380120
}
```

### Gateway Status Packet
```json
{
  "wifiConnected": true,
  "mqttConnected": true,
  "loraActive": true,
  "cowNodesActive": 2,
  "milkStationsActive": 1,
  "uptime": 3600000,
  "freeHeap": 245000,
  "rssi": -65,
  "lastDataReceived": 1699380000
}
```

---

## Health Status Decision Logic

### Cow Node Health Indicators

| Parameter    | HEALTHY     | WARNING           | CRITICAL         |
|--------------|-------------|-------------------|------------------|
| Temperature  | 38-39°C     | 37-38°C, 39-40°C  | <37°C, >40°C     |
| Heart Rate   | 60-80 bpm   | 50-60, 80-100 bpm | <50, >100 bpm    |
| Activity     | >40%        | 20-40%            | <20%             |
| Battery      | >20%        | 10-20%            | <10%             |
| GPS Fix      | Yes         | No                | No (missing cow) |

**LED Status**:
- 🟢 GREEN: All parameters healthy
- 🟡 YELLOW: One or more warnings
- 🔴 RED: Any critical parameter

### Milk Quality Indicators

| Parameter      | GOOD        | FAIR           | POOR          |
|----------------|-------------|----------------|---------------|
| Temperature    | 3-5°C       | 5-7°C          | >7°C          |
| pH             | 6.6-6.8     | 6.4-6.6, 6.8-7.0 | <6.4, >7.0  |
| Conductivity   | 4.5-5.5 mS  | 5.5-6.5 mS     | >6.5 mS       |

**Display & LED**:
- 🟢 GREEN LED + "Quality: GOOD" - Accept milk
- 🔴 RED LED + "Quality: POOR" - Reject milk

---

## Power Management

### Cow Node (Battery Powered)
- **Normal**: 150 mA average
- **Deep Sleep**: 10 mA (future enhancement)
- **Battery**: 3.7V 2000mAh LiPo
- **Runtime**: ~13 hours continuous
- **Charging**: Solar panel or dock station

### Milk Station (Mains Powered)
- **Power**: 5V 2A USB adapter
- **Consumption**: 200 mA average
- **Backup**: Optional UPS for data integrity

### Gateway (Mains Powered)
- **Power**: 5V 2A USB adapter
- **Consumption**: 180 mA average
- **Backup**: Recommended UPS for 24/7 operation

---

## Deployment Guide

### Phase 1: Gateway Setup
1. Install gateway in central farm location
2. Connect to WiFi and MQTT broker
3. Verify cloud connectivity
4. Test LoRa reception

### Phase 2: Milk Station Deployment
1. Install at milking parlor
2. Connect power supply
3. Calibrate sensors (pH, conductivity)
4. Test LoRa communication with gateway
5. Train operators on LCD interface

### Phase 3: Cow Node Deployment
1. Charge all cow node batteries
2. Assign cow IDs and configure
3. Attach to cow collars/ear tags
4. Verify GPS fix and LoRa range
5. Monitor first 24h for battery life

### Phase 4: Cloud Dashboard
1. Set up MQTT subscription
2. Create visualization dashboard
3. Configure alert thresholds
4. Set up notification channels (SMS/email)
5. Train farm manager on interface

---

## Maintenance

### Daily
- Check gateway status LED (should be blinking green)
- Verify cloud dashboard receiving data
- Check milk station display for errors

### Weekly
- Charge/replace cow node batteries
- Clean milk station sensors
- Check LoRa signal strength (RSSI)

### Monthly
- Calibrate pH and conductivity sensors
- Update firmware if needed
- Backup historical data
- Review alert logs

---

## Scalability

Current system supports:
- **Cow Nodes**: Up to 50 (LoRa bandwidth limit)
- **Milk Stations**: Up to 10
- **Gateways**: 1 per farm, multiple for large farms

For larger deployments:
- Add more gateways with different LoRa channels
- Implement mesh networking
- Use 4G gateway for remote areas
- Add edge computing for local analytics

---

## Security

### Device Level
- Unique device IDs
- LoRa sync word (private network)
- Encrypted MQTT (TLS) recommended for production

### Cloud Level
- MQTT authentication
- API keys for dashboard access
- Role-based access control
- Data encryption at rest

### Network Level
- WPA2/WPA3 WiFi encryption
- VPN for remote access
- Firewall rules on gateway

---

## Future Enhancements

### Planned Features
- [ ] Machine learning for disease prediction
- [ ] Automatic cow location tracking
- [ ] Feed intake monitoring
- [ ] Automated milking robot integration
- [ ] Blockchain for milk traceability
- [ ] Weather data integration
- [ ] Breeding cycle tracking
- [ ] Voice alerts for farmers

### Hardware Upgrades
- [ ] Solar panels for cow nodes
- [ ] 4G/LTE backup for gateway
- [ ] Camera integration for visual monitoring
- [ ] Automated feeding systems
- [ ] Water quality sensors

---

## Support & Documentation

- **Setup Guides**: See individual node README files
- **API Documentation**: Check header files
- **Troubleshooting**: See QUICK_SETUP.md
- **Cloud Integration**: See gateway README
- **Hardware Schematics**: See diagram.json files

## License
MIT License - Smart Farm System 2025
