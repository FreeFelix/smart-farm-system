// Smart Cow Health Monitoring System - Configuration
// Pin definitions and system constants

#ifndef CONFIG_H
#define CONFIG_H

// ==================== PIN DEFINITIONS ====================
// Temperature Sensor
#define TEMP_PIN 4
#define TEMP_SENSOR_PIN 4    // Alias for compatibility

// Status LEDs
#define LED_GREEN 27         // Green LED - Healthy status
#define LED_YELLOW 26        // Yellow LED - Warning
#define LED_RED 25           // Red LED - Alert/Critical
#define STATUS_LED 27        // Alias for compatibility
#define ALERT_LED 25         // Alias for compatibility

// GPS (Serial2)
#define GPS_RX 16
#define GPS_TX 17

// LoRa SPI
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 23
#define LORA_CS 18
#define LORA_RST 14
#define LORA_DIO0 26
#define LORA_IRQ 26          // Alias for LoRa interrupt

// Microphone & Power Monitoring
#define MIC_PIN 34           // Analog microphone sensor (digestion sounds)
#define BATTERY_PIN 35       // Battery voltage monitoring (ADC)
#define SOLAR_PIN 32         // Solar panel voltage monitoring

// Milk Production Sensors
#define MILK_FLOW_PIN 25     // Milk flow sensor (YF-S201) - pulse counter
#define MILK_TEMP_PIN 33     // Milk temperature sensor (DS18B20)
#define MILK_PH_PIN 36       // pH sensor (analog)
#define MILK_CONDUCT_PIN 39  // Conductivity sensor (analog)
#define HX711_DOUT 13        // Load cell data pin
#define HX711_SCK 12         // Load cell clock pin
#define SOLENOID_PIN 15      // Solenoid valve control

// I2C for MPU6050 and OLED
#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SDA 21           // Alias for I2C SDA
#define I2C_SCL 22           // Alias for I2C SCL

// OLED Display (SSD1306)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// ==================== DEVICE CONFIGURATION ====================
// Configure these settings during device setup
#define COW_GENDER_MALE 0
#define COW_GENDER_FEMALE 1

// Set during device configuration
extern bool MILK_MONITORING_ENABLED;  // Can be changed at runtime
extern uint8_t COW_GENDER;            // 0 = Male, 1 = Female

// ==================== SYSTEM CONSTANTS ====================
#define READING_INTERVAL 2000           // 2 seconds
#define LORA_INTERVAL 10000             // 10 seconds
#define LORA_TRANSMIT_INTERVAL 10000    // 10 seconds (alias)
#define LORA_FREQUENCY 915E6            // 915 MHz for North America
#define INACTIVITY_ALERT_TIME 1800000   // 30 minutes in milliseconds

// Health thresholds
#define TEMP_MIN 37.5              // Minimum normal temperature (°C)
#define TEMP_MAX 39.5              // Maximum normal temperature (°C)
#define ACTIVITY_HIGH 15.0         // High activity threshold (m/s²)
#define ACTIVITY_MODERATE 10.0     // Moderate activity threshold
#define ACTIVITY_LIGHT 5.0         // Light activity threshold
#define DIGESTION_THRESHOLD 500    // Sound level for active digestion

// Milk production thresholds
#define MILK_TEMP_MIN 2.0          // Minimum safe milk temperature (°C)
#define MILK_TEMP_MAX 6.0          // Maximum safe milk temperature (°C)
#define MILK_PH_MIN 6.4            // Minimum normal milk pH
#define MILK_PH_MAX 6.8            // Maximum normal milk pH
#define MILK_CONDUCT_NORMAL 4.5    // Normal milk conductivity (mS/cm)
#define MILK_CONDUCT_ALERT 5.5     // Mastitis alert threshold (mS/cm)
#define MIN_MILK_VOLUME 5.0        // Minimum expected milk volume (liters)

#endif
