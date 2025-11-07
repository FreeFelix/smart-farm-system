# SMART FARM SYSTEM - DETAILED BUDGET BREAKDOWN

## Executive Summary
**Project:** Smart Farm Cattle Monitoring System  
**Target:** 25-cow dairy farm implementation  
**Currency:** USD and Rwandan Francs (RWF)  
**Exchange Rate:** 1 USD = 1,000 RWF (as of Nov 2025)  

---

## 1. HARDWARE COSTS

### 1.1 Cow Node (Wearable Collar) - Unit Cost

| Component | Model/Specification | Qty | Unit Price (USD) | Unit Price (RWF) | Source/Supplier |
|-----------|-------------------|-----|------------------|------------------|-----------------|
| **Microcontroller** | ESP32-WROOM-32 DevKit | 1 | $8.70 | 8,700 | AliExpress/Local |
| **GPS Module** | NEO-6M/8M with antenna | 1 | $14.50 | 14,500 | SparkFun/Digi-Key |
| **Temperature Sensor** | DS18B20 Waterproof | 1 | $7.25 | 7,250 | Adafruit/Mouser |
| **Accelerometer** | MPU6050 6-axis IMU | 1 | $4.35 | 4,350 | Local electronics |
| **LoRa Module** | SX1276 Ra-02 915MHz | 1 | $14.50 | 14,500 | Hope RF/Semtech |
| **Battery Pack** | 2x 18650 Li-ion 3000mAh | 1 | $17.40 | 17,400 | Samsung/Panasonic |
| **Solar Panel** | 5V 1W Monocrystalline | 1 | $14.50 | 14,500 | SunPower/Jinko |
| **Charge Controller** | TP4056 + Protection PCB | 1 | $2.90 | 2,900 | Local supplier |
| **Enclosure** | IP67 Polycarbonate Box | 1 | $10.87 | 10,875 | Bud Industries |
| **Collar Strap** | Adjustable TPU/Nylon | 1 | $7.25 | 7,250 | Local leather goods |
| **PCB & Components** | Custom PCB + resistors/caps | 1 | $8.70 | 8,700 | JLCPCB/Local |
| **Antenna** | 915MHz LoRa antenna | 1 | $4.35 | 4,350 | Linx Technologies |
| **Connectors** | Waterproof connectors | 1 | $5.80 | 5,800 | Amphenol |
| **Assembly Labor** | PCB assembly & testing | 1 | $5.80 | 5,800 | Local technician |
| **Subtotal per Cow Node** | | | **$126.87** | **126,875** | |

### 1.2 Milk Station Node - Unit Cost

| Component | Model/Specification | Qty | Unit Price (USD) | Unit Price (RWF) | Source/Supplier |
|-----------|-------------------|-----|------------------|------------------|-----------------|
| **Microcontroller** | ESP32-WROOM-32 DevKit | 1 | $8.70 | 8,700 | AliExpress/Local |
| **Flow Sensor** | YF-S201 Hall Effect | 1 | $21.75 | 21,750 | SeeedStudio |
| **Load Cell System** | 5kg Load Cell + HX711 | 1 | $14.50 | 14,500 | SparkFun |
| **pH Sensor Kit** | Analog pH Probe + Module | 1 | $36.25 | 36,250 | Atlas Scientific |
| **Conductivity Sensor** | TDS/EC Probe + Module | 1 | $43.50 | 43,500 | DFRobot |
| **Temperature Sensor** | DS18B20 Food Grade | 2 | $10.87 | 10,875 | Maxim Integrated |
| **RFID Reader** | RC522 13.56MHz Module | 1 | $7.25 | 7,250 | NXP Semiconductors |
| **RFID Tags** | 125kHz Cow Ear Tags | 50 | $43.50 | 43,500 | Allflex/Datamars |
| **LoRa Module** | SX1276 Ra-02 915MHz | 1 | $14.50 | 14,500 | Hope RF |
| **LCD Display** | 20x4 I2C LCD Blue | 1 | $14.50 | 14,500 | Hitachi HD44780 |
| **Relay Module** | 4-Channel 5V Relays | 1 | $7.25 | 7,250 | Local electronics |
| **Solenoid Valve** | 12V 1/2" Normally Closed | 2 | $29.00 | 29,000 | ASCO/Parker |
| **Power Supply** | 12V 5A Switching PSU | 1 | $21.75 | 21,750 | Mean Well |
| **Enclosure** | NEMA 4X Stainless Steel | 1 | $58.00 | 58,000 | Hoffman/Rittal |
| **Sensors Housing** | Food Grade SS Tubing | 1 | $21.75 | 21,750 | Swagelok |
| **Cables & Fittings** | IP67 Cables/Connectors | 1 | $14.50 | 14,500 | Belden/Amphenol |
| **Assembly Labor** | Integration & Calibration | 1 | $21.75 | 21,750 | Skilled technician |
| **Subtotal per Milk Station** | | | **$389.62** | **389,625** | |

### 1.3 Gateway Node - Unit Cost

| Component | Model/Specification | Qty | Unit Price (USD) | Unit Price (RWF) | Source/Supplier |
|-----------|-------------------|-----|------------------|------------------|-----------------|
| **Microcontroller** | ESP32-WROOM-32 DevKit | 1 | $8.70 | 8,700 | Espressif |
| **TFT Display** | 3.5" ILI9341 Touch Screen | 1 | $29.00 | 29,000 | Adafruit |
| **LoRa Module** | SX1276 Ra-02 915MHz | 1 | $14.50 | 14,500 | Semtech |
| **SD Card Module** | MicroSD Card Reader | 1 | $4.35 | 4,350 | Local electronics |
| **RTC Module** | DS3231 Precision RTC | 1 | $7.25 | 7,250 | Maxim Integrated |
| **WiFi Antenna** | External 2.4GHz 5dBi | 1 | $7.25 | 7,250 | TP-Link |
| **Power Supply** | 12V 3A Wall Adapter | 1 | $14.50 | 14,500 | Mean Well |
| **UPS Battery** | 12V 7Ah SLA Battery | 1 | $21.75 | 21,750 | CSB/Yuasa |
| **Enclosure** | Desktop ABS Housing | 1 | $21.75 | 21,750 | OKW Enclosures |
| **Buzzer & LEDs** | Alert System Components | 1 | $7.25 | 7,250 | Local electronics |
| **Ethernet Module** | W5500 Ethernet Shield | 1 | $14.50 | 14,500 | WIZnet |
| **Antenna Extension** | LoRa Antenna + Cable | 1 | $10.87 | 10,875 | Times Microwave |
| **Assembly & Test** | Final Integration | 1 | $14.50 | 14,500 | System integrator |
| **Subtotal per Gateway** | | | **$176.17** | **176,175** | |

---

## 2. SYSTEM COST CALCULATION (25-Cow Farm)

### 2.1 Hardware Quantities Required

| Component | Quantity | Unit Cost (USD) | Unit Cost (RWF) | Total (USD) | Total (RWF) |
|-----------|----------|-----------------|-----------------|-------------|-------------|
| **Cow Nodes** | 25 | $126.87 | 126,875 | $3,171.75 | 3,171,875 |
| **Milk Stations** | 2 | $389.62 | 389,625 | $779.24 | 779,250 |
| **Gateway** | 1 | $176.17 | 176,175 | $176.17 | 176,175 |
| **Total Hardware** | | | | **$4,127.16** | **4,127,300** |

### 2.2 Additional System Components

| Item | Description | Cost (USD) | Cost (RWF) |
|------|-------------|------------|------------|
| **Installation Kit** | Mounting hardware, cables | $150.00 | 150,000 |
| **Calibration Equipment** | pH buffers, test solutions | $75.00 | 75,000 |
| **Spare Parts Kit** | 10% of hardware cost | $412.72 | 412,730 |
| **Tools & Equipment** | Basic maintenance tools | $100.00 | 100,000 |
| **Subtotal Additional** | | **$737.72** | **737,730** |

### 2.3 Software Development Costs

| Component | Description | Cost (USD) | Cost (RWF) |
|-----------|-------------|------------|------------|
| **Firmware Development** | Embedded software for all nodes | $1,200.00 | 1,200,000 |
| **Gateway UI Development** | Touch screen interface | $600.00 | 600,000 |
| **Cloud Platform** | Web dashboard and API | $800.00 | 800,000 |
| **Mobile App** | iOS/Android application | $500.00 | 500,000 |
| **Testing & QA** | System testing and validation | $400.00 | 400,000 |
| **Documentation** | User manuals and guides | $200.00 | 200,000 |
| **Subtotal Software** | | **$3,700.00** | **3,700,000** |

---

## 3. OPERATIONAL COSTS

### 3.1 First Year Operating Expenses

| Category | Item | Monthly (USD) | Monthly (RWF) | Annual (USD) | Annual (RWF) |
|----------|------|---------------|---------------|--------------|-------------|
| **Cloud Services** | AWS/Azure hosting | $25.00 | 25,000 | $300.00 | 300,000 |
| **Data Storage** | Database and backups | $10.00 | 10,000 | $120.00 | 120,000 |
| **Connectivity** | 4G/WiFi data plans | $15.00 | 15,000 | $180.00 | 180,000 |
| **Support** | Technical support staff | $200.00 | 200,000 | $2,400.00 | 2,400,000 |
| **Maintenance** | Hardware replacements | $50.00 | 50,000 | $600.00 | 600,000 |
| **Insurance** | Equipment coverage | $25.00 | 25,000 | $300.00 | 300,000 |
| **Total Monthly** | | **$325.00** | **325,000** | | |
| **Total Annual** | | | | **$3,900.00** | **3,900,000** |

### 3.2 5-Year Total Cost of Ownership

| Year | Hardware (USD) | Software (USD) | Operations (USD) | Total (USD) | Total (RWF) |
|------|----------------|----------------|------------------|-------------|-------------|
| **Year 0** | $4,864.88 | $3,700.00 | $0.00 | $8,564.88 | 8,564,880 |
| **Year 1** | $0.00 | $200.00 | $3,900.00 | $4,100.00 | 4,100,000 |
| **Year 2** | $487.27 | $200.00 | $4,095.00 | $4,782.27 | 4,782,270 |
| **Year 3** | $0.00 | $200.00 | $4,299.75 | $4,499.75 | 4,499,750 |
| **Year 4** | $487.27 | $200.00 | $4,514.74 | $5,202.01 | 5,202,010 |
| **Year 5** | $0.00 | $200.00 | $4,740.48 | $4,940.48 | 4,940,480 |
| **Total 5-Year** | | | | **$32,089.39** | **32,089,390** |

---

## 4. COST-BENEFIT ANALYSIS

### 4.1 Current Traditional Farming Costs (Annual)

| Category | Description | Cost (USD) | Cost (RWF) |
|----------|-------------|------------|------------|
| **Labor** | Manual monitoring (2h/day × $2/h) | $1,460.00 | 1,460,000 |
| **Veterinary** | Regular checkups + treatments | $750.00 | 750,000 |
| **Feed Waste** | Poor health monitoring | $500.00 | 500,000 |
| **Milk Loss** | Quality issues, late detection | $800.00 | 800,000 |
| **Record Keeping** | Manual data entry | $200.00 | 200,000 |
| **Total Traditional** | | **$3,710.00** | **3,710,000** |

### 4.2 Smart Farm System Benefits (Annual)

| Category | Description | Savings (USD) | Savings (RWF) |
|----------|-------------|---------------|---------------|
| **Labor Reduction** | 70% less manual monitoring | $1,022.00 | 1,022,000 |
| **Early Disease Detection** | 50% reduction in vet costs | $375.00 | 375,000 |
| **Feed Optimization** | Better health = efficient feeding | $350.00 | 350,000 |
| **Milk Quality** | Higher grade milk premiums | $600.00 | 600,000 |
| **Production Increase** | 5% more milk per cow | $1,250.00 | 1,250,000 |
| **Total Annual Benefits** | | **$3,597.00** | **3,597,000** |

### 4.3 Return on Investment (ROI) Calculation

| Metric | Value (USD) | Value (RWF) |
|--------|-------------|-------------|
| **Initial Investment** | $8,564.88 | 8,564,880 |
| **Annual Net Benefit** | $3,597.00 | 3,597,000 |
| **Payback Period** | **2.38 years** | **2.38 years** |
| **5-Year Total Benefits** | $17,985.00 | 17,985,000 |
| **5-Year Net Profit** | $9,420.12 | 9,420,120 |
| **5-Year ROI** | **110%** | **110%** |

---

## 5. FINANCING OPTIONS

### 5.1 Payment Plans

#### Option A: Full Payment
- **Upfront Cost**: $8,564.88 USD / 8,564,880 RWF
- **Discount**: 5% ($428.24 USD / 428,244 RWF)
- **Final Cost**: $8,136.64 USD / 8,136,636 RWF

#### Option B: 12-Month Plan
- **Down Payment**: $2,500.00 USD / 2,500,000 RWF
- **Monthly Payment**: $530.00 USD / 530,000 RWF
- **Interest**: 5% APR
- **Total Cost**: $8,860.00 USD / 8,860,000 RWF

#### Option C: 24-Month Plan
- **Down Payment**: $2,000.00 USD / 2,000,000 RWF
- **Monthly Payment**: $295.00 USD / 295,000 RWF
- **Interest**: 7% APR
- **Total Cost**: $9,080.00 USD / 9,080,000 RWF

### 5.2 Leasing Option
- **Monthly Lease**: $380.00 USD / 380,000 RWF
- **Lease Term**: 36 months
- **Purchase Option**: $1,000.00 USD / 1,000,000 RWF
- **Total Cost**: $14,680.00 USD / 14,680,000 RWF

### 5.3 Government Financing Programs

#### Rwanda Agriculture Finance Program
- **Loan Amount**: Up to 80% of project cost
- **Interest Rate**: 3-5% annually
- **Repayment**: 5-7 years
- **Eligibility**: Registered farmers with collateral

#### Cooperative Financing
- **Group Purchase**: 10+ farmers together
- **Bulk Discount**: 15% off total cost
- **Shared Gateway**: Multiple farms share central system
- **Cost per Farm**: $6,000 USD / 6,000,000 RWF

---

## 6. RISK MITIGATION COSTS

### 6.1 Insurance Coverage

| Type | Annual Premium (USD) | Annual Premium (RWF) | Coverage |
|------|---------------------|---------------------|----------|
| **Equipment Insurance** | $120.00 | 120,000 | Hardware replacement |
| **Business Interruption** | $80.00 | 80,000 | Lost income coverage |
| **Cyber Liability** | $60.00 | 60,000 | Data breach protection |
| **Total Insurance** | **$260.00** | **260,000** | |

### 6.2 Warranty and Support

| Service | Duration | Cost (USD) | Cost (RWF) |
|---------|----------|------------|------------|
| **Standard Warranty** | 2 years | Included | Included |
| **Extended Warranty** | +3 years | $500.00 | 500,000 |
| **Priority Support** | 24/7 support | $200/year | 200,000/year |
| **Training Program** | 2-day course | $300.00 | 300,000 |

---

## 7. BUDGET SUMMARY

### 7.1 Complete System Cost Breakdown

| Category | Cost (USD) | Cost (RWF) | Percentage |
|----------|------------|------------|------------|
| **Hardware** | $4,127.16 | 4,127,160 | 48.2% |
| **Additional Components** | $737.72 | 737,720 | 8.6% |
| **Software Development** | $3,700.00 | 3,700,000 | 43.2% |
| **Total Project Cost** | **$8,564.88** | **8,564,880** | **100%** |

### 7.2 Per-Cow Investment
- **Hardware per Cow**: $165.09 USD / 165,088 RWF
- **Software per Cow**: $148.00 USD / 148,000 RWF
- **Total per Cow**: **$342.60 USD / 342,595 RWF**

### 7.3 Monthly Cost Comparison

| System | Monthly Cost (USD) | Monthly Cost (RWF) |
|--------|-------------------|-------------------|
| **Traditional Farming** | $309.17 | 309,170 |
| **Smart Farm System** | $179.95 | 179,950 |
| **Monthly Savings** | **$129.22** | **129,220** |
| **Annual Savings** | **$1,550.64** | **1,550,640** |

---

**Document Prepared By:** Smart Agriculture Solutions Team  
**Date:** November 7, 2025  
**Currency Exchange Rate:** 1 USD = 1,000 RWF  
**Pricing Valid Until:** December 31, 2025  

**Contact for Quotes:**
- Email: quotes@smartfarm.rw
- Phone: +250 XXX XXX XXX
- Website: www.smartfarm.rw

---

*All prices are subject to change based on component availability and exchange rates. Bulk discounts available for orders of 10+ systems.*