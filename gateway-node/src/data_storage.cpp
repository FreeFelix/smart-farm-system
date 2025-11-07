#include "data_storage.h"

DataStorage::DataStorage() : sdInitialized(false), dbInitialized(false),
                             cowDataIndex(0), milkDataIndex(0),
                             numCowNodes(0), numMilkStations(0),
                             lastSaveTime(0) {
    // Initialize arrays
    for (int i = 0; i < MAX_COW_NODES; i++) {
        registeredCowNodes[i] = "";
    }
    for (int i = 0; i < MAX_MILK_STATIONS; i++) {
        registeredMilkStations[i] = "";
    }
}

bool DataStorage::begin() {
    Serial.println("Initializing SD Card Storage...");
    
    // Initialize SPI for SD card (shared with TFT)
    // Hardware SPI pins: SCK=18, MISO=19, MOSI=23
    SPI.begin(18, 19, 23, SD_CS);  // SCK, MISO, MOSI, CS
    
    if (!SD.begin(SD_CS, SPI, 80000000)) {  // Try 80MHz SPI speed
        Serial.println("✗ SD Card initialization failed!");
        Serial.println("   Trying lower speed...");
        
        // Try slower speed
        if (!SD.begin(SD_CS, SPI, 4000000)) {  // Try 4MHz
            Serial.println("✗ SD Card still failed - continuing without storage");
            sdInitialized = false;
            return false;
        }
    }
    
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("✗ No SD card attached!");
        sdInitialized = false;
        return false;
    }
    
    Serial.print("✓ SD Card initialized! Type: ");
    if (cardType == CARD_MMC) Serial.println("MMC");
    else if (cardType == CARD_SD) Serial.println("SD");
    else if (cardType == CARD_SDHC) Serial.println("SDHC");
    
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.print("SD Card Size: ");
    Serial.print(cardSize);
    Serial.println(" MB");
    
    sdInitialized = true;
    
    // Create directory structure
    if (!SD.exists("/data")) SD.mkdir("/data");
    if (!SD.exists("/data/cow")) SD.mkdir("/data/cow");
    if (!SD.exists("/data/milk")) SD.mkdir("/data/milk");
    if (!SD.exists("/logs")) SD.mkdir("/logs");
    
    logInfo("Gateway storage system initialized");
    
    return true;
}

bool DataStorage::isSDAvailable() {
    return sdInitialized;
}

bool DataStorage::saveCowData(const CowData& data) {
    if (!sdInitialized || !data.isValid) return false;
    
    // Save to ring buffer (in-memory)
    cowDataHistory[cowDataIndex] = data;
    cowDataIndex = (cowDataIndex + 1) % HISTORY_SIZE;
    
    // Save to SD card periodically
    if (millis() - lastSaveTime < DATA_SAVE_INTERVAL) {
        return true;  // Skip SD write to reduce wear
    }
    
    // Create filename: /data/cow/COW001_20251107.json
    String filename = "/data/cow/" + data.deviceId + "_";
    filename += String(millis() / 86400000);  // Day number
    filename += ".json";
    
    // Append to daily file
    File file = SD.open(filename, FILE_APPEND);
    if (!file) {
        logError("Failed to open cow data file: " + filename);
        return false;
    }
    
    StaticJsonDocument<512> doc;
    doc["deviceId"] = data.deviceId;
    doc["timestamp"] = data.timestamp;
    doc["temperature"] = data.temperature;
    doc["heartRate"] = data.heartRate;
    doc["activity"] = data.activity;
    doc["battery"] = data.batteryLevel;
    doc["gpsFixed"] = data.gpsFixed;
    doc["latitude"] = data.latitude;
    doc["longitude"] = data.longitude;
    doc["healthStatus"] = data.healthStatus;
    
    serializeJson(doc, file);
    file.println();
    file.close();
    
    lastSaveTime = millis();
    return true;
}

bool DataStorage::saveMilkData(const MilkData& data) {
    if (!sdInitialized || !data.isValid) return false;
    
    // Save to ring buffer (in-memory)
    milkDataHistory[milkDataIndex] = data;
    milkDataIndex = (milkDataIndex + 1) % HISTORY_SIZE;
    
    // Save to SD card periodically
    if (millis() - lastSaveTime < DATA_SAVE_INTERVAL) {
        return true;
    }
    
    String filename = "/data/milk/" + data.deviceId + "_";
    filename += String(millis() / 86400000);
    filename += ".json";
    
    File file = SD.open(filename, FILE_APPEND);
    if (!file) {
        logError("Failed to open milk data file: " + filename);
        return false;
    }
    
    StaticJsonDocument<512> doc;
    doc["deviceId"] = data.deviceId;
    doc["cowId"] = data.cowId;
    doc["timestamp"] = data.timestamp;
    doc["temperature"] = data.temperature;
    doc["ph"] = data.ph;
    doc["conductivity"] = data.conductivity;
    doc["flowRate"] = data.flowRate;
    doc["volume"] = data.volume;
    doc["quality"] = data.quality;
    doc["sessionActive"] = data.sessionActive;
    
    serializeJson(doc, file);
    file.println();
    file.close();
    
    lastSaveTime = millis();
    return true;
}

bool DataStorage::saveGatewayStatus(const GatewayStatus& status) {
    if (!sdInitialized) return false;
    
    String filename = "/logs/gateway_status.json";
    
    File file = SD.open(filename, FILE_WRITE);
    if (!file) return false;
    
    StaticJsonDocument<512> doc;
    doc["timestamp"] = millis();
    doc["wifiConnected"] = status.wifiConnected;
    doc["mqttConnected"] = status.mqttConnected;
    doc["loraActive"] = status.loraActive;
    doc["cowNodesActive"] = status.cowNodesActive;
    doc["milkStationsActive"] = status.milkStationsActive;
    doc["uptime"] = status.uptime;
    doc["freeHeap"] = status.freeHeap;
    doc["rssi"] = status.rssi;
    
    serializeJson(doc, file);
    file.close();
    
    return true;
}

bool DataStorage::getCowData(String deviceId, CowData& data) {
    // Search in ring buffer (most recent data)
    for (int i = 0; i < HISTORY_SIZE; i++) {
        if (cowDataHistory[i].deviceId == deviceId && cowDataHistory[i].isValid) {
            data = cowDataHistory[i];
            return true;
        }
    }
    return false;
}

bool DataStorage::getMilkData(String deviceId, MilkData& data) {
    for (int i = 0; i < HISTORY_SIZE; i++) {
        if (milkDataHistory[i].deviceId == deviceId && milkDataHistory[i].isValid) {
            data = milkDataHistory[i];
            return true;
        }
    }
    return false;
}

bool DataStorage::getLatestCowData(CowData dataArray[], int& count) {
    count = 0;
    for (int i = 0; i < HISTORY_SIZE && count < HISTORY_SIZE; i++) {
        if (cowDataHistory[i].isValid) {
            dataArray[count++] = cowDataHistory[i];
        }
    }
    return count > 0;
}

bool DataStorage::getLatestMilkData(MilkData dataArray[], int& count) {
    count = 0;
    for (int i = 0; i < HISTORY_SIZE && count < HISTORY_SIZE; i++) {
        if (milkDataHistory[i].isValid) {
            dataArray[count++] = milkDataHistory[i];
        }
    }
    return count > 0;
}

bool DataStorage::registerCowNode(String deviceId) {
    // Check if already registered
    for (int i = 0; i < numCowNodes; i++) {
        if (registeredCowNodes[i] == deviceId) {
            return true;  // Already registered
        }
    }
    
    // Add new node
    if (numCowNodes < MAX_COW_NODES) {
        registeredCowNodes[numCowNodes++] = deviceId;
        logInfo("Registered new cow node: " + deviceId);
        return true;
    }
    
    logWarning("Max cow nodes limit reached!");
    return false;
}

bool DataStorage::registerMilkStation(String deviceId) {
    for (int i = 0; i < numMilkStations; i++) {
        if (registeredMilkStations[i] == deviceId) {
            return true;
        }
    }
    
    if (numMilkStations < MAX_MILK_STATIONS) {
        registeredMilkStations[numMilkStations++] = deviceId;
        logInfo("Registered new milk station: " + deviceId);
        return true;
    }
    
    logWarning("Max milk stations limit reached!");
    return false;
}

int DataStorage::getRegisteredCowNodes(String nodes[], int maxNodes) {
    int count = min(numCowNodes, maxNodes);
    for (int i = 0; i < count; i++) {
        nodes[i] = registeredCowNodes[i];
    }
    return count;
}

int DataStorage::getRegisteredMilkStations(String stations[], int maxStations) {
    int count = min(numMilkStations, maxStations);
    for (int i = 0; i < count; i++) {
        stations[i] = registeredMilkStations[i];
    }
    return count;
}

void DataStorage::logInfo(String message) {
    Serial.print("[INFO] ");
    Serial.println(message);
    
    if (!sdInitialized) return;
    
    String filename = "/logs/system_";
    filename += String(millis() / 86400000);
    filename += ".log";
    
    File file = SD.open(filename, FILE_APPEND);
    if (file) {
        file.print("[");
        file.print(millis());
        file.print("] [INFO] ");
        file.println(message);
        file.close();
    }
}

void DataStorage::logError(String message) {
    Serial.print("[ERROR] ");
    Serial.println(message);
    
    if (!sdInitialized) return;
    
    String filename = "/logs/system_";
    filename += String(millis() / 86400000);
    filename += ".log";
    
    File file = SD.open(filename, FILE_APPEND);
    if (file) {
        file.print("[");
        file.print(millis());
        file.print("] [ERROR] ");
        file.println(message);
        file.close();
    }
}

void DataStorage::logWarning(String message) {
    Serial.print("[WARNING] ");
    Serial.println(message);
    
    if (!sdInitialized) return;
    
    String filename = "/logs/system_";
    filename += String(millis() / 86400000);
    filename += ".log";
    
    File file = SD.open(filename, FILE_APPEND);
    if (file) {
        file.print("[");
        file.print(millis());
        file.print("] [WARNING] ");
        file.println(message);
        file.close();
    }
}

unsigned long DataStorage::getTotalRecords() {
    // Count files in data directories
    // Simplified implementation
    return (cowDataIndex + milkDataIndex);
}

unsigned long DataStorage::getSDCardSpace() {
    if (!sdInitialized) return 0;
    return SD.totalBytes() - SD.usedBytes();
}

bool DataStorage::exportDataToCSV(String filename) {
    if (!sdInitialized) return false;
    
    logInfo("Exporting data to CSV: " + filename);
    
    // Create CSV file
    File csvFile = SD.open(filename, FILE_WRITE);
    if (!csvFile) {
        logError("Failed to create CSV file");
        return false;
    }
    
    // Write cow data header
    csvFile.println("DeviceID,Timestamp,Temperature,HeartRate,Activity,Battery,Latitude,Longitude,HealthStatus");
    
    for (int i = 0; i < HISTORY_SIZE; i++) {
        if (cowDataHistory[i].isValid) {
            csvFile.print(cowDataHistory[i].deviceId);
            csvFile.print(",");
            csvFile.print(cowDataHistory[i].timestamp);
            csvFile.print(",");
            csvFile.print(cowDataHistory[i].temperature);
            csvFile.print(",");
            csvFile.print(cowDataHistory[i].heartRate);
            csvFile.print(",");
            csvFile.print(cowDataHistory[i].activity);
            csvFile.print(",");
            csvFile.print(cowDataHistory[i].batteryLevel);
            csvFile.print(",");
            csvFile.print(cowDataHistory[i].latitude, 6);
            csvFile.print(",");
            csvFile.print(cowDataHistory[i].longitude, 6);
            csvFile.print(",");
            csvFile.println(cowDataHistory[i].healthStatus);
        }
    }
    
    csvFile.close();
    logInfo("CSV export completed");
    return true;
}

bool DataStorage::clearOldData(int daysOld) {
    if (!sdInitialized) return false;
    
    logInfo("Clearing data older than " + String(daysOld) + " days");
    
    // Simple implementation - delete files older than threshold
    // In production, use proper timestamp comparison
    
    return true;
}

bool DataStorage::backupDatabase() {
    if (!sdInitialized) return false;
    
    logInfo("Creating database backup...");
    
    // Copy data directory to backup directory
    if (!SD.exists("/backup")) SD.mkdir("/backup");
    
    // Implementation would copy files to backup directory
    
    return true;
}

void DataStorage::printStatus() {
    Serial.println("\n=== Storage Status ===");
    Serial.print("SD Card: ");
    Serial.println(sdInitialized ? "OK" : "NOT AVAILABLE");
    
    if (sdInitialized) {
        Serial.print("Total Space: ");
        Serial.print(SD.totalBytes() / (1024 * 1024));
        Serial.println(" MB");
        Serial.print("Used Space: ");
        Serial.print(SD.usedBytes() / (1024 * 1024));
        Serial.println(" MB");
        Serial.print("Free Space: ");
        Serial.print(getSDCardSpace() / (1024 * 1024));
        Serial.println(" MB");
    }
    
    Serial.print("Registered Cow Nodes: ");
    Serial.println(numCowNodes);
    Serial.print("Registered Milk Stations: ");
    Serial.println(numMilkStations);
    Serial.print("Cow Data Records: ");
    Serial.println(cowDataIndex);
    Serial.print("Milk Data Records: ");
    Serial.println(milkDataIndex);
    Serial.println("=====================\n");
}
