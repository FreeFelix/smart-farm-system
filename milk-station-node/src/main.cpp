// Milk Station Node - Main Controller
// Fixed milking station with RFID, milk sensors, LCD display, WiFi & LoRa

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LoRa.h>

#include "config.h"
#include "health_data.h"
#include "device_config.h"
#include "communication.h"
#include "milk_monitor.h"
#include "lcd_display.h"
#include "rfid_reader.h"

// Global variables (healthData and loraAvailable are in health_data.cpp)
bool wifiConnected = false;
unsigned long lastTransmitTime = 0;
unsigned long lastRFIDScan = 0;
unsigned long lastWiFiCheck = 0;
String currentCowID = "";
bool sessionActive = false;

// WiFi connection management
void connectToWiFi() {
    if (!deviceConfig.wifiEnabled || deviceConfig.wifiSSID.length() == 0) {
        Serial.println("WiFi not configured");
        return;
    }
    
    Serial.print("Connecting to WiFi: ");
    Serial.println(deviceConfig.wifiSSID);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(deviceConfig.wifiSSID.c_str(), deviceConfig.wifiPassword.c_str());
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        wifiConnected = true;
        Serial.println("\nWiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        wifiConnected = false;
        Serial.println("\nWiFi connection failed!");
    }
}

// Send data to device manager via WiFi
void sendToDeviceManager(const String &data) {
    if (!wifiConnected) return;
    
    HTTPClient http;
    String url = "http://" + deviceConfig.deviceManagerIP + ":" + String(deviceConfig.deviceManagerPort) + "/api/milk-data";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    String jsonData = "{";
    jsonData += "\"deviceID\":\"" + deviceConfig.deviceID + "\",";
    jsonData += "\"farmID\":\"" + deviceConfig.farmID + "\",";
    jsonData += "\"data\":\"" + data + "\"";
    jsonData += "}";
    
    int httpCode = http.POST(jsonData);
    
    if (httpCode > 0) {
        Serial.printf("Data sent to manager, response: %d\n", httpCode);
    } else {
        Serial.printf("Failed to send data: %s\n", http.errorToString(httpCode).c_str());
    }
    
    http.end();
}

// Process serial commands
void processSerialInput() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        if (input.length() > 0) {
            processConfigCommand(input);
        }
    }
}

void setup() {
    // Initialize Serial
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("\n\n========================================");
    Serial.println("      MILK STATION NODE - v1.0");
    Serial.println("   Smart Dairy Farm Management System");
    Serial.println("========================================\n");
    
    // Initialize device configuration
    initDeviceConfig();
    loadDeviceConfig();
    printDeviceConfig();
    
    Serial.println("Type 'HELP' for configuration commands\n");
    
    // Initialize LEDs
    pinMode(STATUS_LED, OUTPUT);
    pinMode(ALERT_LED, OUTPUT);
    pinMode(SOLENOID_PIN, OUTPUT);
    digitalWrite(STATUS_LED, LOW);
    digitalWrite(ALERT_LED, LOW);
    digitalWrite(SOLENOID_PIN, LOW);
    
    // Initialize I2C for LCD
    Wire.begin(I2C_SDA, I2C_SCL);
    
    // Initialize LCD Display
    Serial.println("Initializing LCD display...");
    setupLCD();
    displayWelcome();
    delay(2000);
    
    // Initialize RFID Reader
    Serial.println("Initializing RFID reader...");
    setupRFID();
    
    // Initialize Milk Monitoring System
    Serial.println("Initializing milk sensors...");
    initializeMilkMonitoring();
    
    // Connect to WiFi if enabled
    if (deviceConfig.wifiEnabled) {
        connectToWiFi();
    }
    
    // Initialize LoRa if enabled
    if (deviceConfig.loraEnabled) {
        Serial.println("Initializing LoRa communication...");
        if (setupLoRa()) {
            loraAvailable = true;
            Serial.println("LoRa ready - " + String(deviceConfig.loraFrequency / 1E6) + " MHz");
            digitalWrite(STATUS_LED, HIGH);
        } else {
            Serial.println("LoRa failed! System will continue without LoRa.");
            digitalWrite(ALERT_LED, HIGH);
            delay(1000);
            digitalWrite(ALERT_LED, LOW);
        }
    }
    
    Serial.println("\n========================================");
    Serial.println("  MILK STATION READY");
    Serial.println("  Device ID: " + deviceConfig.deviceID);
    Serial.println("  Waiting for cow...");
    Serial.println("========================================\n");
    
    displayWaitingForCow();
}

void loop() {
    // Process serial commands
    processSerialInput();
    
    // Check WiFi connection periodically
    if (deviceConfig.wifiEnabled && (millis() - lastWiFiCheck >= 30000)) {
        lastWiFiCheck = millis();
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi disconnected, reconnecting...");
            connectToWiFi();
        }
    }
    
    // Scan for RFID tag
    if (millis() - lastRFIDScan >= RFID_SCAN_INTERVAL) {
        lastRFIDScan = millis();
        
        String scannedID;
        if (readCowTag(scannedID)) {
            if (!sessionActive || scannedID != currentCowID) {
                // New cow detected, start new session
                currentCowID = scannedID;
                sessionActive = true;
                resetMilkSession(); healthData.isMilkProducing = true;
                
                Serial.print("Starting milking session for cow: ");
                Serial.println(currentCowID);
                
                // Open solenoid valve
                digitalWrite(SOLENOID_PIN, HIGH);
            }
        }
    }
    
    // Update milk monitoring if session active
    if (sessionActive) {
        readMilkProduction(); checkMilkQuality();
        
        // Update LCD with current session data
        displayMilkingSession(currentCowID, healthData.milkVolume, healthData.milkWeight);
        displayMilkQuality(healthData);
        
        // Check for session timeout or completion
        if (false || !isCowPresent()) {
            // End session
            Serial.println("Milking session complete");
            
            // Close solenoid valve
            digitalWrite(SOLENOID_PIN, LOW);
            
            // Display summary
            displaySessionComplete(healthData.milkVolume, healthData.milkWeight);
            
            // Prepare data string
            String dataString = "MILK|" + currentCowID + "|";
            dataString += String(healthData.milkVolume, 2) + "|";
            dataString += String(healthData.milkWeight, 2) + "|";
            dataString += String(healthData.milkPH, 2) + "|";
            dataString += String(healthData.milkTemperature, 1) + "|";
            dataString += String(healthData.milkConductivity, 2) + "|";
            dataString += String((healthData.milkPHAlert || healthData.mastitisAlert ? 0.3 : 0.9), 2);
            
            // Send via WiFi to device manager
            if (wifiConnected) {
                sendToDeviceManager(dataString);
            }
            
            // Send via LoRa
            if (loraAvailable) {
                transmitMilkData(currentCowID, healthData);
            }
            
            delay(5000);
            
            // Reset for next cow
            sessionActive = false;
            currentCowID = "";
            displayWaitingForCow();
        }
        
        // Check for quality alerts
        if ((healthData.milkPHAlert || healthData.mastitisAlert ? 0.3 : 0.9) < 0.5) {
            digitalWrite(ALERT_LED, HIGH);
            displayAlert("MILK QUALITY LOW!");
            Serial.println("ALERT: Low milk quality detected!");
        } else {
            digitalWrite(ALERT_LED, LOW);
        }
    }
    
    // Periodic LoRa transmission (if session active)
    if (sessionActive && loraAvailable && (millis() - lastTransmitTime >= LORA_INTERVAL)) {
        transmitMilkData(currentCowID, healthData);
        lastTransmitTime = millis();
    }
    
    // Status LED heartbeat
    if (millis() % 2000 < 100) {
        digitalWrite(STATUS_LED, HIGH);
    } else if (millis() % 2000 > 100 && millis() % 2000 < 200) {
        digitalWrite(STATUS_LED, LOW);
    }
    
    delay(100);
}
