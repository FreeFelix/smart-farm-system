// Smart Cow Health Monitoring System - System Initialization
// Handles all hardware initialization

#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TinyGPSPlus.h>
#include <LoRa.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include "system_init.h"
#include "config.h"
#include "health_data.h"
#include "communication.h"

// External global objects (defined in main.cpp)
extern OneWire oneWire;
extern DallasTemperature tempSensor;
extern TinyGPSPlus gps;
extern Adafruit_MPU6050 mpu;

void initializeHardware() {
  // Initialize all 3 status LEDs
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  
  // Turn all LEDs OFF initially
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
  
  // Initialize analog pins
  pinMode(MIC_PIN, INPUT);
  pinMode(BATTERY_PIN, INPUT);
  pinMode(SOLAR_PIN, INPUT);
  
  Serial.println("Basic hardware initialized");
}

void initializeI2C() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.println("I2C bus initialized");
}

void initializeTemperatureSensor() {
  Serial.println("Initializing DS18B20 temperature sensor...");
  tempSensor.begin();
  Serial.println("Temperature sensor ready");
}

void initializeMPU6050Sensor() {
  Serial.println("Initializing MPU6050 accelerometer...");
  if (!mpu.begin()) {
    Serial.println("WARNING: MPU6050 not found! Using simulated data");
    mpu6050Available = false;
  } else {
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("MPU6050 initialized successfully");
    mpu6050Available = true;
  }
}

void initializeGPS() {
  Serial2.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  Serial.println("GPS module initialized");
  gpsAvailable = true;
}

void initializeHealthData() {
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
  healthData.solarVoltage = 0;
  healthData.tempAlert = false;
  healthData.loraPacketsSent = 0;
  healthData.lastActivityTime = 0;
  healthData.inactivityAlert = false;
  
  Serial.println("Health data structure initialized");
}

void initializeAllSystems() {
  initializeHardware();
  initializeI2C();
  initializeTemperatureSensor();
  initializeMPU6050Sensor();
  initializeGPS();
  initializeCommunication();
  initializeHealthData();
  
  Serial.println("\n========================================");
  Serial.println("System initialized successfully!");
  Serial.println("Starting health monitoring...");
  Serial.println("========================================\n");
}
