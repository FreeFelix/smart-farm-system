# Wokwi Simulation Guide - Milk Station Node

## Overview
This guide explains how to simulate the Milk Station Node using Wokwi, a browser-based electronics simulator that supports ESP32 and various sensors.

## What is Wokwi?
Wokwi is an online electronics simulator that allows you to:
- Test ESP32 code without physical hardware
- Interact with virtual sensors and displays
- Debug firmware before hardware deployment
- Share simulations with team members

## Quick Start

### Option 1: VS Code Wokwi Extension (Recommended)

1. **Install Wokwi Extension**
   - Open VS Code
   - Go to Extensions (Ctrl+Shift+X)
   - Search for "Wokwi Simulator"
   - Click Install

2. **Open Project**
   ```bash
   cd milk-station-node
   code .
   ```

3. **Build Firmware**
   - Open terminal in VS Code
   - Run: `pio run`
   - Wait for successful build

4. **Start Simulation**
   - Press F1 (Command Palette)
   - Type: "Wokwi: Start Simulator"
   - Select Enter
   - Wokwi window opens with your circuit

5. **Interact with Simulation**
   - Click on sensors to adjust values
   - Rotate potentiometers
   - Press buttons
   - View LCD display updates
   - Monitor serial output

### Option 2: Wokwi Web Interface

1. **Build Firmware**
   ```bash
   cd milk-station-node
   pio run
   ```

2. **Upload to Wokwi**
   - Go to https://wokwi.com
   - Create new ESP32 project
   - Import `diagram.json` from this folder
   - Upload `.pio/build/esp32dev/firmware.elf`

3. **Run Simulation**
   - Click green "Start Simulation" button
   - Interact with components

## Circuit Components

### ESP32 DevKit C V4
- **Main microcontroller**
- 240 MHz dual-core processor
- WiFi and Bluetooth built-in
- 40 GPIO pins

### DS18B20 Temperature Sensor
- **Purpose**: Measures milk temperature
- **Pin**: GPIO 4
- **Range**: -55°C to +125°C
- **Accuracy**: ±0.5°C
- **Wokwi Control**: Click sensor to adjust temperature
- **Normal Milk Temp**: 2-6°C

### 20x4 LCD Display (I2C)
- **Purpose**: Shows milk data to farmer
- **Interface**: I2C (SDA: GPIO 21, SCL: GPIO 22)
- **Address**: 0x27
- **Display**:
  - Line 1: Cow ID or status
  - Line 2: Milk volume
  - Line 3: Temperature and pH
  - Line 4: Alerts or status

### Status LEDs

#### Green LED (GPIO 2)
- **Status**: System OK
- **Lights**: When all systems normal
- **Resistor**: 220Ω

#### Red LED (GPIO 27)
- **Status**: Alert condition
- **Lights**: Quality issues, errors
- **Resistor**: 220Ω

#### Blue LED (GPIO 14)
- **Status**: Boot indicator
- **Lights**: During system initialization
- **Resistor**: 220Ω

### Potentiometers (Sensors Simulation)

#### pH Potentiometer (GPIO 36)
- **Purpose**: Simulates milk pH sensor
- **Range**: 0-1023 (ADC)
- **Normal Milk pH**: 6.4-6.8
- **Wokwi Control**: Rotate to adjust
- **Mapping**:
  - Low (0-300): pH < 6.4 (acidic, ALERT)
  - Mid (300-700): pH 6.4-6.8 (normal)
  - High (700-1023): pH > 6.8 (alkaline, ALERT)

#### Conductivity Potentiometer (GPIO 39)
- **Purpose**: Simulates milk conductivity sensor
- **Range**: 0-1023 (ADC)
- **Normal**: ~4.5 mS/cm
- **Mastitis Alert**: >5.5 mS/cm
- **Wokwi Control**: Rotate to adjust
- **Mapping**:
  - Low (0-500): Normal (4.0-5.0 mS/cm)
  - High (500-1023): High conductivity (>5.5 mS/cm, mastitis warning)

### Flow Pulse Button (GPIO 25)
- **Purpose**: Simulates milk flow sensor pulses
- **Type**: Pushbutton
- **Wokwi Control**: Click to send pulse
- **Function**: Each click = milk flow pulse
- **Volume Calculation**: Pulses counted → liters calculated

## Wokwi Configuration Files

### wokwi.toml
```toml
[wokwi]
version = 1
elf = ".pio/build/esp32dev/firmware.elf"
firmware = ".pio/build/esp32dev/firmware.bin"
```

This file tells Wokwi where to find your compiled firmware.

### diagram.json
Contains the complete circuit layout:
- Component positions
- Pin connections
- Component properties
- Wire routing

## Simulation Scenarios

### Scenario 1: Normal Milking Session

**Steps**:
1. Start simulation
2. Observe boot sequence (Blue LED blinks)
3. LCD shows "Milk Station Ready"
4. Green LED turns ON (system healthy)
5. Serial shows "Waiting for cow tag..."
6. Click Flow Button 10 times (simulate milk flow)
7. LCD updates: Volume increases
8. Temperature shows ~4.5°C
9. pH shows ~6.6 (mid-range)
10. Conductivity normal
11. Green LED stays ON (all good)

**Expected Serial Output**:
```
Milk Station Node Initialized
LCD: Milk Station Ready
Temperature: 4.5°C
pH: 6.6
Conductivity: 4.5 mS/cm
Milk Flow Detected
Volume: 2.1 L
Status: Good Quality
```

### Scenario 2: High pH Alert

**Steps**:
1. Start simulation
2. Rotate pH potentiometer to HIGH position
3. Observe Red LED turns ON
4. LCD shows "ALERT: High pH"
5. Serial shows pH alert message

**Expected Behavior**:
- Red LED: ON (alert)
- Green LED: OFF
- LCD Line 4: "pH Alert: 7.2"
- Serial: "WARNING: Milk pH out of range"

### Scenario 3: Mastitis Detection

**Steps**:
1. Start simulation
2. Rotate Conductivity potentiometer to HIGH
3. Observe Red LED blinks rapidly
4. LCD shows "WARNING: High Conductivity"
5. Serial shows mastitis warning

**Expected Behavior**:
- Red LED: Blinking (urgent)
- LCD Line 4: "Check Udder Health"
- Serial: "MASTITIS ALERT: Conductivity 6.2 mS/cm"

### Scenario 4: Temperature Alert

**Steps**:
1. Click on DS18B20 sensor
2. Set temperature to 8°C (above normal)
3. Observe Red LED ON
4. LCD shows "Temp Too High"

**Expected Behavior**:
- Red LED: ON
- LCD: "Temp: 8.0°C ALERT"
- Serial: "Milk temperature out of safe range"

## Interactive Controls

### Adjusting Temperature
1. Click on DS18B20 sensor in Wokwi
2. Use slider or type value
3. Range: -55°C to +125°C
4. Normal milk: 2-6°C
5. Changes reflected immediately

### Adjusting pH
1. Click on pH potentiometer
2. Rotate knob or use slider
3. Low position: Acidic (pH < 6.4)
4. Mid position: Normal (pH 6.4-6.8)
5. High position: Alkaline (pH > 6.8)

### Adjusting Conductivity
1. Click on Conductivity potentiometer
2. Rotate knob or use slider
3. Low position: Normal (4-5 mS/cm)
4. High position: Mastitis alert (>5.5 mS/cm)

### Simulating Milk Flow
1. Click Flow Pulse button repeatedly
2. Each click = one pulse from flow sensor
3. Volume calculated: pulses × calibration factor
4. LCD updates volume in real-time

## LCD Display Layout

```
┌────────────────────┐
│ COW-001  Milking   │  Line 1: Cow ID & Status
│ Volume: 12.5 L     │  Line 2: Milk Volume
│ Temp:4.5C pH:6.6   │  Line 3: Quality Metrics
│ Status: Good       │  Line 4: Overall Status
└────────────────────┘
```

### Display States

**Idle State**:
```
  Milk Station v1.0
    System Ready
   Scan Cow Tag
```

**Milking State**:
```
 COW-001  Milking
  Volume: 8.2 L
 Temp:4.2C pH:6.5
  Status: Good
```

**Alert State**:
```
 COW-001  ALERT!
  Volume: 5.1 L
 Temp:8.5C pH:7.2
  High Temp & pH!
```

## Serial Monitor

### Viewing Serial Output

**VS Code Wokwi**:
- Serial output appears in Wokwi terminal panel
- Auto-scrolls with new messages

**Wokwi Web**:
- Click "Serial Monitor" tab
- Shows all Serial.print() output
- Baud rate: 115200 (auto-detected)

### Expected Serial Messages

**Boot Sequence**:
```
=================================
   MILK STATION NODE v1.0
=================================
Initializing hardware...
I2C initialized
LCD initialized
Temperature sensor: OK
LoRa: Initializing...
LoRa: Ready (915 MHz)
WiFi: Not configured
System ready. Waiting for cow tag...
```

**During Milking**:
```
[12:34:56] Temperature: 4.5°C
[12:34:56] pH: 6.6
[12:34:56] Conductivity: 4.5 mS/cm
[12:34:57] Milk flow detected
[12:34:57] Volume: 2.1 L
[12:34:58] Volume: 2.3 L
[12:35:00] Quality: Good
[12:35:00] Transmitting data...
```

**Alerts**:
```
!!!!! QUALITY ALERT !!!!!
Parameter: pH
Current: 7.2
Normal Range: 6.4-6.8
Recommendation: Check milk quality
!!!!!!!!!!!!!!!!!!!!!!!!
```

## Troubleshooting

### LCD Shows Nothing

**Possible Causes**:
- I2C address mismatch
- SDA/SCL not connected
- LCD not initialized

**Solutions**:
1. Check I2C address in code (0x27 or 0x3F)
2. Verify SDA/SCL connections in diagram.json
3. Check serial output for LCD init errors

### Temperature Always Shows 0°C or 85°C

**Possible Causes**:
- DS18B20 not connected
- Missing pull-up resistor
- Wrong GPIO pin

**Solutions**:
1. Verify DS18B20 connection to GPIO 4
2. Check 4.7kΩ pull-up resistor present
3. Ensure OneWire library included

### LEDs Not Working

**Possible Causes**:
- Wrong GPIO pins
- Missing resistors
- Pin mode not set

**Solutions**:
1. Verify GPIO pins: 2 (green), 27 (red), 14 (blue)
2. Check 220Ω resistors in diagram
3. Ensure pinMode(LED, OUTPUT) in code

### No LoRa Transmission

**Note**: LoRa module not available in Wokwi free tier

**Solutions**:
1. Check serial output for "LoRa init failed"
2. Code should handle LoRa absence gracefully
3. WiFi can still transmit data

## Performance Notes

### Simulation Speed
- Wokwi runs at ~80% real-time speed
- Delays may be slightly longer
- Does not affect functionality

### Memory Usage
- Same as real ESP32
- Check serial for memory warnings
- Flash: ~75% used
- RAM: ~15% used

### Component Limitations

**Available in Wokwi**:
- ✅ ESP32 DevKit C
- ✅ DS18B20 temperature sensor
- ✅ LCD 20x4 I2C display
- ✅ LEDs and resistors
- ✅ Potentiometers (ADC simulation)
- ✅ Pushbuttons
- ✅ Serial monitor

**Not Available**:
- ❌ RFID MFRC522 (must simulate via serial input)
- ❌ LoRa SX1276 (check for library presence)
- ❌ HX711 load cell (use potentiometer)
- ❌ Real flow sensor (use button)

## Tips for Best Results

1. **Build First**: Always run `pio run` before starting simulation
2. **Check Serial**: Monitor serial output for errors
3. **Save Often**: Wokwi auto-saves, but export diagram.json regularly
4. **Test Incrementally**: Test one sensor at a time
5. **Use Realistic Values**: Set temperature to 4°C, pH to 6.6, etc.
6. **Document Changes**: Note any modifications to diagram.json
7. **Share Links**: Wokwi generates shareable simulation links

## Next Steps

1. Run `./simulate_wokwi.sh` for detailed instructions
2. Build firmware: `pio run`
3. Start Wokwi in VS Code: F1 → "Wokwi: Start Simulator"
4. Test all scenarios documented above
5. Modify sensor values and observe behavior
6. Note any issues for hardware deployment

## Support

For Wokwi-specific issues:
- Wokwi Documentation: https://docs.wokwi.com
- Wokwi Discord: https://wokwi.com/discord
- VS Code Extension: https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode

For project-specific issues:
- Check serial output for error messages
- Review README.md for hardware details
- Consult CONFIGURATION.md for settings
