// Milk Quality Alert Functions
#ifndef MILK_QUALITY_H
#define MILK_QUALITY_H

#include <Arduino.h>
#include "config.h"
#include "health_data.h"

void checkMilkQualityAlerts();
bool isMilkQualityGood();
String getMilkQualityStatus();

#endif
