// Smart Cow Health Monitoring System - Milk Production Monitoring
// Header file for milk production sensors (Optional Feature)

#ifndef MILK_MONITOR_H
#define MILK_MONITOR_H

#include <Arduino.h>
#include "config.h"
#include "health_data.h"

// Milk monitoring functions (only active when enabled)
void initializeMilkMonitoring();  // Setup milk sensors
void readMilkProduction();        // Read all milk sensors
void checkMilkQuality();          // Analyze milk quality
void disableMilkMonitoring();     // Disable milk sensors to save power

// Calibration functions
void calibrateMilkSensors();      // Initial calibration
void resetMilkSession();          // Reset session counters

#endif
