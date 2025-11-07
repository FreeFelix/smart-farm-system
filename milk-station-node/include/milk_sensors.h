// Milk Sensors Reading Functions
#ifndef MILK_SENSORS_H
#define MILK_SENSORS_H

#include <Arduino.h>
#include "config.h"
#include "health_data.h"

void readMilkFlow();
void readMilkTemperature();
void readMilkPH();
void readMilkConductivity();
void readMilkWeight();
void readAllMilkSensors();

#endif
