// Smart Cow Health Monitoring System - Sensors
// Temperature, GPS, and Activity sensor functions

#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

// Sensor initialization
void initializeSensors();

// Sensor reading functions
void readTemperature();
void readGPS();
void readMPU6050();
void readBattery();
void readMicrophone();

// Helper functions
void simulateGPS();
void simulateActivity();

#endif
