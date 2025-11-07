// Smart Cow Health Monitoring System - Data Structures
// Health data structure and global variables

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
  
  // Milk Production Data
  bool isMilkProducing;       // Is this a milking cow?
  float milkVolume;            // Current milking session volume (liters)
  float milkFlowRate;          // Current flow rate (L/min)
  float milkTemperature;       // Milk temperature (°C)
  float milkPH;                // Milk pH level
  float milkConductivity;      // Milk conductivity (mS/cm)
  float milkWeight;            // Milk weight (kg)
  bool milkTempAlert;          // Milk temperature out of range
  bool milkPHAlert;            // Milk pH abnormal
  bool mastitisAlert;          // Mastitis detected via conductivity
  unsigned long milkSessionStart;  // Milking session start time
  float totalDailyMilk;        // Total milk today (liters)
  
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
extern bool oledAvailable;

#endif
