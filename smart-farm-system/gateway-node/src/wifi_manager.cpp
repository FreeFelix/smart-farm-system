#include "wifi_manager.h"

WiFiManager::WiFiManager() : connected(false), lastConnectAttempt(0), reconnectAttempts(0) {}

bool WiFiManager::begin() {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    return connect();
}

bool WiFiManager::connect() {
    if (connected) return true;
    
    // Skip connection attempt if in demo mode - go straight to AP
    if (strcmp(WIFI_SSID, "DEMO_MODE_AP_ONLY") == 0) {
        Serial.println("Demo mode detected - creating Access Point directly");
        return createAccessPoint();
    }
    
    Serial.println("Connecting to WiFi...");
    Serial.print("SSID: ");
    Serial.println(WIFI_SSID);
    
    // Try to connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    unsigned long startTime = millis();
    int dotCount = 0;
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT_MS) {
        delay(100);  // Shorter delay
        Serial.print(".");
        dotCount++;
        if (dotCount > 10) {  // Break out early
            break;
        }
    }
    Serial.println();
    
    connected = (WiFi.status() == WL_CONNECTED);
    
    if (connected) {
        Serial.println("✓ WiFi Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("RSSI: ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
        reconnectAttempts = 0;
    } else {
        Serial.println("✗ WiFi Connection Failed - Creating Access Point");
        return createAccessPoint();
    }
    
    lastConnectAttempt = millis();
    return connected;
}

bool WiFiManager::createAccessPoint() {
    // Disconnect from station mode first
    WiFi.disconnect(true);
    delay(100);  // Shorter delay
    
    // Create Access Point for configuration
    WiFi.mode(WIFI_AP);
    bool apStarted = WiFi.softAP("SmartFarm_Gateway", "smartfarm123");
    
    if (apStarted) {
        Serial.println("✓ Access Point Created!");
        Serial.print("AP IP: ");
        Serial.println(WiFi.softAPIP());
        Serial.println("Connect to 'SmartFarm_Gateway' with password: smartfarm123");
        connected = true;  // Consider AP mode as "connected"
    } else {
        Serial.println("✗ Failed to create Access Point!");
        connected = false;
    }
    return apStarted;
}

bool WiFiManager::isConnected() {
    // In AP mode, always consider connected
    if (WiFi.getMode() == WIFI_AP) {
        return true;
    }
    
    connected = (WiFi.status() == WL_CONNECTED);
    return connected;
}

void WiFiManager::disconnect() {
    WiFi.disconnect();
    connected = false;
    Serial.println("WiFi Disconnected");
}

int WiFiManager::getRSSI() {
    if (WiFi.getMode() == WIFI_AP) {
        return WiFi.softAPgetStationNum() > 0 ? -30 : -100;  // Fake RSSI for AP mode
    }
    return WiFi.RSSI();
}

String WiFiManager::getIP() {
    if (WiFi.getMode() == WIFI_AP) {
        return WiFi.softAPIP().toString();
    }
    return WiFi.localIP().toString();
}

void WiFiManager::handleConnection() {
    // In demo mode (AP only), don't try to reconnect
    if (strcmp(WIFI_SSID, "DEMO_MODE_AP_ONLY") == 0) {
        return;
    }
    
    if (!isConnected() && millis() - lastConnectAttempt > RECONNECT_INTERVAL) {
        Serial.println("WiFi lost. Reconnecting...");
        connect();
    }
}

void WiFiManager::printStatus() {
    Serial.println("\n=== WiFi Status ===");
    Serial.print("Status: ");
    Serial.println(connected ? "Connected" : "Disconnected");
    if (connected) {
        Serial.print("IP: ");
        Serial.println(getIP());
        Serial.print("RSSI: ");
        Serial.print(getRSSI());
        Serial.println(" dBm");
    }
    Serial.print("Reconnect Attempts: ");
    Serial.println(reconnectAttempts);
    Serial.println("==================");
}
