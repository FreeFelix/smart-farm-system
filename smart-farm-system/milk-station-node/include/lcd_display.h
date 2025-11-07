// LCD Display Module - Header
// 20x4 LCD display for milk station

#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Arduino.h>
#include "health_data.h"

void setupLCD();
void displayWelcome();
void displayWaitingForCow();
void displayMilkingSession(const String &cowID, float volume, float weight);
void displayMilkQuality(const CowHealthData &data);
void displaySessionComplete(float totalVolume, float totalWeight);
void displayAlert(const String &message);

#endif
