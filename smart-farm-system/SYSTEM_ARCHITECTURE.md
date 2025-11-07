# Smart Farm Monitoring System - Complete Architecture

## System Overview

This is a comprehensive IoT-based cattle monitoring system consisting of **3 separate firmware projects** working together to provide complete farm management.

```
┌─────────────────────────────────────────────────────────────────┐
│                     SMART FARM SYSTEM                           │
│                                                                 │
│  ┌──────────────┐      ┌──────────────┐      ┌──────────────┐   │
│  │   COW NODE   │      │ MILK STATION │      │   GATEWAY    │   │
│  │   (Mobile)   │─LoRa─│    (Fixed)   │─LoRa─│  (Central)   │   │
│  │              │      │              │      │              │   │
│  └──────────────┘      └──────────────┘      └──────────────┘   │
│        │                     │                      │           │
│     GPS/Temp            Milk Sensors           WiFi/Cloud       │
│   Accelerometer          Flow/pH/Cond          Dashboard        │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## 1. COW NODE FIRMWARE (Wearable Collar)

### Hardware Components
| Component | Purpose | Power Usage |
|-----------|---------|-------------|
| ESP32 | Main controller | ~80mA active, ~10µA deep sleep |
| DS18B20 | Body temperature | ~1mA |
| MPU6050 | Activity/movement | ~3.5mA |
| GPS Module | Location tracking | ~25mA |
| LoRa (SX1276) | Communication | ~120mA TX, ~10mA RX |
| Microphone | Digestion sounds | ~0.5mA |
| Battery Monitor | Power status | Minimal |
| 3.7V Li-ion Battery | Power source | 2000-5000mAh |
| Solar Panel | Charging | 5V 500mA |

### Key Features
- ✅ **Low Power Design**: Deep sleep between readings
- ✅ **Unique Cow ID**: Each device has unique identifier
- ✅ **Health Monitoring**: Temperature, activity, location
- ✅ **Alert System**: Fever detection, inactivity alerts
- ✅ **LoRa Communication**: Long-range wireless (up to 10km)
- ✅ **Solar Charging**: Self-sustaining power
- ✅ **Weatherproof**: IP67 rated enclosure

### Data Transmission Format
```
COW_ID:001|TEMP:38.2|LAT:40.7128|LON:-74.0060|ACTIVITY:12.5|MOVING:1|DIGEST:1|BATT:85|ALERT:0
```

### Power Management
- **Active Mode** (5 sec every 30 sec): Read sensors, transmit
- **Deep Sleep** (25 sec): Ultra-low power consumption
- **Expected Battery Life**: 7-14 days on single charge with solar assist

### Firmware Location
```
/home/admino/smart-farm-system/cow-node/
```

---

## 2. MILK STATION NODE FIRMWARE (Milking Equipment)

### Hardware Components
| Component | Purpose | Cost (RWF) |
|-----------|---------|------------|
| ESP32 | Main controller | 8,700 |
| YF-S201 Flow Sensor | Milk volume | 21,750 |
| HX711 + Load Cell | Milk weight | 14,500 |
| DS18B20 | Milk temperature | 7,250 |
| pH Sensor | Milk acidity | 36,250 |
| Conductivity Sensor | Mastitis detection | 43,500 |
| Solenoid Valve | Flow control | 29,000 |
| LCD 20x4 Display | Local display | 7,250 |
| LoRa Module | Communication | 14,500 |
| RFID Reader | Cow identification | 21,750 |
| 12V Power Supply | Main power | 14,500 |

**Total Hardware Cost**: ~219,000 RWF (~$220 USD)

### Key Features
- ✅ **Cow Identification**: RFID tag or manual ID entry
- ✅ **Gender Validation**: Only accept female cows
- ✅ **Milk Volume Tracking**: Precise flow measurement
- ✅ **Quality Monitoring**: pH, conductivity, temperature
- ✅ **Mastitis Detection**: High conductivity alerts
- ✅ **Per-Cow Logging**: Individual production records
- ✅ **Session Management**: Start/stop milking sessions
- ✅ **Real-time Display**: LCD shows current status

### Data Transmission Format
```
MILK_SESSION|COW_ID:001|VOLUME:12.5L|WEIGHT:12.8kg|PH:6.6|TEMP:37.5|COND:4.5|QUALITY:GOOD|TIME:1234567890
```

### Operation Flow
```
1. Cow enters milking station
2. RFID reads Cow ID (or manual entry)
3. System checks: Is cow female? Is milk enabled?
4. If YES: Start milking session
5. Monitor: Flow, weight, pH, conductivity, temperature
6. Detect quality issues (mastitis, spoilage)
7. End session: Log total volume, quality status
8. Transmit data to Gateway via LoRa
9. Display summary on LCD
```

### Firmware Location
```
/home/admino/smart-farm-system/milk-station-node/
```

---

## 3. GATEWAY NODE FIRMWARE (Central Management)

### Hardware Components
| Component | Purpose |
|-----------|---------|
| ESP32 | Main controller |
| LoRa Module | Receive from all nodes |
| 3.5" TFT LCD Touch | Display & control |
| WiFi/Ethernet | Cloud connectivity |
| SD Card | Local data logging |
| RTC Module | Accurate timestamps |
| Buzzer | Alert notifications |
| 12V Power Supply | Mains powered |

### Key Features
- ✅ **Multi-Device Dashboard**: View all cows and milk stations
- ✅ **Real-time Monitoring**: Live data from all nodes
- ✅ **Alert Management**: Visual + audio alerts
- ✅ **Device Configuration**: Remote setup of cow/milk nodes
- ✅ **Data Logging**: Store to SD card + cloud
- ✅ **Human-Readable Display**: Farmer-friendly interface
- ✅ **Historical Reports**: Daily/weekly production summaries
- ✅ **Cloud Sync**: Upload to web dashboard

### Display Screens

#### Screen 1: Dashboard Overview
```
┌─────────────────────────────────────┐
│  SMART FARM DASHBOARD     12:34 PM  │
├─────────────────────────────────────┤
│ Total Cows: 25    Active: 23        │
│ Alerts: 2         Milking: 3        │
├─────────────────────────────────────┤
│ COW #001  Temp:38.2°C  [ALERT!]     │
│ COW #003  Location: Field A         │
│ COW #007  Activity: High            │
├─────────────────────────────────────┤
│ MILK STATION: Cow #012 Milking...   │
│ Volume: 8.5L  Quality: Good         │
├─────────────────────────────────────┤
│ [Cows] [Milk] [Alerts] [Settings]   │
└─────────────────────────────────────┘
```

#### Screen 2: Individual Cow View
```
┌─────────────────────────────────────┐
│  COW #001 - Daisy (Female)          │
├─────────────────────────────────────┤
│ Temperature:  38.5°C  [FEVER!]      │
│ Activity:     Resting               │
│ Location:     Field B (GPS OK)      │
│ Digestion:    Normal                │
│ Battery:      75% (Solar ON)        │
│ Last Update:  2 min ago             │
├─────────────────────────────────────┤
│ Today's Milk: 18.5 liters           │
│ Quality:      Excellent             │
│ Last Milking: 6:30 AM               │
├─────────────────────────────────────┤
│ [View Map] [History] [Configure]    │
└─────────────────────────────────────┘
```

#### Screen 3: Milk Production View
```
┌─────────────────────────────────────┐
│  MILK PRODUCTION - Today            │
├─────────────────────────────────────┤
│ Total: 285 liters (18 cows)         │
│ Average: 15.8 L/cow                 │
│ Quality Issues: 1 (Cow #015)        │
├─────────────────────────────────────┤
│ Top Producers:                       │
│ #023: 22.5L  #007: 21.3L            │
│ #012: 19.8L  #001: 18.5L            │
├─────────────────────────────────────┤
│ Low Producers (Check Health):        │
│ #015: 8.2L [Mastitis Alert]         │
│ #019: 10.5L                          │
├─────────────────────────────────────┤
│ [Daily] [Weekly] [Export]            │
└─────────────────────────────────────┘
```

#### Screen 4: Alerts & Notifications
```
┌─────────────────────────────────────┐
│  ACTIVE ALERTS (2)           [MUTE] │
├─────────────────────────────────────┤
│ 🔴 CRITICAL                          │
│ Cow #001: High Temperature (38.5°C) │
│ Location: Field B - 2 min ago       │
│ Action: Call veterinarian           │
├─────────────────────────────────────┤
│ ⚠️  WARNING                          │
│ Cow #015: Mastitis detected         │
│ Milk conductivity: 8.2 mS/cm        │
│ Action: Separate milk, check udder  │
├─────────────────────────────────────┤
│ [Acknowledge] [View Cow] [Dismiss]   │
└─────────────────────────────────────┘
```

### Firmware Location
```
/home/admino/smart-farm-system/gateway-node/
```

---

## Communication Protocol

### LoRa Network Configuration
- **Frequency**: 915 MHz (or 868 MHz for Europe)
- **Bandwidth**: 125 kHz
- **Spreading Factor**: 7-12 (adaptive)
- **Coding Rate**: 4/5
- **TX Power**: 20 dBm (100mW)
- **Range**: Up to 10 km line-of-sight

### Message Types

#### 1. Cow Node → Gateway
```
Type: COW_DATA
Format: COW|<ID>|<TEMP>|<LAT>|<LON>|<ACTIVITY>|<MOVING>|<DIGEST>|<BATTERY>|<ALERT>
Example: COW|001|38.2|40.7128|-74.0060|12.5|1|1|85|0
Frequency: Every 30 seconds (or on alert)
```

#### 2. Milk Station → Gateway
```
Type: MILK_DATA
Format: MILK|<COW_ID>|<VOLUME>|<WEIGHT>|<PH>|<TEMP>|<COND>|<QUALITY>|<TIMESTAMP>
Example: MILK|001|12.5|12.8|6.6|37.5|4.5|GOOD|1699364123
Frequency: At end of milking session
```

#### 3. Gateway → Cow Node (Configuration)
```
Type: CONFIG
Format: CFG|<COW_ID>|<PARAM>|<VALUE>
Example: CFG|001|SLEEP_TIME|30
Frequency: On-demand
```

#### 4. Gateway → Milk Station (Configuration)
```
Type: CONFIG
Format: CFG|MILK|<PARAM>|<VALUE>
Example: CFG|MILK|PH_MIN|6.5
Frequency: On-demand
```

---

## Data Flow Diagram

```
┌──────────────┐
│   COW #001   │  Temperature: 38.2°C, Location: Field A
│  (Collar)    │  Activity: Walking, Battery: 85%
└──────┬───────┘
       │ LoRa (Every 30s)
       ▼
┌──────────────┐
│   GATEWAY    │  ◄─── Receives data from all cows
│ (Base Stn)   │  ◄─── Receives milk data from station
└──────┬───────┘  ─── Displays on LCD dashboard
       │             ─── Sends to cloud/database
       │ WiFi         ─── Manages alerts
       ▼
┌──────────────┐
│  CLOUD/WEB   │  Historical data, reports, analytics
│  DASHBOARD   │  Accessible via phone/computer
└──────────────┘

       ▲
       │ LoRa (End of session)
       │
┌──────────────┐
│ MILK STATION │  Cow #001 milked: 12.5L, Quality: Good
│   (Fixed)    │  pH: 6.6, Temp: 37.5°C, No mastitis
└──────────────┘
```

---

## Project Structure

```
smart-farm-system/
│
├── cow-node/                      # Firmware 1: Wearable collar
│   ├── platformio.ini
│   ├── src/
│   │   ├── main.cpp              # Main loop
│   │   ├── sensors.cpp           # Health sensors
│   │   ├── lora_comm.cpp         # LoRa communication
│   │   └── power_mgmt.cpp        # Sleep/battery management
│   └── include/
│       ├── config.h              # Cow node configuration
│       └── *.h                   # Header files
│
├── milk-station-node/             # Firmware 2: Milking equipment
│   ├── platformio.ini
│   ├── src/
│   │   ├── main.cpp              # Main loop
│   │   ├── milk_sensors.cpp      # Flow, pH, conductivity
│   │   ├── rfid_reader.cpp       # Cow identification
│   │   ├── lcd_display.cpp       # Local display
│   │   └── lora_comm.cpp         # LoRa communication
│   └── include/
│       ├── config.h              # Station configuration
│       └── *.h                   # Header files
│
├── gateway-node/                  # Firmware 3: Central hub
│   ├── platformio.ini
│   ├── src/
│   │   ├── main.cpp              # Main loop
│   │   ├── lora_receiver.cpp     # Receive from all nodes
│   │   ├── tft_display.cpp       # Touch screen interface
│   │   ├── data_logger.cpp       # SD card logging
│   │   ├── cloud_sync.cpp        # WiFi/cloud upload
│   │   └── device_manager.cpp    # Configure nodes
│   └── include/
│       ├── config.h              # Gateway configuration
│       └── *.h                   # Header files
│
└── SYSTEM_ARCHITECTURE.md         # This file

```

---

## Development Plan

### Phase 1: Cow Node (Week 1-2)
- [x] Basic health monitoring
- [ ] LoRa transmission
- [ ] Power management
- [ ] Testing with real hardware

### Phase 2: Milk Station (Week 3-4)
- [ ] Sensor integration
- [ ] RFID reader
- [ ] Quality monitoring
- [ ] LCD interface

### Phase 3: Gateway (Week 5-6)
- [ ] LoRa receiver
- [ ] TFT display interface
- [ ] Multi-device management
- [ ] Alert system

### Phase 4: Integration (Week 7-8)
- [ ] End-to-end testing
- [ ] Cloud connectivity
- [ ] Performance optimization
- [ ] Documentation

---

## Next Steps

Would you like me to:

1. **Create Cow Node firmware** - Based on current smart-collar-platformio with LoRa focus
2. **Create Milk Station firmware** - New project with milk sensors + RFID
3. **Create Gateway firmware** - Dashboard with TFT display + multi-device management

Which firmware would you like to develop first?
