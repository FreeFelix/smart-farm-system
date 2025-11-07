// Smart Cow Health Monitoring System - Modular Version
// Wearable cow health monitor - No display (weatherproof design)
// Milk production is monitored at the Milk Station using cow RFID

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TinyGPSPlus.h>
#include <LoRa.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include "config.h"
#include "health_data.h"
#include "sensors.h"
#include "communication.h"
#include "health_monitor.h"
#include "device_config.h"
#include "system_init.h"

// Global object instances
OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);
TinyGPSPlus gps;
Adafruit_MPU6050 mpu;

// Global variables
CowHealthData healthData;
bool loraAvailable = false;
bool gpsAvailable = false;
bool mpu6050Available = false;
unsigned long lastTransmitTime = 0;

// WiFi check time is in communication.cpp
extern unsigned long lastWiFiCheck;

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n========================================");
  Serial.println("  SMART COW HEALTH MONITORING SYSTEM");
  Serial.println("  Wearable Sensor - Weatherproof Design");
  Serial.println("         Version 3.0 - No Display");
  Serial.println("========================================\n");
  
  // Load saved configuration
  initDeviceConfig();
  loadDeviceConfig();
  printDeviceConfig();
  
  Serial.println("Type 'HELP' for configuration options\n");
  
  // Initialize LEDs
  pinMode(STATUS_LED, OUTPUT);
  pinMode(ALERT_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);
  digitalWrite(ALERT_LED, LOW);
  
  // Initialize analog pins
  pinMode(MIC_PIN, INPUT);
  pinMode(BATTERY_PIN, INPUT);
  pinMode(SOLAR_PIN, INPUT);
  
  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  
  // Initialize Temperature Sensor
  Serial.println("Initializing DS18B20 temperature sensor...");
  tempSensor.begin();
  Serial.println("Temperature sensor ready");
  
  // Initialize MPU6050
  Serial.println("Initializing MPU6050 accelerometer...");
  if (!mpu.begin()) {
    Serial.println("WARNING: MPU6050 not found! Using simulated data");
  } else {
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("MPU6050 initialized successfully");
  }
  
  // Initialize GPS (for location tracking)
  Serial2.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  Serial.println("GPS module initialized");
  
  // Initialize LoRa
  initializeCommunication();
  
  // Initialize health data
  healthData.temperature = 0.0;
  healthData.activityLevel = 0.0;
  healthData.isMoving = false;
  healthData.soundLevel = 0;
  healthData.isDigesting = false;
  healthData.latitude = 0.0;
  healthData.longitude = 0.0;
  healthData.altitude = 0.0;
  healthData.satellites = 0;
  healthData.batteryVoltage = 0.0;
  healthData.solarVoltage = 0.0;
  healthData.tempAlert = false;
  healthData.loraPacketsSent = 0;
  healthData.lastActivityTime = 0;
  healthData.inactivityAlert = false;
  
  Serial.println("\n========================================");
  Serial.println("System initialized successfully!");
  Serial.println("Device Type: Wearable Cow Health Monitor");
  Serial.println("Cow Health Monitoring: ENABLED");
  Serial.print("Cow Gender: ");
  Serial.println(deviceConfig.cowGender == 1 ? "FEMALE" : "MALE");
  Serial.print("Milk Monitoring at Station: ");
  Serial.println(deviceConfig.milkMonitoring ? "YES" : "NO (Male cow)");
  Serial.println("Display: NONE (Weatherproof design)");
  Serial.println("Data transmission: LoRa only");
  Serial.println("Starting health monitoring...");
  Serial.println("========================================\n");
  
  delay(1000);
}

void loop() {
  // Check for configuration commands from Serial
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    processConfigCommand(command);
  }
  
  // Check WiFi connection periodically (every 30 seconds)
  if (deviceConfig.wifiEnabled && (millis() - lastWiFiCheck >= 30000)) {
    lastWiFiCheck = millis();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected, reconnecting...");
      connectToWiFi();
    }
  }
  
  // Read all sensors
  readTemperature();
  readGPS();
  readMPU6050();
  readBattery();
  readMicrophone();
  
  // Check for health alerts
  checkHealthAlerts();
  
  // Update status LEDs
  updateStatusLEDs();
  
  // Print health data to serial every 5 seconds
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 5000) {
    printHealthData();
    lastPrint = millis();
  }
  
  // Transmit data every 10 seconds (WiFi + LoRa)
  if (millis() - lastTransmitTime >= LORA_TRANSMIT_INTERVAL) {
    transmitHealthData();  // Uses both WiFi and LoRa if available
    lastTransmitTime = millis();
  }
  
  delay(100);
}
