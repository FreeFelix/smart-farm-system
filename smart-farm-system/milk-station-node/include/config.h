// Milk Station Node - Configuration
// Pin definitions and system constants for fixed milking station

#ifndef CONFIG_H
#define CONFIG_H

// ==================== PIN DEFINITIONS ====================
// LEDs
#define STATUS_LED 2         // Green LED - System OK
#define ALERT_LED 27         // Red LED - Alert condition
#define BOOT_LED 14          // Blue LED - Booting

// LoRa SPI Communication
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 23
#define LORA_CS 18
#define LORA_RST 14
#define LORA_DIO0 26
#define LORA_IRQ 26          // Alias for LoRa interrupt

// Milk Production Sensors
#define MILK_FLOW_PIN 25     // Milk flow sensor (YF-S201) - pulse counter
#define MILK_TEMP_PIN 4      // Milk temperature sensor (DS18B20)
#define MILK_PH_PIN 36       // pH sensor (analog)
#define MILK_CONDUCT_PIN 39  // Conductivity sensor (analog)
#define HX711_DOUT 13        // Load cell data pin
#define HX711_SCK 12         // Load cell clock pin
#define SOLENOID_PIN 15      // Solenoid valve control

// RFID Reader (MFRC522) - SPI
#define RFID_SDA 17          // RFID SDA/SS pin
#define RFID_RST 16          // RFID Reset pin

// I2C for LCD Display
#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SDA 21           // Alias for I2C SDA
#define I2C_SCL 22           // Alias for I2C SCL

// LCD Display (20x4 I2C)
#define LCD_ADDRESS 0x27
#define LCD_COLS 20
#define LCD_ROWS 4

// ==================== SYSTEM CONSTANTS ====================
#define READING_INTERVAL 2000           // 2 seconds
#define LORA_INTERVAL 10000             // 10 seconds
#define LORA_TRANSMIT_INTERVAL 10000    // 10 seconds (alias)
#define LORA_FREQUENCY 915E6            // 915 MHz for North America

// Milk production thresholds
#define MILK_TEMP_MIN 2.0          // Minimum safe milk temperature (°C)
#define MILK_TEMP_MAX 6.0          // Maximum safe milk temperature (°C)
#define MILK_PH_MIN 6.4            // Minimum normal milk pH
#define MILK_PH_MAX 6.8            // Maximum normal milk pH
#define MILK_CONDUCT_NORMAL 4.5    // Normal milk conductivity (mS/cm)
#define MILK_CONDUCT_ALERT 5.5     // Mastitis alert threshold (mS/cm)
#define MIN_MILK_VOLUME 5.0        // Minimum expected milk volume (liters)
#define MILK_SESSION_TIMEOUT 300000  // 5 minutes session timeout

// RFID settings
#define RFID_SCAN_INTERVAL 1000    // Scan for cow tag every 1 second

#endif
