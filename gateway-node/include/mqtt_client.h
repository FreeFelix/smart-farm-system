#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "config.h"
#include "device_data.h"

class MQTTClient {
private:
    WiFiClient wifiClient;
    PubSubClient mqttClient;
    unsigned long lastReconnectAttempt;
    bool connected;
    
    void callback(char* topic, byte* payload, unsigned int length);
    
public:
    MQTTClient();
    bool begin();
    bool connect();
    bool isConnected();
    void disconnect();
    bool publishCowData(const CowData& data);
    bool publishMilkData(const MilkData& data);
    bool publishGatewayStatus(const GatewayStatus& status);
    void loop();
    void handleConnection();
    
    // Callback for received messages
    void (*onMessageReceived)(String topic, String payload);
};

#endif // MQTT_CLIENT_H
