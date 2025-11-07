// Smart Cow Health Monitoring System - Data Structures
// Health data structure - Health monitoring only
// Milk production is tracked at Milk Station via RFID

#ifndef HEALTH_DATA_H
#define HEALTH_DATA_H

struct CowHealthData {
  // Temperature
  float temperature;
  bool tempAlert;
  
  // Activity
  bool isMoving;
  float activityLevel;
  float accelX, accelY, accelZ;
  unsigned long lastActivityTime;
  bool inactivityAlert;
  
  // GPS Data
  double latitude;
  double longitude;
  float altitude;
  int satellites;
  bool gpsValid;
  
  // Digestion & Power
  int soundLevel;
  bool isDigesting;
  int batteryLevel;
  float batteryVoltage;
  int solarVoltage;
  
  // System
  unsigned long timestamp;
  unsigned long loraPacketsSent;
};

// Global health data instance
extern CowHealthData healthData;

// System flags
extern bool gpsAvailable;
extern bool mpu6050Available;
extern bool loraAvailable;

#endif
