# Device Configuration System

## Overview
All three node types (Cow Node, Milk Station, Gateway) use a unified device configuration system that supports:
- **WiFi connectivity** for device manager communication
- **LoRa wireless** for inter-device communication  
- **Persistent storage** using ESP32 NVS (Non-Volatile Storage)
- **Serial configuration** interface for setup

## Configuration Structure

### Common Settings (All Devices)
- `deviceID` - Unique identifier (auto-generated from MAC)
- `deviceType` - COW_NODE (1), MILK_STATION (2), or GATEWAY (3)
- `deviceName` - Human-readable name
- `farmID` - Farm identification code

### WiFi Configuration
- `wifiEnabled` - Enable/disable WiFi (boolean)
- `wifiSSID` - WiFi network name
- `wifiPassword` - WiFi password
- `deviceManagerIP` - IP address of central device manager
- `deviceManagerPort` - Port for device manager API (default: 8080)

### LoRa Configuration
- `loraEnabled` - Enable/disable LoRa (boolean)
- `loraFrequency` - Operating frequency (default: 915 MHz)
- `loraSyncWord` - Network sync word (default: 0x12)
- `loraSpreadFactor` - Spreading factor 7-12 (default: 7)

### Cow Node Specific
- `cowGender` - 0 = Male, 1 = Female
- `milkMonitoring` - Enable milk production monitoring (boolean)

## Serial Configuration Commands

Connect via serial monitor at 115200 baud and use these commands:

### General Commands
```
HELP                    - Show all available commands
SHOW                    - Display current configuration
SAVE                    - Save configuration to NVS
RESET                   - Reset to factory defaults
```

### Device Identity
```
SET_NAME <name>         - Set device name
                          Example: SET_NAME Milk Station 01

SET_FARM <id>           - Set farm ID
                          Example: SET_FARM FARM001
```

### WiFi Configuration
```
SET_WIFI <ssid> <pass>  - Configure WiFi credentials
                          Example: SET_WIFI MyFarm password123

SET_MANAGER <ip> <port> - Set device manager address
                          Example: SET_MANAGER 192.168.1.100 8080

WIFI_ON                 - Enable WiFi
WIFI_OFF                - Disable WiFi
```

### LoRa Configuration  
```
SET_LORA <freq> <sync> <sf>  - Configure LoRa parameters
                               Example: SET_LORA 915000000 18 7
                               (freq in Hz, sync 0-255, SF 7-12)

LORA_ON                 - Enable LoRa
LORA_OFF                - Disable LoRa
```

### Cow Node Only
```
SET_GENDER MALE         - Configure for male cow (no milk monitoring)
SET_GENDER FEMALE       - Configure for female cow
MILK_ON                 - Enable milk monitoring
MILK_OFF                - Disable milk monitoring
```

## Device Manager Communication

### WiFi HTTP API
When WiFi is enabled and connected, devices send data to:
```
POST http://<deviceManagerIP>:<port>/api/<endpoint>
Content-Type: application/json

{
  "deviceID": "COW-A1B2C3D4",
  "farmID": "FARM001",
  "data": "<sensor data string>"
}
```

### Endpoints
- `/api/cow-data` - Cow health monitoring data
- `/api/milk-data` - Milk production data  
- `/api/gateway-status` - Gateway status updates

### Data Formats

#### Cow Health Data
```
COW|ID|TEMP|LAT|LON|ACTIVITY|MOVING|DIGEST|BATT|ALERT
```

#### Milk Production Data
```
MILK|COW_ID|VOLUME|WEIGHT|PH|TEMP|COND|QUALITY|TIMESTAMP
```

## LoRa Communication

All devices use the same LoRa configuration for inter-device communication:
- Devices broadcast data packets periodically
- Gateway receives all packets and aggregates
- Sync word ensures network isolation
- CRC enabled for data integrity

## Setup Examples

### Cow Node Setup
```
SHOW                                    # View current config
SET_NAME Cow Collar 01                  # Set device name
SET_FARM DAIRY_FARM_001                 # Set farm ID
SET_WIFI FarmNetwork password123        # Configure WiFi
SET_MANAGER 192.168.1.100 8080          # Set manager IP
SET_GENDER FEMALE                       # Female cow
MILK_ON                                 # Enable milk monitoring
SAVE                                    # Save configuration
```

### Milk Station Setup
```
SHOW                                    # View current config
SET_NAME Milk Station A                 # Set device name
SET_FARM DAIRY_FARM_001                 # Set farm ID
SET_WIFI FarmNetwork password123        # Configure WiFi
SET_MANAGER 192.168.1.100 8080          # Set manager IP
SAVE                                    # Save configuration
```

### Gateway Setup
```
SHOW                                    # View current config
SET_NAME Main Gateway                   # Set device name
SET_FARM DAIRY_FARM_001                 # Set farm ID
SET_WIFI FarmNetwork password123        # Configure WiFi
WIFI_ON                                 # Enable WiFi
LORA_ON                                 # Enable LoRa receiver
SAVE                                    # Save configuration
```

## Default Values

### Cow Node
- Device Type: DEVICE_COW_NODE (1)
- Device ID: COW-<MAC_ADDRESS>
- Name: "Cow Collar 1"
- WiFi: Enabled, not configured
- LoRa: Enabled, 915 MHz
- Gender: Female
- Milk Monitoring: Enabled

### Milk Station
- Device Type: DEVICE_MILK_STATION (2)  
- Device ID: MILK-<MAC_ADDRESS>
- Name: "Milk Station 1"
- WiFi: Enabled, not configured
- LoRa: Enabled, 915 MHz

### Gateway
- Device Type: DEVICE_GATEWAY (3)
- Device ID: GW-<MAC_ADDRESS>
- Name: "Gateway 1"
- WiFi: Enabled, not configured
- LoRa: Enabled, 915 MHz (receive mode)

## Storage

Configuration is persisted in ESP32 NVS (Non-Volatile Storage) under namespace "device".
Settings survive power cycles and firmware updates.

To completely reset: `RESET` command or erase flash during upload.

## Network Architecture

```
┌─────────────┐         LoRa          ┌─────────────┐
│  Cow Node   │◄─────────────────────►│   Gateway   │
│  (Wearable) │                       │    Node     │
└─────────────┘                       └──────┬──────┘
                                             │
       LoRa                                  │ WiFi
        ↓                                    ↓
┌─────────────┐                        ┌─────────────┐
│Milk Station │         WiFi           │   Device    │
│    Node     │───────────────────────►│   Manager   │
└─────────────┘                        │   (Server)  │
                                       └─────────────┘
```

All devices can communicate via:
1. **LoRa** - Low power, long range (up to 10km)
2. **WiFi** - High bandwidth, short range
3. Gateway aggregates LoRa data and forwards via WiFi

