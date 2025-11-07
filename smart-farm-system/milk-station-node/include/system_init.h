// System Initialization Functions
#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include <Arduino.h>

void initializeHardware();
void initializeI2C();
void initializeTemperatureSensor();
void initializeMPU6050Sensor();
void initializeOLED();
void initializeGPS();
void initializeHealthData();
void initializeAllSystems();

#endif
