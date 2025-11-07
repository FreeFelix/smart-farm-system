#include <Arduino.h>
#include "config.h"
#include "device_data.h"
#include "wifi_manager.h"
#include "mqtt_client.h"
#include "lora_comm.h"
#include "gateway_display.h"
#include "data_storage.h"

// ==================== FUNCTION DECLARATIONS ====================
void printConfiguration();
void printGatewayStatus();
void printCowData(const CowData& data);
void printMilkData(const MilkData& data);

// ==================== GLOBAL OBJECTS ====================
WiFiManager wifiMgr;
MQTTClient mqttClient;
LoRaComm loraComm;
GatewayDisplay gatewayDisplay;
DataStorage dataStorage;

// ==================== DATA STORAGE ====================
CowData cowDataArray[HISTORY_SIZE];
MilkData milkDataArray[HISTORY_SIZE];
int cowDataCount = 0;
int milkDataCount = 0;
GatewayStatus gatewayStatus;

// ==================== TIMING VARIABLES ====================
unsigned long lastDataSend = 0;
unsigned long lastStatusUpdate = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long startTime = 0;

// ==================== SETUP ====================
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n");
    Serial.println("========================================");
    Serial.println("   SMART FARM GATEWAY v2.0 (ESP32)");
    Serial.println("========================================");
    Serial.println();
    
    startTime = millis();
    
    // Initialize Status LEDs with test
    Serial.println("Initializing LEDs...");
    pinMode(STATUS_LED, OUTPUT);
    pinMode(ERROR_LED, OUTPUT);
    
    // LED Test - Flash both LEDs to verify they work
    for (int i = 0; i < 3; i++) {
        digitalWrite(STATUS_LED, HIGH);
        digitalWrite(ERROR_LED, HIGH);
        Serial.println("LEDs ON");
        delay(200);
        digitalWrite(STATUS_LED, LOW);
        digitalWrite(ERROR_LED, LOW);
        Serial.println("LEDs OFF");
        delay(200);
    }
    
    digitalWrite(STATUS_LED, LOW);
    digitalWrite(ERROR_LED, HIGH);  // Red during init
    Serial.println("✓ LEDs initialized and tested!");
    
    // Initialize Display (FIRST - for visual feedback)
    Serial.println("Step 1/5: Initializing TFT Display...");
    Serial.printf("TFT Pins - CS:%d, DC:%d, RST:%d, BL:%d\n", TFT_CS, TFT_DC, TFT_RST, TFT_BL);
    
    if (gatewayDisplay.begin()) {
        gatewayDisplay.displayMessage("INIT", "Step 1/5: Display OK", TFT_GREEN);
        delay(500);  // Reduced delay
    } else {
        Serial.println("Warning: Display not available");
    }
    
    // Initialize SD Card Storage
    Serial.println("Step 2/5: Initializing SD Card...");
    gatewayDisplay.displayMessage("INIT", "Step 2/5: SD Card...", TFT_YELLOW);
    if (dataStorage.begin()) {
        gatewayDisplay.displayMessage("INIT", "Step 2/5: SD Card OK", TFT_GREEN);
    } else {
        Serial.println("Warning: SD Card not available - data will not be saved");
        gatewayDisplay.displayMessage("WARNING", "SD Card Failed!", TFT_ORANGE);
        delay(500);  // Reduced delay
    }
    delay(200);  // Reduced delay
    
    // Initialize LoRa Communication
    Serial.println("Step 3/5: Initializing LoRa...");
    gatewayDisplay.displayMessage("INIT", "Step 3/5: LoRa...", TFT_YELLOW);
    if (loraComm.begin()) {
        gatewayStatus.loraActive = true;
        gatewayDisplay.displayMessage("INIT", "Step 3/5: LoRa OK", TFT_GREEN);
    } else {
        Serial.println("Warning: LoRa not available");
        gatewayStatus.loraActive = false;
        gatewayDisplay.displayMessage("WARNING", "LoRa Failed!", TFT_ORANGE);
        delay(500);  // Reduced delay
    }
    delay(200);  // Reduced delay
    
    // Initialize WiFi
    Serial.println("Step 4/5: Connecting to WiFi...");
    gatewayDisplay.displayMessage("INIT", "Step 4/5: WiFi...", TFT_YELLOW);
    if (wifiMgr.begin()) {
        gatewayStatus.wifiConnected = true;
        gatewayDisplay.displayMessage("INIT", "Step 4/5: WiFi/AP OK", TFT_GREEN);
        digitalWrite(STATUS_LED, HIGH);  // Green when WiFi connected
    } else {
        Serial.println("Warning: WiFi/AP setup failed - continuing anyway");
        gatewayStatus.wifiConnected = false;
        gatewayDisplay.displayMessage("WARNING", "WiFi Failed!", TFT_ORANGE);
        delay(500);  // Reduced delay
    }
    delay(200);  // Reduced delay
    
    // Initialize MQTT
    Serial.println("Step 5/5: Connecting to MQTT Broker...");
    gatewayDisplay.displayMessage("INIT", "Step 5/5: MQTT...", TFT_YELLOW);
    if (wifiMgr.isConnected() && mqttClient.begin()) {
        gatewayStatus.mqttConnected = true;
        gatewayDisplay.displayMessage("INIT", "Step 5/5: MQTT OK", TFT_GREEN);
        digitalWrite(ERROR_LED, LOW);  // Turn off error LED
    } else {
        Serial.println("Warning: MQTT connection failed - local operation only");
        gatewayStatus.mqttConnected = false;
        gatewayDisplay.displayMessage("WARNING", "MQTT Failed!", TFT_ORANGE);
        delay(500);  // Reduced delay
    }
    delay(200);  // Reduced delay
    
    // System Ready
    Serial.println("\n========================================");
    Serial.println("   GATEWAY SYSTEM READY!");
    Serial.println("========================================");
    Serial.println();
    Serial.println("Hardware: ESP32 + ILI9341 TFT + SD Card");
    Serial.printf("Display: %s\n", gatewayDisplay.isInitialized() ? "320x240 TFT Color" : "Not Available");
    Serial.printf("Storage: %s\n", dataStorage.isSDAvailable() ? "SD Card Ready" : "No SD Card");
    Serial.printf("LoRa: %s\n", gatewayStatus.loraActive ? "915MHz Ready" : "Not Available");
    Serial.printf("WiFi: %s\n", gatewayStatus.wifiConnected ? "Connected" : "AP Mode");
    Serial.printf("MQTT: %s\n", gatewayStatus.mqttConnected ? "Connected" : "Not Available");
    Serial.println("Capacity: 30 Cow Nodes + 20 Milk Stations");
    Serial.println();
    Serial.println("Waiting for data from devices...");
    Serial.println();
    
    gatewayDisplay.displayMessage("READY", "System Online!", TFT_GREEN);
    delay(2000);
    
    // Print configuration
    printConfiguration();
    dataStorage.printStatus();
}

// ==================== MAIN LOOP ====================
void loop() {
    unsigned long currentMillis = millis();
    
    // Update uptime
    gatewayStatus.uptime = currentMillis - startTime;
    gatewayStatus.freeHeap = ESP.getFreeHeap();
    
    // Handle WiFi Connection
    wifiMgr.handleConnection();
    gatewayStatus.wifiConnected = wifiMgr.isConnected();
    if (gatewayStatus.wifiConnected) {
        gatewayStatus.rssi = wifiMgr.getRSSI();
    }
    
    // Handle MQTT Connection
    if (gatewayStatus.wifiConnected) {
        mqttClient.handleConnection();
        mqttClient.loop();
        gatewayStatus.mqttConnected = mqttClient.isConnected();
    } else {
        gatewayStatus.mqttConnected = false;
    }
    
    // Update status LED (blink when connected)
    if (gatewayStatus.wifiConnected && gatewayStatus.mqttConnected) {
        digitalWrite(STATUS_LED, (currentMillis / 500) % 2);  // Blink 1Hz
        digitalWrite(ERROR_LED, LOW);
    } else {
        digitalWrite(STATUS_LED, LOW);
        digitalWrite(ERROR_LED, (currentMillis / 200) % 2);  // Fast blink error
    }
    
    // Receive data from LoRa devices
    if (gatewayStatus.loraActive) {
        // Check for Cow Node data
        CowData newCowData;
        if (loraComm.receiveCowData(newCowData)) {
            // Save to storage
            dataStorage.saveCowData(newCowData);
            dataStorage.registerCowNode(newCowData.deviceId);
            
            // Add to display array
            if (cowDataCount < HISTORY_SIZE) {
                cowDataArray[cowDataCount++] = newCowData;
            }
            
            gatewayStatus.lastDataReceived = currentMillis;
            gatewayStatus.cowNodesActive = cowDataCount;
            
            Serial.println("\n--- Cow Data Received ---");
            printCowData(newCowData);
            
            // Send to cloud immediately when new data arrives
            if (gatewayStatus.mqttConnected) {
                mqttClient.publishCowData(newCowData);
            }
        }
        
        // Check for Milk Station data
        MilkData newMilkData;
        if (loraComm.receiveMilkData(newMilkData)) {
            // Save to storage
            dataStorage.saveMilkData(newMilkData);
            dataStorage.registerMilkStation(newMilkData.deviceId);
            
            // Add to display array
            if (milkDataCount < HISTORY_SIZE) {
                milkDataArray[milkDataCount++] = newMilkData;
            }
            
            gatewayStatus.lastDataReceived = currentMillis;
            gatewayStatus.milkStationsActive = milkDataCount;
            
            Serial.println("\n--- Milk Data Received ---");
            printMilkData(newMilkData);
            
            // Send to cloud immediately when new data arrives
            if (gatewayStatus.mqttConnected) {
                mqttClient.publishMilkData(newMilkData);
            }
        }
    }
    
    // Periodic data sending (backup in case immediate send fails)
    if (currentMillis - lastDataSend >= DATA_SEND_INTERVAL) {
        lastDataSend = currentMillis;
        
        if (gatewayStatus.mqttConnected) {
            // Resend latest valid data
            for (int i = 0; i < cowDataCount; i++) {
                if (cowDataArray[i].isValid) {
                    mqttClient.publishCowData(cowDataArray[i]);
                }
            }
            for (int i = 0; i < milkDataCount; i++) {
                if (milkDataArray[i].isValid) {
                    mqttClient.publishMilkData(milkDataArray[i]);
                }
            }
            
            // Send gateway status
            mqttClient.publishGatewayStatus(gatewayStatus);
            
            // Save gateway status to SD
            if (dataStorage.isSDAvailable()) {
                dataStorage.saveGatewayStatus(gatewayStatus);
            }
        }
    }
    
    // Update display
    if (currentMillis - lastDisplayUpdate >= STATUS_UPDATE_INTERVAL) {
        lastDisplayUpdate = currentMillis;
        gatewayDisplay.update(gatewayStatus, cowDataArray, cowDataCount,
                            milkDataArray, milkDataCount);
    }
    
    // Print status periodically
    if (currentMillis - lastStatusUpdate >= 60000) {  // Every minute
        lastStatusUpdate = currentMillis;
        printGatewayStatus();
    }
    
    // Demo mode: Generate fake data if no LoRa (for simulation)
    static unsigned long lastDemoData = 0;
    if (!gatewayStatus.loraActive && currentMillis - lastDemoData >= 15000) {  // Every 15 seconds
        lastDemoData = currentMillis;
        
        // Generate demo cow data
        if (cowDataCount < 3) {  // Limit to 3 demo cows
            CowData demoCow;
            demoCow.deviceId = "DEMO_COW_" + String(cowDataCount + 1);
            demoCow.temperature = 38.5 + (random(-10, 10) / 10.0);  // 38.0-39.0°C
            demoCow.heartRate = 60 + random(-10, 20);  // 50-80 bpm
            demoCow.activity = random(20, 90);  // 20-90%
            demoCow.batteryLevel = 85 - (cowDataCount * 10);  // Decreasing battery
            demoCow.gpsFixed = true;
            demoCow.latitude = -1.9441 + (random(-100, 100) / 10000.0);  // Kigali area
            demoCow.longitude = 30.0619 + (random(-100, 100) / 10000.0);
            demoCow.healthStatus = (demoCow.temperature > 39.0) ? "Warning" : "Healthy";
            demoCow.isValid = true;
            demoCow.timestamp = currentMillis;
            
            cowDataArray[cowDataCount++] = demoCow;
            gatewayStatus.cowNodesActive = cowDataCount;
            gatewayStatus.lastDataReceived = currentMillis;
            
            Serial.println("\n--- Demo Cow Data Generated ---");
            printCowData(demoCow);
        }
        
        // Generate demo milk data
        if (milkDataCount < 2) {  // Limit to 2 demo stations
            MilkData demoMilk;
            demoMilk.deviceId = "DEMO_MILK_" + String(milkDataCount + 1);
            demoMilk.cowId = "DEMO_COW_" + String(milkDataCount + 1);
            demoMilk.temperature = 4.0 + (random(-5, 5) / 10.0);  // 3.5-4.5°C
            demoMilk.ph = 6.6 + (random(-2, 2) / 10.0);  // 6.4-6.8
            demoMilk.volume = 2.5 + (random(0, 20) / 10.0);  // 2.5-4.5 L
            demoMilk.conductivity = 4.5 + (random(-5, 5) / 10.0);  // 4.0-5.0
            demoMilk.flowRate = 1.2 + (random(0, 8) / 10.0);  // 1.2-2.0 L/min
            demoMilk.quality = (demoMilk.ph < 6.5) ? "FAIR" : "GOOD";
            demoMilk.sessionActive = true;
            demoMilk.sessionStart = currentMillis - random(60000, 300000);  // Started 1-5 min ago
            demoMilk.isValid = true;
            demoMilk.timestamp = currentMillis;
            
            milkDataArray[milkDataCount++] = demoMilk;
            gatewayStatus.milkStationsActive = milkDataCount;
            gatewayStatus.lastDataReceived = currentMillis;
            
            Serial.println("\n--- Demo Milk Data Generated ---");
            printMilkData(demoMilk);
        }
    }
    
    // Small delay to prevent watchdog issues
    delay(10);
}

// ==================== HELPER FUNCTIONS ====================

void printConfiguration() {
    Serial.println("\n=== GATEWAY CONFIGURATION ===");
    Serial.print("WiFi SSID: ");
    Serial.println(WIFI_SSID);
    Serial.print("MQTT Broker: ");
    Serial.println(MQTT_BROKER);
    Serial.print("MQTT Port: ");
    Serial.println(MQTT_PORT);
    Serial.print("LoRa Frequency: ");
    Serial.print(LORA_FREQUENCY / 1E6);
    Serial.println(" MHz");
    Serial.println("Topics:");
    Serial.print("  Cow Data: ");
    Serial.println(TOPIC_COW_DATA);
    Serial.print("  Milk Data: ");
    Serial.println(TOPIC_MILK_DATA);
    Serial.print("  Gateway Status: ");
    Serial.println(TOPIC_GATEWAY_STATUS);
    Serial.print("  Commands: ");
    Serial.println(TOPIC_COMMANDS);
    Serial.println("=============================\n");
}

void printGatewayStatus() {
    Serial.println("\n╔══════════════════════════════════════╗");
    Serial.println("║      GATEWAY STATUS REPORT           ║");
    Serial.println("╠══════════════════════════════════════╣");
    Serial.print("║ Uptime: ");
    Serial.print(gatewayStatus.uptime / 60000);
    Serial.println(" minutes");
    Serial.print("║ WiFi: ");
    Serial.print(gatewayStatus.wifiConnected ? "✓ Connected" : "✗ Disconnected");
    if (gatewayStatus.wifiConnected) {
        Serial.print(" (");
        Serial.print(gatewayStatus.rssi);
        Serial.print(" dBm)");
    }
    Serial.println();
    Serial.print("║ MQTT: ");
    Serial.println(gatewayStatus.mqttConnected ? "✓ Connected" : "✗ Disconnected");
    Serial.print("║ LoRa: ");
    Serial.println(gatewayStatus.loraActive ? "✓ Active" : "✗ Inactive");
    Serial.print("║ Cow Nodes: ");
    Serial.println(gatewayStatus.cowNodesActive);
    Serial.print("║ Milk Stations: ");
    Serial.println(gatewayStatus.milkStationsActive);
    Serial.print("║ Free Heap: ");
    Serial.print(gatewayStatus.freeHeap);
    Serial.println(" bytes");
    Serial.print("║ Last Data: ");
    if (gatewayStatus.lastDataReceived > 0) {
        Serial.print((millis() - gatewayStatus.lastDataReceived) / 1000);
        Serial.println(" sec ago");
    } else {
        Serial.println("Never");
    }
    Serial.println("╚══════════════════════════════════════╝\n");
}

void printCowData(const CowData& data) {
    Serial.print("Device: ");
    Serial.println(data.deviceId);
    Serial.print("Temperature: ");
    Serial.print(data.temperature);
    Serial.println(" °C");
    Serial.print("Heart Rate: ");
    Serial.print(data.heartRate);
    Serial.println(" bpm");
    Serial.print("Activity: ");
    Serial.print(data.activity);
    Serial.println(" %");
    Serial.print("Battery: ");
    Serial.print(data.batteryLevel);
    Serial.println(" %");
    Serial.print("GPS: ");
    Serial.println(data.gpsFixed ? "Fixed" : "No Fix");
    if (data.gpsFixed) {
        Serial.print("Location: ");
        Serial.print(data.latitude, 6);
        Serial.print(", ");
        Serial.println(data.longitude, 6);
    }
    Serial.print("Health Status: ");
    Serial.println(data.healthStatus);
    Serial.println("-------------------------");
}

void printMilkData(const MilkData& data) {
    Serial.print("Station: ");
    Serial.println(data.deviceId);
    Serial.print("Cow ID: ");
    Serial.println(data.cowId);
    Serial.print("Temperature: ");
    Serial.print(data.temperature);
    Serial.println(" °C");
    Serial.print("pH: ");
    Serial.println(data.ph);
    Serial.print("Conductivity: ");
    Serial.print(data.conductivity);
    Serial.println(" mS/cm");
    Serial.print("Flow Rate: ");
    Serial.print(data.flowRate);
    Serial.println(" L/min");
    Serial.print("Volume: ");
    Serial.print(data.volume);
    Serial.println(" L");
    Serial.print("Quality: ");
    Serial.println(data.quality);
    Serial.print("Session Active: ");
    Serial.println(data.sessionActive ? "Yes" : "No");
    Serial.println("-------------------------");
}
