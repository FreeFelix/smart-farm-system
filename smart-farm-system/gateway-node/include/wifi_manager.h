#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "config.h"

class WiFiManager {
private:
    bool connected;
    unsigned long lastConnectAttempt;
    int reconnectAttempts;

public:
    WiFiManager();
    bool begin();
    bool connect();
    bool createAccessPoint();
    bool isConnected();
    void disconnect();
    int getRSSI();
    String getIP();
    void handleConnection();
    void printStatus();
};

#endif // WIFI_MANAGER_H
