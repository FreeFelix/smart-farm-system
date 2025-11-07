// Smart Cow Health Monitoring System - Communication Implementation
// Dual communication: WiFi + LoRa to Device Manager

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LoRa.h>
#include "config.h"
#include "health_data.h"
#include "communication.h"
#include "device_config.h"

bool wifiConnected = false;
unsigned long lastWiFiCheck = 0;

void connectToWiFi() {
  if (!deviceConfig.wifiEnabled || deviceConfig.wifiSSID.length() == 0) {
    Serial.println("WiFi not configured");
    wifiConnected = false;
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

void initializeCommunication() {
  // Initialize LoRa if enabled
  if (deviceConfig.loraEnabled) {
    Serial.println("Initializing LoRa communication...");
    
    LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
    
    if (!LoRa.begin(deviceConfig.loraFrequency)) {
      Serial.println("ERROR: LoRa initialization failed!");
      loraAvailable = false;
    } else {
      LoRa.setTxPower(20);
      LoRa.setSpreadingFactor(deviceConfig.loraSpreadFactor);
      LoRa.setSignalBandwidth(125E3);
      LoRa.setSyncWord(deviceConfig.loraSyncWord);
      Serial.print("LoRa initialized successfully (");
      Serial.print(deviceConfig.loraFrequency / 1E6);
      Serial.println(" MHz)");
      loraAvailable = true;
    }
  } else {
    Serial.println("LoRa disabled in configuration");
    loraAvailable = false;
  }
  
  // Connect to WiFi if enabled
  if (deviceConfig.wifiEnabled) {
    connectToWiFi();
  } else {
    Serial.println("WiFi disabled in configuration");
    wifiConnected = false;
  }
}

void transmitLoRa() {
  if (!loraAvailable) return;
  
  Serial.println("\n>>> Transmitting data via LoRa...");
  
  // Build compact data packet
  String packet = "HEALTH|" + deviceConfig.deviceID + "|";
  packet += String(healthData.temperature, 1) + "|";
  packet += String(healthData.activityLevel, 2) + "|";
  packet += String(healthData.isMoving ? 1 : 0) + "|";
  packet += String(healthData.isDigesting ? 1 : 0) + "|";
  packet += String(healthData.latitude, 4) + "|";
  packet += String(healthData.longitude, 4) + "|";
  packet += String(healthData.altitude, 0) + "|";
  packet += String(healthData.satellites) + "|";
  packet += String(healthData.batteryLevel) + "|";
  packet += String(healthData.tempAlert ? 1 : 0);
  
  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();
  
  healthData.loraPacketsSent++;
  
  Serial.println("LoRa: Data transmitted successfully!");
  Serial.print("Packets sent: ");
  Serial.println(healthData.loraPacketsSent);
}

void transmitViaWiFi() {
  if (!wifiConnected) {
    Serial.println("WiFi not connected, skipping WiFi transmission");
    return;
  }
  
  Serial.println("\n>>> Transmitting data via WiFi...");
  
  HTTPClient http;
  String url = "http://" + deviceConfig.deviceManagerIP + ":" + 
               String(deviceConfig.deviceManagerPort) + "/api/health-data";
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  // Build JSON data
  String jsonData = "{";
  jsonData += "\"deviceID\":\"" + deviceConfig.deviceID + "\",";
  jsonData += "\"farmID\":\"" + deviceConfig.farmID + "\",";
  jsonData += "\"type\":\"COW_HEALTH\",";
  jsonData += "\"timestamp\":" + String(millis()) + ",";
  jsonData += "\"data\":{";
  jsonData += "\"temperature\":" + String(healthData.temperature, 1) + ",";
  jsonData += "\"activityLevel\":" + String(healthData.activityLevel, 2) + ",";
  jsonData += "\"isMoving\":" + String(healthData.isMoving ? "true" : "false") + ",";
  jsonData += "\"isDigesting\":" + String(healthData.isDigesting ? "true" : "false") + ",";
  jsonData += "\"latitude\":" + String(healthData.latitude, 6) + ",";
  jsonData += "\"longitude\":" + String(healthData.longitude, 6) + ",";
  jsonData += "\"altitude\":" + String(healthData.altitude, 1) + ",";
  jsonData += "\"satellites\":" + String(healthData.satellites) + ",";
  jsonData += "\"batteryLevel\":" + String(healthData.batteryLevel) + ",";
  jsonData += "\"batteryVoltage\":" + String(healthData.batteryVoltage, 2) + ",";
  jsonData += "\"solarVoltage\":" + String(healthData.solarVoltage) + ",";
  jsonData += "\"tempAlert\":" + String(healthData.tempAlert ? "true" : "false") + ",";
  jsonData += "\"inactivityAlert\":" + String(healthData.inactivityAlert ? "true" : "false");
  jsonData += "}}";
  
  int httpCode = http.POST(jsonData);
  
  if (httpCode > 0) {
    Serial.printf("WiFi: Data sent successfully, response: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      Serial.println("Server response: " + response);
    }
  } else {
    Serial.printf("WiFi: Failed to send data: %s\n", http.errorToString(httpCode).c_str());
  }
  
  http.end();
}

void transmitHealthData() {
  // Try WiFi first (faster and more reliable when available)
  if (wifiConnected && deviceConfig.wifiEnabled) {
    transmitViaWiFi();
  }
  
  // Always send via LoRa as well for redundancy/gateway routing
  if (loraAvailable && deviceConfig.loraEnabled) {
    // Small delay between WiFi and LoRa to avoid interference
    if (wifiConnected) delay(100);
    transmitLoRa();
  }
  
  // If neither is available, log error
  if (!wifiConnected && !loraAvailable) {
    Serial.println("ERROR: No communication method available!");
  }
}
