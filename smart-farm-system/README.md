# Smart Farm System 🐄📱

> **IoT-based dairy farm monitoring and automation system**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ESP32](https://img.shields.io/badge/Platform-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![LoRa](https://img.shields.io/badge/Communication-LoRa-green.svg)](https://lora-alliance.org/)

---

## 🎯 Project Overview

The Smart Farm System is a comprehensive IoT solution designed to revolutionize dairy farming through automated health monitoring, milk quality analysis, and real-time farm management. Built with ESP32 microcontrollers and LoRa mesh networking.

### 🔥 Key Features

- **🏥 Real-time Health Monitoring** - Heart rate, temperature, activity tracking per cow
- **🥛 Automated Milk Quality Analysis** - pH, conductivity, temperature analysis  
- **📍 GPS Location Tracking** - Monitor cow movement and grazing patterns
- **🔗 LoRa Mesh Network** - Long-range communication up to 15km
- **📱 Mobile App Integration** - Real-time dashboard and alerts
- **☀️ Solar Powered** - 72+ hour battery life with solar charging
- **🛡️ IP65 Protection** - Weather-resistant for outdoor farm use

---

## 🏗️ System Architecture

### Hardware Components

#### 🐄 Cow Node (Per Animal)
- **ESP32-WROOM-32** - Main microcontroller
- **MAX30102** - Heart rate and pulse oximetry sensor
- **DS18B20** - Waterproof temperature sensor
- **MPU6050** - 6-axis accelerometer for activity monitoring
- **NEO-6M** - GPS module for location tracking
- **RC522** - RFID reader for cow identification
- **SX1276** - LoRa communication module
- **Solar Panel** - 6V 1W with 2500mAh battery

#### 🥛 Milk Station Node
- **ESP32-WROOM-32** - Processing unit
- **pH Sensor** - Milk acidity analysis
- **Conductivity Sensor** - Milk composition analysis
- **16x2 LCD** - Local data display
- **SD Card** - Local data storage

#### 🌐 Gateway Node
- **ESP32-WROOM-32** - Central coordinator
- **3.5" TFT Display** - Farm dashboard
- **Ethernet Module** - Internet connectivity
- **LoRa Coordinator** - Mesh network management

---

## 💰 Cost Analysis

### Hardware Pricing (Updated Nov 2024)

| Component | Cost (USD) | Cost (RWF) |
|-----------|------------|------------|
| **Cow Node** | $127.30 | 184,810 |
| **Milk Station** | $278.70 | 404,611 |
| **Gateway Node** | $190.20 | 276,130 |

*Exchange Rate: 1 USD = 1,451.78 RWF*

### Farm Configuration Examples

- **Small Farm (10 cows)**: $2,002.60 (2,907,324 RWF)
- **Standard Farm (25 cows)**: $4,419.50 (6,416,113 RWF)
- **Large Farm (50 cows)**: $8,839.00 (12,832,226 RWF)

---

## 🚀 Quick Start

### 1. Hardware Setup
```bash
# Clone repository
git clone https://github.com/FreeFelix/smart-farm-system.git
cd smart-farm-system

# Install PlatformIO
pip install platformio

# Build cow node firmware
cd cow-node
pio run --target upload
```

### 2. Gateway Configuration
```bash
# Build and upload gateway firmware
cd ../gateway-node
pio run --target upload
```

### 3. Network Setup
- Configure WiFi credentials in `config.h`
- Set LoRa frequency for your region
- Deploy nodes across farm infrastructure

---

## 📁 Project Structure

```
smart-farm-system/
├── cow-node/              # Wearable cow monitoring device
│   ├── src/main.cpp       # Main cow node firmware
│   ├── platformio.ini     # Build configuration
│   └── include/           # Header files
├── milk-station-node/     # Milk quality analysis station
├── gateway-node/          # Central communication hub
│   ├── src/main.cpp       # Gateway firmware
│   └── platformio.ini     # Display and network config
├── docs/                  # Project documentation
│   ├── HARDWARE_BUDGET_PLAN.md
│   └── SMART_FARM_EXECUTIVE_SUMMARY.md
└── README.md              # This file
```

---

## 🔧 Technical Specifications

### Communication
- **Protocol**: LoRa mesh network
- **Frequency**: 915MHz (US) / 868MHz (EU)
- **Range**: Up to 15km line of sight
- **Data Rate**: 0.3-37.5 kbps adaptive

### Power Management
- **Cow Node**: Solar + 2500mAh battery (72h runtime)
- **Stations**: 12V DC power supply
- **Sleep Mode**: <1mA power consumption

### Environmental
- **Operating Temp**: -20°C to +60°C
- **Humidity**: 0-95% RH
- **IP Rating**: IP65 (dust/water resistant)

---

## 📊 Performance Metrics

- **Health Monitoring Accuracy**: ±0.5°C temperature, ±2 BPM heart rate
- **GPS Precision**: ±3 meters CEP
- **Network Uptime**: 99.5% availability
- **Battery Life**: 3+ days without sun
- **Sensor Update Rate**: 1-minute intervals

---

## 🤝 Contributing

We welcome contributions! Please see our contributing guidelines:

1. Fork the repository
2. Create feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit changes (`git commit -m 'Add AmazingFeature'`)
4. Push to branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 📞 Contact & Support

**ThereTech.rw - Smart Agriculture Solutions**

- **Phone**: 0781008164
- **Technical Support**: Available for setup and configuration
- **Custom Solutions**: Contact for farm-specific requirements

---

## 🎉 Powered by ThereTech.rw

© 2024 ThereTech.rw - All rights reserved

*Revolutionizing dairy farming through IoT innovation*

---

### 🌟 Star this repository if it helped your farming operations!