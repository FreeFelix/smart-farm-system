#include "mqtt_client.h"

MQTTClient::MQTTClient() : mqttClient(wifiClient), lastReconnectAttempt(0), 
                           connected(false), onMessageReceived(nullptr) {
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setBufferSize(DATA_BUFFER_SIZE);
}

bool MQTTClient::begin() {
    return connect();
}

bool MQTTClient::connect() {
    if (connected) return true;
    
    Serial.print("Connecting to MQTT broker: ");
    Serial.println(MQTT_BROKER);
    
    bool success = false;
    if (strlen(MQTT_USERNAME) > 0) {
        success = mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    } else {
        success = mqttClient.connect(MQTT_CLIENT_ID);
    }
    
    if (success) {
        Serial.println("✓ MQTT Connected!");
        
        // Subscribe to command topic
        mqttClient.subscribe(TOPIC_COMMANDS);
        Serial.print("Subscribed to: ");
        Serial.println(TOPIC_COMMANDS);
        
        // Publish online status
        mqttClient.publish(TOPIC_GATEWAY_STATUS, "{\"status\":\"online\"}");
        
        connected = true;
    } else {
        Serial.print("✗ MQTT Connection Failed, rc=");
        Serial.println(mqttClient.state());
        connected = false;
    }
    
    lastReconnectAttempt = millis();
    return connected;
}

bool MQTTClient::isConnected() {
    connected = mqttClient.connected();
    return connected;
}

void MQTTClient::disconnect() {
    if (connected) {
        mqttClient.publish(TOPIC_GATEWAY_STATUS, "{\"status\":\"offline\"}");
        mqttClient.disconnect();
    }
    connected = false;
    Serial.println("MQTT Disconnected");
}

bool MQTTClient::publishCowData(const CowData& data) {
    if (!connected || !data.isValid) return false;
    
    StaticJsonDocument<512> doc;
    doc["deviceId"] = data.deviceId;
    doc["temperature"] = data.temperature;
    doc["heartRate"] = data.heartRate;
    doc["activity"] = data.activity;
    doc["battery"] = data.batteryLevel;
    doc["gpsFixed"] = data.gpsFixed;
    doc["latitude"] = data.latitude;
    doc["longitude"] = data.longitude;
    doc["healthStatus"] = data.healthStatus;
    doc["timestamp"] = data.timestamp;
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    bool success = mqttClient.publish(TOPIC_COW_DATA, jsonString.c_str());
    
    if (DEBUG_SERIAL) {
        Serial.print("Published Cow Data: ");
        Serial.println(success ? "✓" : "✗");
        if (DEBUG_VERBOSE) Serial.println(jsonString);
    }
    
    return success;
}

bool MQTTClient::publishMilkData(const MilkData& data) {
    if (!connected || !data.isValid) return false;
    
    StaticJsonDocument<512> doc;
    doc["deviceId"] = data.deviceId;
    doc["cowId"] = data.cowId;
    doc["temperature"] = data.temperature;
    doc["ph"] = data.ph;
    doc["conductivity"] = data.conductivity;
    doc["flowRate"] = data.flowRate;
    doc["volume"] = data.volume;
    doc["quality"] = data.quality;
    doc["sessionActive"] = data.sessionActive;
    doc["timestamp"] = data.timestamp;
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    bool success = mqttClient.publish(TOPIC_MILK_DATA, jsonString.c_str());
    
    if (DEBUG_SERIAL) {
        Serial.print("Published Milk Data: ");
        Serial.println(success ? "✓" : "✗");
        if (DEBUG_VERBOSE) Serial.println(jsonString);
    }
    
    return success;
}

bool MQTTClient::publishGatewayStatus(const GatewayStatus& status) {
    if (!connected) return false;
    
    StaticJsonDocument<512> doc;
    doc["wifiConnected"] = status.wifiConnected;
    doc["mqttConnected"] = status.mqttConnected;
    doc["loraActive"] = status.loraActive;
    doc["cowNodes"] = status.cowNodesActive;
    doc["milkStations"] = status.milkStationsActive;
    doc["uptime"] = status.uptime;
    doc["freeHeap"] = status.freeHeap;
    doc["rssi"] = status.rssi;
    doc["lastDataReceived"] = status.lastDataReceived;
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    bool success = mqttClient.publish(TOPIC_GATEWAY_STATUS, jsonString.c_str());
    
    if (DEBUG_SERIAL && DEBUG_VERBOSE) {
        Serial.print("Published Gateway Status: ");
        Serial.println(success ? "✓" : "✗");
    }
    
    return success;
}

void MQTTClient::loop() {
    if (connected) {
        mqttClient.loop();
    }
}

void MQTTClient::handleConnection() {
    if (!isConnected() && millis() - lastReconnectAttempt > RECONNECT_INTERVAL) {
        Serial.println("MQTT lost. Reconnecting...");
        connect();
    }
}

void MQTTClient::callback(char* topic, byte* payload, unsigned int length) {
    if (onMessageReceived != nullptr) {
        String payloadStr = "";
        for (unsigned int i = 0; i < length; i++) {
            payloadStr += (char)payload[i];
        }
        onMessageReceived(String(topic), payloadStr);
    }
}
