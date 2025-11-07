# SMART FARM SYSTEM - TECHNICAL SPECIFICATIONS

## Document Information
**Project:** Smart Farm Cattle Monitoring System  
**Document Type:** Technical Specifications  
**Version:** 1.0  
**Date:** November 7, 2025  
**Prepared by:** ThereTech Team  

---

## 1. SYSTEM OVERVIEW

### 1.1 Architecture Diagram

```
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                    SMART FARM ECOSYSTEM                                +
+                                                                        +
+  +++++++++++++++    LoRa     +++++++++++++++    LoRa     +++++++++++++ +
+  + COW NODE    +*++++++++++* +MILK STATION +*++++++++++* +  GATEWAY  + +
+  +(Collar)     +             +   NODE      +             +   NODE    + +
+  +             +             +             +             +           + +
+  +• GPS        +             +• Flow Meter +             +• LoRa Hub + +
+  +• Temp Sens  +             +• pH Sensor  +             +• TFT LCD  + +
+  +• Accel      +             +• RFID Read  +             +• WiFi     + +
+  +• Solar      +             +• LCD Disp   +             +• Cloud    + +
+  +• Battery    +             +• Valves     +             +• Alerts   + +
+  +++++++++++++++             +++++++++++++++             +++++++++++++ +
+                                                                        +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
```

### 1.2 Communication Matrix

| Source | Destination | Protocol | Frequency | Data Type |
|--------|-------------|----------|-----------|-----------|
| Cow Node | Gateway | LoRa 915MHz | Every 30s | Health data |
| Milk Station | Gateway | LoRa 915MHz | Per session | Production data |
| Gateway | Cloud | WiFi/4G HTTP | Every 5min | Aggregated data |
| Mobile App | Cloud | HTTPS | Real-time | User requests |
| Cloud | Gateway | WebSocket | Real-time | Alerts/Config |

---

## 2. COW NODE SPECIFICATIONS

### 2.1 Hardware Specifications

#### 2.1.1 Main Controller
- **Microcontroller**: ESP32-WROOM-32
- **Architecture**: Dual-core Xtensa LX6
- **Clock Speed**: 240 MHz
- **RAM**: 520 KB SRAM
- **Flash**: 4 MB
- **Wireless**: Wi-Fi 802.11 b/g/n + Bluetooth 4.2

#### 2.1.2 Sensors
| Sensor | Model | Interface | Accuracy | Range | Power |
|--------|-------|-----------|----------|-------|-------|
| **Temperature** | DS18B20 | 1-Wire | ±0.5°C | -55°C to +125°C | 1mA |
| **Accelerometer** | MPU6050 | I2C | ±1% | ±2/4/8/16g | 3.5mA |
| **GPS** | NEO-6M/8M | UART | ±3m CEP | Global | 25mA |
| **Battery Monitor** | INA219 | I2C | ±0.5% | 0-26V | 1mA |

#### 2.1.3 Communication
- **LoRa Module**: SX1276/RFM95W
- **Frequency**: 915 MHz (US), 868 MHz (EU)
- **TX Power**: +20 dBm (100mW) max
- **Sensitivity**: -148 dBm
- **Range**: 10+ km line-of-sight, 2-5 km typical

#### 2.1.4 Power System
- **Primary Battery**: 2x 18650 Li-ion (3000mAh each)
- **Total Capacity**: 6000mAh @ 3.7V = 22.2Wh
- **Solar Panel**: 5V 1W Monocrystalline
- **Charge Controller**: TP4056 with protection
- **Low Voltage Cutoff**: 3.0V per cell

#### 2.1.5 Environmental Protection
- **Enclosure Rating**: IP67 (waterproof/dustproof)
- **Operating Temperature**: -20°C to +60°C
- **Storage Temperature**: -40°C to +85°C
- **Humidity**: 0-95% RH non-condensing
- **Shock/Vibration**: MIL-STD-810G compliant

### 2.2 Software Specifications

#### 2.2.1 Operating System
- **Framework**: Arduino ESP32 Core
- **RTOS**: FreeRTOS v10.x
- **Development**: PlatformIO/Arduino IDE
- **Language**: C++ with Arduino libraries

#### 2.2.2 Data Structures
```cpp
// Cow Health Data Packet
struct CowHealthData {
    char deviceID[16];      // "COW-A1B2C3D4E5F6"
    uint32_t timestamp;     // Unix timestamp
    float temperature;      // Body temperature (°C)
    float latitude;         // GPS coordinates
    float longitude;        // GPS coordinates
    uint8_t gpsQuality;     // GPS fix quality (0-9)
    float activityLevel;    // Activity magnitude
    uint8_t movementState;  // 0=Rest, 1=Walk, 2=Active
    uint8_t ruminationState; // 0=None, 1=Ruminating
    uint8_t batteryLevel;   // Battery percentage (0-100)
    uint8_t alertFlags;     // Bit flags for alerts
    uint16_t checksum;      // Data integrity check
};
```

#### 2.2.3 Power Management
```cpp
// Sleep Schedule
#define ACTIVE_DURATION_MS      5000   // 5 seconds active
#define SLEEP_DURATION_MS       25000  // 25 seconds sleep
#define DEEP_SLEEP_THRESHOLD    3.3    // Voltage for deep sleep
#define CRITICAL_BATTERY        10     // Critical battery %

// Wake-up Sources
- Timer wake (every 30 seconds)
- Temperature threshold exceeded
- High activity detection
- Low battery alert
```

### 2.3 Performance Specifications

#### 2.3.1 Power Consumption
| State | Current (mA) | Duration | Daily Usage |
|-------|--------------|----------|-------------|
| **Active** | 120 | 5s/30s cycle | 4.8 Ah |
| **Deep Sleep** | 0.01 | 25s/30s cycle | 0.02 Ah |
| **GPS Acquisition** | 150 | 30s (cold start) | 0.3 Ah |
| **LoRa Transmission** | 140 | 0.5s per packet | 0.1 Ah |
| **Total Daily** | | | **5.22 Ah** |

#### 2.3.2 Battery Life Calculation
```
Solar Generation (6h effective sun):
1W ÷ 3.7V = 270mA × 6h = 1.62 Ah/day

Net Consumption: 5.22 - 1.62 = 3.6 Ah/day
Battery Life: 6000mAh ÷ 3600mAh = 1.67 days minimum
With 50% solar efficiency: 4-7 days typical
```

#### 2.3.3 Accuracy Specifications
- **Temperature Accuracy**: ±0.5°C (calibrated)
- **GPS Accuracy**: ±3 meters CEP (50% confidence)
- **Activity Detection**: ±5% error rate
- **Battery Monitoring**: ±2% accuracy
- **Timestamp Accuracy**: ±1 second (NTP synced)

---

## 3. MILK STATION SPECIFICATIONS

### 3.1 Hardware Specifications

#### 3.1.1 Main Controller
- **Microcontroller**: ESP32-WROOM-32
- **Processing Power**: Same as Cow Node
- **Memory**: 4MB Flash, 520KB RAM
- **Interfaces**: 2×UART, 2×I2C, 4×SPI, 18×GPIO

#### 3.1.2 Milk Monitoring Sensors
| Sensor | Model | Measurement | Range | Accuracy | Interface |
|--------|-------|-------------|-------|----------|-----------|
| **Flow Meter** | YF-S201 | Volume flow | 1-30 L/min | ±2% | Pulse counting |
| **Load Cell** | 5kg Capacity | Weight | 0-5 kg | ±0.1% | HX711 ADC |
| **pH Sensor** | SEN0161 | Acidity | 0-14 pH | ±0.1 pH | Analog |
| **Conductivity** | DFR0300 | EC/TDS | 0-20 mS/cm | ±2% | Analog |
| **Temperature** | DS18B20 | Milk temp | 0-50°C | ±0.5°C | 1-Wire |

#### 3.1.3 Identification System
- **RFID Reader**: RC522 (13.56 MHz)
- **Read Range**: 0-60mm
- **Tag Type**: ISO14443A/MIFARE
- **Data Storage**: 1KB per tag
- **Read Time**: <100ms

#### 3.1.4 User Interface
- **Display**: 20×4 Character LCD with I2C
- **Backlight**: Blue LED with dimming
- **Input**: 4×4 Matrix keypad
- **Status LEDs**: Red/Yellow/Green indicators
- **Audio**: Active buzzer for alerts

#### 3.1.5 Control Systems
- **Solenoid Valves**: 2× 12V normally closed
- **Flow Control**: Automated start/stop
- **Sample Valve**: Quality testing port
- **Emergency Stop**: Manual override button

### 3.2 Milk Quality Standards

#### 3.2.1 Quality Parameters
| Parameter | Acceptable Range | Warning Level | Reject Level |
|-----------|-----------------|---------------|--------------|
| **pH** | 6.6 - 6.8 | 6.4-6.6, 6.8-7.0 | <6.4, >7.0 |
| **Temperature** | 35-39°C | 32-35°C, 39-42°C | <32°C, >42°C |
| **Conductivity** | 4.0-5.5 mS/cm | 5.5-6.5 mS/cm | >6.5 mS/cm |
| **Flow Rate** | 2-8 L/min | 1-2, 8-10 L/min | <1, >10 L/min |

#### 3.2.2 Mastitis Detection Algorithm
```cpp
bool detectMastitis(float conductivity, float temperature) {
    // High conductivity indicates somatic cell count
    if (conductivity > 6.5) return true;
    
    // Combined indicators
    if (conductivity > 5.8 && temperature > 40.0) return true;
    
    // Trend analysis (3-day average)
    if (conductivityTrend > 1.5) return true;
    
    return false;
}
```

### 3.3 Data Management

#### 3.3.1 Session Data Structure
```cpp
struct MilkSession {
    char stationID[16];     // "MILK-A1B2C3D4E5F6"
    char cowID[8];          // "COW001"
    uint32_t startTime;     // Session start timestamp
    uint32_t endTime;       // Session end timestamp
    float totalVolume;      // Total milk volume (L)
    float totalWeight;      // Total milk weight (kg)
    float avgpH;            // Average pH
    float avgTemp;          // Average temperature (°C)
    float avgConductivity;  // Average conductivity (mS/cm)
    uint8_t qualityGrade;   // A=1, B=2, C=3, Reject=4
    bool mastitisFlag;      // Mastitis detected
    uint16_t dataPoints;    // Number of sensor readings
};
```

#### 3.3.2 Real-time Monitoring
- **Sampling Rate**: 1 Hz (1 sample/second)
- **Data Buffer**: 300 samples (5 minutes)
- **Transmission**: Every 30 seconds to Gateway
- **Local Storage**: 1000 sessions on SD card

---

## 4. GATEWAY SPECIFICATIONS

### 4.1 Hardware Specifications

#### 4.1.1 Main Controller
- **Microcontroller**: ESP32-WROOM-32
- **Enhanced RAM**: 8MB PSRAM (optional)
- **Storage**: 16MB Flash + SD card slot
- **Real-time Clock**: DS3231 with battery backup

#### 4.1.2 User Interface
- **Display**: 3.5" TFT LCD (480×320 pixels)
- **Touch Screen**: Resistive touch interface
- **Colors**: 65K colors (16-bit)
- **Brightness**: Auto-adjusting with ambient sensor
- **Screen Saver**: 5-minute timeout

#### 4.1.3 Communication Interfaces
| Interface | Specification | Purpose |
|-----------|---------------|---------|
| **LoRa** | SX1276 915MHz | Receive from nodes |
| **WiFi** | 802.11 b/g/n | Internet connectivity |
| **Ethernet** | W5500 module | Wired network backup |
| **4G/LTE** | SIM800L module | Cellular backup |
| **USB** | Type-C connector | Programming/debug |

#### 4.1.4 Storage and Backup
- **Local Database**: SQLite on SD card
- **Capacity**: 32GB SD card (2+ years data)
- **Backup Power**: 12V 7Ah SLA battery
- **Runtime**: 8+ hours on battery
- **Auto-shutdown**: At 20% battery remaining

### 4.2 Software Architecture

#### 4.2.1 User Interface Framework
```cpp
// Screen Management
enum ScreenType {
    SCREEN_DASHBOARD = 0,
    SCREEN_COW_LIST,
    SCREEN_COW_DETAIL,
    SCREEN_MILK_PRODUCTION,
    SCREEN_ALERTS,
    SCREEN_SETTINGS,
    SCREEN_REPORTS
};

// Touch Event Handling
struct TouchEvent {
    uint16_t x, y;
    bool pressed;
    uint32_t timestamp;
};
```

#### 4.2.2 Data Aggregation
- **Collection Rate**: Real-time from LoRa
- **Processing**: Moving averages, trend analysis
- **Storage**: Time-series database structure
- **Compression**: Delta compression for historical data
- **Sync Interval**: Every 5 minutes to cloud

#### 4.2.3 Alert System
```cpp
enum AlertLevel {
    ALERT_INFO = 0,      // Informational
    ALERT_WARNING = 1,   // Requires attention
    ALERT_CRITICAL = 2   // Immediate action needed
};

struct Alert {
    uint8_t level;
    char deviceID[16];
    char message[128];
    uint32_t timestamp;
    bool acknowledged;
};
```

### 4.3 Performance Specifications

#### 4.3.1 Processing Capacity
- **Concurrent Nodes**: 50+ cow nodes, 5+ milk stations
- **Data Rate**: 1000+ packets/hour
- **Response Time**: <100ms for touch events
- **Screen Refresh**: 30 FPS for animations
- **Memory Usage**: <70% RAM utilization

#### 4.3.2 Network Performance
| Protocol | Throughput | Latency | Reliability |
|----------|------------|---------|-------------|
| **LoRa RX** | 1.2 kbps | 100-500ms | >99% |
| **WiFi** | 10+ Mbps | <50ms | >95% |
| **Ethernet** | 10 Mbps | <10ms | >99% |
| **4G/LTE** | 1+ Mbps | 100-300ms | >90% |

---

## 5. COMMUNICATION PROTOCOL

### 5.1 LoRa Physical Layer

#### 5.1.1 RF Parameters
- **Frequency**: 915 MHz ±0.5 MHz
- **Bandwidth**: 125 kHz
- **Spreading Factor**: 7-12 (adaptive)
- **Coding Rate**: 4/5, 4/6, 4/7, 4/8
- **TX Power**: 2-20 dBm (1.6-100mW)
- **Receiver Sensitivity**: -148 dBm

#### 5.1.2 Data Rate vs Range
| Spreading Factor | Data Rate | Range (km) | Airtime (50 bytes) |
|------------------|-----------|------------|-------------------|
| **SF7** | 5.47 kbps | 2-3 km | 56.6 ms |
| **SF8** | 3.13 kbps | 3-5 km | 103.2 ms |
| **SF9** | 1.76 kbps | 4-6 km | 185.3 ms |
| **SF10** | 0.98 kbps | 6-8 km | 371.2 ms |
| **SF11** | 0.54 kbps | 8-10 km | 741.4 ms |
| **SF12** | 0.29 kbps | 10+ km | 1483.8 ms |

### 5.2 Protocol Stack

#### 5.2.1 Packet Structure
```
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Preamble + Header + Device ID + Payload + CRC + End +
+ (8 bytes)+(4 bytes)+ (8 bytes) +(1-255 B)+(2 B)+(1 B)+
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Header Format:
- Protocol Version (4 bits)
- Message Type (4 bits)
- Payload Length (8 bits)
- Sequence Number (8 bits)
- Timestamp (8 bits - seconds since hour)
```

#### 5.2.2 Message Types
| Type | ID | Description | Max Size |
|------|----|-----------|----|
| **COW_DATA** | 0x01 | Health monitoring data | 64 bytes |
| **MILK_DATA** | 0x02 | Milk production data | 128 bytes |
| **ALERT** | 0x03 | Emergency alerts | 32 bytes |
| **CONFIG** | 0x04 | Configuration commands | 64 bytes |
| **ACK** | 0x05 | Acknowledgment | 8 bytes |
| **HEARTBEAT** | 0x06 | Keep-alive signal | 16 bytes |

### 5.3 Error Handling

#### 5.3.1 Automatic Repeat Request (ARQ)
- **Retry Attempts**: 3 maximum
- **Timeout**: 2× expected airtime
- **Backoff**: Exponential (1s, 2s, 4s)
- **Duplicate Detection**: Sequence number tracking

#### 5.3.2 Forward Error Correction
- **Reed-Solomon**: RS(255,223) encoding
- **Interleaving**: Byte-level interleaving
- **Error Recovery**: Up to 16 byte errors

---

## 6. POWER ANALYSIS

### 6.1 System Power Budget

#### 6.1.1 Cow Node Power Profile
| Component | Active (mA) | Sleep (mA) | Duty Cycle | Avg (mA) |
|-----------|-------------|------------|------------|----------|
| **ESP32** | 80 | 0.01 | 16.7% | 13.3 |
| **GPS** | 25 | 0 | 8.3% | 2.1 |
| **Sensors** | 10 | 0 | 16.7% | 1.7 |
| **LoRa TX** | 120 | 0 | 0.5% | 0.6 |
| **LoRa RX** | 12 | 0 | 1.0% | 0.1 |
| **Total Average** | | | | **17.8 mA** |

#### 6.1.2 Solar Charging Analysis
```
Daily Energy Consumption:
17.8mA × 24h = 427.2 mAh/day = 1.58 Wh/day

Solar Panel Output (5V 1W, 6h effective):
1W × 6h = 6 Wh/day theoretical
6 Wh × 0.8 efficiency × 0.7 weather = 3.36 Wh/day practical

Net Energy: 3.36 - 1.58 = 1.78 Wh/day surplus
Battery Life: Indefinite with adequate sun exposure
```

### 6.2 Thermal Management

#### 6.2.1 Operating Temperature Ranges
| Component | Min (°C) | Max (°C) | Typical (°C) |
|-----------|----------|----------|--------------|
| **ESP32** | -40 | +85 | +25 to +45 |
| **Battery** | -20 | +60 | +10 to +40 |
| **LCD** | -20 | +70 | +5 to +50 |
| **Sensors** | -40 | +85 | Variable |

#### 6.2.2 Thermal Protection
- **Over-temperature Shutdown**: >65°C
- **Under-temperature Protection**: <-15°C
- **Battery Heater**: Activated below 0°C
- **Ventilation**: Passive cooling vents

---

## 7. SAFETY AND COMPLIANCE

### 7.1 Electrical Safety

#### 7.1.1 Safety Standards Compliance
- **IEC 60950-1**: Information Technology Equipment Safety
- **FCC Part 15**: Radio Frequency Devices
- **CE Marking**: European Conformity
- **RoHS**: Restriction of Hazardous Substances

#### 7.1.2 Protection Features
- **Overvoltage Protection**: MOVs and TVS diodes
- **Overcurrent Protection**: Resettable fuses
- **ESD Protection**: ESD suppressors on all I/O
- **Isolation**: Optoisolated sensitive circuits

### 7.2 Food Safety Compliance

#### 7.2.1 Materials
- **Food Contact Surfaces**: FDA-approved stainless steel 316L
- **Gaskets and Seals**: FDA-approved silicone
- **Cleaning**: CIP (Clean-in-Place) compatible
- **Sanitization**: Compatible with standard dairy sanitizers

#### 7.2.2 Cleaning Procedures
```
Daily Cleaning:
1. Disconnect sensors from milk line
2. Flush with warm water (40-45°C)
3. Circulate alkaline cleaner (2% solution)
4. Rinse with potable water
5. Sanitize with iodophor or chlorine solution
6. Final rinse and air dry
```

### 7.3 Data Security

#### 7.3.1 Encryption
- **Data at Rest**: AES-256 encryption
- **Data in Transit**: TLS 1.3 for cloud communication
- **LoRa Security**: AES-128 with rolling keys
- **Key Management**: Hardware security module

#### 7.3.2 Privacy Protection
- **Data Anonymization**: Optional cow ID hashing
- **Local Storage**: No cloud dependency required
- **User Consent**: Configurable data sharing
- **GDPR Compliance**: Right to erasure implemented

---

## 8. TESTING AND VALIDATION

### 8.1 Environmental Testing

#### 8.1.1 Test Standards
- **IP67 Waterproof**: IEC 60529 standard
- **Vibration**: ASTM D4169 (livestock movement)
- **Temperature Cycling**: -20°C to +60°C
- **Humidity**: 95% RH non-condensing
- **UV Exposure**: ASTM G154 (solar radiation)

#### 8.1.2 Durability Testing
| Test | Duration | Criteria |
|------|----------|----------|
| **Drop Test** | 50 drops from 1.5m | No functional failure |
| **Submersion** | 30 min at 1m depth | No water ingress |
| **Vibration** | 8 hours at 5G | All functions operational |
| **Temperature** | 1000 cycles | <1% drift in accuracy |

### 8.2 Performance Validation

#### 8.2.1 Field Testing Protocol
```
Phase 1: Laboratory Testing (2 weeks)
- Sensor accuracy validation
- Communication range testing
- Battery life verification
- Software functionality

Phase 2: Controlled Farm Testing (4 weeks)
- 5 cows with reference monitoring
- Milk quality comparison with lab results
- System reliability assessment
- User interface evaluation

Phase 3: Production Testing (8 weeks)
- 25 cows full deployment
- Veterinarian validation of health alerts
- Milk production correlation analysis
- Long-term reliability data
```

#### 8.2.2 Acceptance Criteria
- **Uptime**: >99% system availability
- **Accuracy**: <5% error vs reference methods
- **Range**: >2km communication in typical farm
- **Battery**: >7 days without solar charging
- **User Satisfaction**: >4.5/5 rating

---

## 9. MANUFACTURING SPECIFICATIONS

### 9.1 Production Requirements

#### 9.1.1 Assembly Process
```
PCB Assembly:
1. SMD component placement (pick-and-place)
2. Reflow soldering (lead-free SAC305)
3. Through-hole component insertion
4. Wave soldering for THT components
5. Cleaning (no-clean flux residue)
6. Automated optical inspection (AOI)
7. In-circuit testing (ICT)
8. Functional testing

Final Assembly:
1. PCB installation in enclosure
2. Antenna attachment and testing
3. Battery pack assembly
4. Solar panel integration
5. Waterproof seal application
6. Final quality inspection
7. Programming and calibration
8. Packaging and documentation
```

#### 9.1.2 Quality Control
- **Incoming Inspection**: 100% critical components
- **Process Control**: Statistical process control
- **Final Testing**: 100% functional test
- **Traceability**: Full component genealogy
- **Yield Target**: >98% first-pass yield

#### 9.2.2 Inventory Management
- **Safety Stock**: 4 weeks production
- **Reorder Points**: Based on lead times
- **Just-in-Time**: For high-volume components
- **Consignment**: For expensive ICs

---

## 10. MAINTENANCE AND SUPPORT

### 10.1 Preventive Maintenance

#### 10.1.1 Maintenance Schedule
| Interval | Task | Time Required |
|----------|------|---------------|
| **Daily** | Visual inspection, alert review | 10 minutes |
| **Weekly** | Sensor cleaning, battery check | 30 minutes |
| **Monthly** | Calibration verification | 1 hour |
| **Quarterly** | Firmware updates, full test | 2 hours |
| **Annually** | Battery replacement, deep clean | 4 hours |

#### 10.1.2 Maintenance Tools Required
- **Basic Tools**: Screwdriver set, multimeter
- **Calibration**: pH buffers, conductivity standards
- **Cleaning**: Appropriate dairy sanitizers
- **Software**: Maintenance app on tablet/phone

### 10.2 Technical Support

#### 10.2.1 Support Tiers
```
Tier 1: Basic Support (Local technician)
- Installation assistance
- Basic troubleshooting
- Routine maintenance
- User training

Tier 2: Advanced Support (Regional expert)
- Complex troubleshooting
- Firmware updates
- Sensor calibration
- Performance optimization

Tier 3: Engineering Support (Development team)
- Hardware/software issues
- Custom modifications
- Advanced analytics
- Product improvements
```

#### 10.2.2 Remote Diagnostics
- **Cloud Monitoring**: 24/7 system health monitoring
- **Predictive Maintenance**: ML-based failure prediction
- **Remote Updates**: Over-the-air firmware updates
- **Performance Analytics**: Trend analysis and optimization

## Contact Information

**Direct Contact**: 0781008164

---

**Powered by ThereTech.rw**  
© 2025 ThereTech.rw - All rights reserved  
