#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include "config.h"
#include "device_data.h"

class DataStorage {
private:
    bool sdInitialized;
    bool dbInitialized;
    File logFile;
    
    // Ring buffers for recent data
    CowData cowDataHistory[HISTORY_SIZE];
    MilkData milkDataHistory[HISTORY_SIZE];
    int cowDataIndex;
    int milkDataIndex;
    
    // Device registry
    String registeredCowNodes[MAX_COW_NODES];
    String registeredMilkStations[MAX_MILK_STATIONS];
    int numCowNodes;
    int numMilkStations;
    
    unsigned long lastSaveTime;
    
public:
    DataStorage();
    bool begin();
    bool isSDAvailable();
    
    // Data storage
    bool saveCowData(const CowData& data);
    bool saveMilkData(const MilkData& data);
    bool saveGatewayStatus(const GatewayStatus& status);
    
    // Data retrieval
    bool getCowData(String deviceId, CowData& data);
    bool getMilkData(String deviceId, MilkData& data);
    bool getLatestCowData(CowData dataArray[], int& count);
    bool getLatestMilkData(MilkData dataArray[], int& count);
    
    // Device registration
    bool registerCowNode(String deviceId);
    bool registerMilkStation(String deviceId);
    int getRegisteredCowNodes(String nodes[], int maxNodes);
    int getRegisteredMilkStations(String stations[], int maxStations);
    
    // Logging
    void logInfo(String message);
    void logError(String message);
    void logWarning(String message);
    
    // Statistics
    unsigned long getTotalRecords();
    unsigned long getSDCardSpace();
    bool exportDataToCSV(String filename);
    
    // Maintenance
    bool clearOldData(int daysOld);
    bool backupDatabase();
    void printStatus();
};

#endif // DATA_STORAGE_H
