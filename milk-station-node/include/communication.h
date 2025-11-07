// Milk Station Node - Communication
// LoRa data transmission functions

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include "health_data.h"

// Communication initialization
bool setupLoRa();

// Data transmission
void transmitMilkData(const String &cowID, const CowHealthData &data);

#endif
