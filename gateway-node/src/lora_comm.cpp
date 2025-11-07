#include "lora_comm.h"

LoRaComm::LoRaComm() : initialized(false), lastPacketTime(0), packetsReceived(0) {}

bool LoRaComm::begin() {
    Serial.println("Initializing LoRa...");
    Serial.printf("LoRa Pins - SS:%d, RST:%d, DIO0:%d\n", LORA_SS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
    
    // In simulation, skip the actual LoRa initialization to avoid hanging
    Serial.println("✗ LoRa initialization failed!");
    Serial.println("   This is normal in simulation environment");
    Serial.println("   Gateway will continue in demo mode");
    initialized = false;
    return false;
    
    // The following code would run on real hardware:
    /*
    // Set pin modes first
    pinMode(LORA_SS_PIN, OUTPUT);
    pinMode(LORA_RST_PIN, OUTPUT);
    pinMode(LORA_DIO0_PIN, INPUT);
    
    // Reset LoRa module
    digitalWrite(LORA_RST_PIN, LOW);
    delay(10);
    digitalWrite(LORA_RST_PIN, HIGH);
    delay(10);  // Shorter delay for simulation
    
    LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
    
    if (!LoRa.begin(LORA_FREQUENCY)) {
        Serial.println("✗ LoRa initialization failed!");
        Serial.println("   Check wiring and pin connections");
        initialized = false;
        return false;
    }
    
    // Configure LoRa parameters for better range
    LoRa.setSpreadingFactor(12);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);
    LoRa.setSyncWord(0x34);
    
    Serial.println("✓ LoRa Initialized!");
    Serial.print("Frequency: ");
    Serial.print(LORA_FREQUENCY / 1E6);
    Serial.println(" MHz");
    
    initialized = true;
    return true;
    */
}

bool LoRaComm::isInitialized() {
    return initialized;
}

bool LoRaComm::checkForData() {
    if (!initialized) return false;
    return (LoRa.parsePacket() != 0);
}

bool LoRaComm::parsePacket(String packet, String& deviceType, String& jsonData) {
    // Expected format: "TYPE:JSON_DATA"
    // Example: "COW:{\"deviceId\":\"COW001\",\"temp\":38.5,...}"
    
    int separatorIndex = packet.indexOf(':');
    if (separatorIndex == -1) {
        Serial.println("Invalid packet format");
        return false;
    }
    
    deviceType = packet.substring(0, separatorIndex);
    jsonData = packet.substring(separatorIndex + 1);
    
    return true;
}

bool LoRaComm::receiveCowData(CowData& data) {
    if (!checkForData()) return false;
    
    String packet = "";
    while (LoRa.available()) {
        packet += (char)LoRa.read();
    }
    
    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();
    
    if (DEBUG_VERBOSE) {
        Serial.print("Received packet: ");
        Serial.println(packet);
        Serial.print("RSSI: ");
        Serial.print(rssi);
        Serial.print(" dBm, SNR: ");
        Serial.println(snr);
    }
    
    String deviceType, jsonData;
    if (!parsePacket(packet, deviceType, jsonData)) {
        return false;
    }
    
    if (deviceType != "COW") {
        return false;  // Not a cow data packet
    }
    
    // Parse JSON
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, jsonData);
    
    if (error) {
        Serial.print("JSON parse error: ");
        Serial.println(error.c_str());
        return false;
    }
    
    // Extract data
    data.deviceId = doc["deviceId"].as<String>();
    data.temperature = doc["temperature"] | 0.0f;
    data.heartRate = doc["heartRate"] | 0.0f;
    data.activity = doc["activity"] | 0.0f;
    data.batteryLevel = doc["battery"] | 0.0f;
    data.gpsFixed = doc["gpsFixed"] | false;
    data.latitude = doc["latitude"] | 0.0f;
    data.longitude = doc["longitude"] | 0.0f;
    data.healthStatus = doc["healthStatus"] | "UNKNOWN";
    data.timestamp = millis();
    data.isValid = true;
    
    lastPacketTime = millis();
    packetsReceived++;
    
    Serial.print("✓ Cow Data Received from: ");
    Serial.println(data.deviceId);
    
    return true;
}

bool LoRaComm::receiveMilkData(MilkData& data) {
    if (!checkForData()) return false;
    
    String packet = "";
    while (LoRa.available()) {
        packet += (char)LoRa.read();
    }
    
    String deviceType, jsonData;
    if (!parsePacket(packet, deviceType, jsonData)) {
        return false;
    }
    
    if (deviceType != "MILK") {
        return false;  // Not a milk data packet
    }
    
    // Parse JSON
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, jsonData);
    
    if (error) {
        Serial.print("JSON parse error: ");
        Serial.println(error.c_str());
        return false;
    }
    
    // Extract data
    data.deviceId = doc["deviceId"].as<String>();
    data.cowId = doc["cowId"] | "UNKNOWN";
    data.temperature = doc["temperature"] | 0.0f;
    data.ph = doc["ph"] | 0.0f;
    data.conductivity = doc["conductivity"] | 0.0f;
    data.flowRate = doc["flowRate"] | 0.0f;
    data.volume = doc["volume"] | 0.0f;
    data.quality = doc["quality"] | "UNKNOWN";
    data.sessionActive = doc["sessionActive"] | false;
    data.timestamp = millis();
    data.isValid = true;
    
    lastPacketTime = millis();
    packetsReceived++;
    
    Serial.print("✓ Milk Data Received from: ");
    Serial.println(data.deviceId);
    
    return true;
}

bool LoRaComm::sendCommand(String deviceId, String command) {
    if (!initialized) return false;
    
    String packet = "CMD:" + deviceId + ":" + command;
    
    LoRa.beginPacket();
    LoRa.print(packet);
    LoRa.endPacket();
    
    Serial.print("Sent command to ");
    Serial.print(deviceId);
    Serial.print(": ");
    Serial.println(command);
    
    return true;
}

int LoRaComm::getPacketRSSI() {
    return LoRa.packetRssi();
}

float LoRaComm::getSNR() {
    return LoRa.packetSnr();
}

void LoRaComm::printStatus() {
    Serial.println("\n=== LoRa Status ===");
    Serial.print("Status: ");
    Serial.println(initialized ? "Initialized" : "Not Initialized");
    if (initialized) {
        Serial.print("Frequency: ");
        Serial.print(LORA_FREQUENCY / 1E6);
        Serial.println(" MHz");
        Serial.print("Packets Received: ");
        Serial.println(packetsReceived);
        Serial.print("Last Packet: ");
        Serial.print((millis() - lastPacketTime) / 1000);
        Serial.println(" sec ago");
    }
    Serial.println("===================");
}
