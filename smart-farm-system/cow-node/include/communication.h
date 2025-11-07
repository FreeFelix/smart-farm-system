// Smart Cow Health Monitoring System - Communication
// Dual communication: WiFi + LoRa for data transmission to Device Manager

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>

// Communication initialization
void initializeCommunication();
void connectToWiFi();

// Data transmission
void transmitLoRa();
void transmitViaWiFi();
void transmitHealthData();  // Automatically uses WiFi or LoRa based on availability

#endif
