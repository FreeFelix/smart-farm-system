// Milk Session Management Functions
#ifndef MILK_SESSION_H
#define MILK_SESSION_H

#include <Arduino.h>
#include "config.h"
#include "health_data.h"

void startMilkingSession();
void endMilkingSession();
void controlMilkFlow(bool open);
void resetDailyMilkTotal();

#endif
