// Smart Cow Health Monitoring System - Sensor Implementation

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TinyGPSPlus.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "config.h"
#include "health_data.h"
#include "sensors.h"

// Sensor objects
extern OneWire oneWire;
extern DallasTemperature tempSensor;
extern TinyGPSPlus gps;
extern Adafruit_MPU6050 mpu;

void readTemperature() {
  tempSensor.requestTemperatures();
  float temp = tempSensor.getTempCByIndex(0);
  
  if (temp > -50 && temp < 60) {
    healthData.temperature = temp;
  } else {
    // Simulate normal cow temperature
    healthData.temperature = 38.5 + random(-10, 30) / 10.0;
  }
  
  healthData.timestamp = millis();
}

void readGPS() {
  if (gpsAvailable && gps.location.isValid()) {
    healthData.latitude = gps.location.lat();
    healthData.longitude = gps.location.lng();
    healthData.altitude = gps.altitude.meters();
    healthData.satellites = gps.satellites.value();
    healthData.gpsValid = true;
  } else {
    simulateGPS();
  }
}

void simulateGPS() {
  // Simulate GPS coordinates (Kigali, Rwanda area)
  healthData.latitude = -1.9441 + random(-100, 100) / 10000.0;
  healthData.longitude = 30.0619 + random(-100, 100) / 10000.0;
  healthData.altitude = 1500 + random(-50, 50);
  healthData.satellites = random(4, 12);
  healthData.gpsValid = true;
}

void readMPU6050() {
  if (mpu6050Available) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    healthData.accelX = a.acceleration.x;
    healthData.accelY = a.acceleration.y;
    healthData.accelZ = a.acceleration.z;
    
    // Calculate activity level
    float accelMagnitude = sqrt(
      pow(a.acceleration.x, 2) + 
      pow(a.acceleration.y, 2) + 
      pow(a.acceleration.z, 2)
    );
    healthData.activityLevel = accelMagnitude;
    healthData.isMoving = (accelMagnitude > 10.0);
  } else {
    simulateActivity();
  }
}

void simulateActivity() {
  int activity = random(0, 100);
  healthData.isMoving = (activity > 40);
  healthData.activityLevel = healthData.isMoving ? random(100, 200) / 10.0 : random(50, 100) / 10.0;
  healthData.accelX = random(-100, 100) / 10.0;
  healthData.accelY = random(-100, 100) / 10.0;
  healthData.accelZ = random(80, 120) / 10.0;
}

void readBattery() {
  int rawValue = analogRead(BATTERY_PIN);
  float voltage = (rawValue / 4095.0) * 3.3 * 2.0;
  
  healthData.batteryLevel = map(voltage * 100, 300, 420, 0, 100);
  healthData.batteryLevel = constrain(healthData.batteryLevel, 0, 100);
  
  int solarRaw = analogRead(SOLAR_PIN);
  healthData.solarVoltage = (solarRaw / 4095.0) * 3.3 * 2.0 * 100;
}

void readMicrophone() {
  int rawSound = analogRead(MIC_PIN);
  
  static int samples[10] = {0};
  static int sampleIndex = 0;
  
  samples[sampleIndex] = rawSound;
  sampleIndex = (sampleIndex + 1) % 10;
  
  int sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += samples[i];
  }
  healthData.soundLevel = sum / 10;
  healthData.isDigesting = (healthData.soundLevel > DIGESTION_THRESHOLD);
}
