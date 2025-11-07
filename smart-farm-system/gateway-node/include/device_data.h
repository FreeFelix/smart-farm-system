#ifndef DEVICE_DATA_H
#define DEVICE_DATA_H

#include <Arduino.h>

// ==================== COW NODE DATA STRUCTURE ====================
struct CowData {
    String deviceId;
    float temperature;
    float heartRate;
    float activity;
    float batteryLevel;
    bool gpsFixed;
    float latitude;
    float longitude;
    String healthStatus;  // "HEALTHY", "WARNING", "CRITICAL"
    unsigned long timestamp;
    bool isValid;

    CowData() : temperature(0), heartRate(0), activity(0), 
                batteryLevel(0), gpsFixed(false), latitude(0), 
                longitude(0), healthStatus("UNKNOWN"), 
                timestamp(0), isValid(false) {}
};

// ==================== MILK STATION DATA STRUCTURE ====================
struct MilkData {
    String deviceId;
    String cowId;
    float temperature;
    float ph;
    float conductivity;
    float flowRate;
    float volume;
    String quality;  // "GOOD", "FAIR", "POOR"
    bool sessionActive;
    unsigned long sessionStart;
    unsigned long timestamp;
    bool isValid;

    MilkData() : temperature(0), ph(0), conductivity(0), 
                 flowRate(0), volume(0), quality("UNKNOWN"), 
                 sessionActive(false), sessionStart(0), 
                 timestamp(0), isValid(false) {}
};

// ==================== GATEWAY STATUS STRUCTURE ====================
struct GatewayStatus {
    bool wifiConnected;
    bool mqttConnected;
    bool loraActive;
    int cowNodesActive;
    int milkStationsActive;
    unsigned long uptime;
    int freeHeap;
    int rssi;
    unsigned long lastDataReceived;
    
    GatewayStatus() : wifiConnected(false), mqttConnected(false), 
                      loraActive(false), cowNodesActive(0), 
                      milkStationsActive(0), uptime(0), freeHeap(0), 
                      rssi(0), lastDataReceived(0) {}
};

#endif // DEVICE_DATA_H
