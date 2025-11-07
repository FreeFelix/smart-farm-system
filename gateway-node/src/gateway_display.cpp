#include "gateway_display.h"

GatewayDisplay::GatewayDisplay() : 
    initialized(false), currentPage(0), lastUpdate(0),
    lastBacklightToggle(0), backlightOn(true) {
}

bool GatewayDisplay::begin() {
    Serial.println("Initializing TFT Display (ILI9341)...");
    Serial.printf("TFT Configuration - CS:%d, DC:%d, RST:%d, BL:%d\n", TFT_CS, TFT_DC, TFT_RST, TFT_BL);
    
    // Initialize backlight pin first and ensure it's ON
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);  // Turn on backlight immediately
    delay(100);  // Give backlight time to stabilize
    Serial.println("✓ TFT Backlight ON");
    
    // Reset display explicitly
    pinMode(TFT_RST, OUTPUT);
    digitalWrite(TFT_RST, LOW);
    delay(20);
    digitalWrite(TFT_RST, HIGH);
    delay(150);
    Serial.println("✓ TFT Reset sequence completed");
    
    // Initialize TFT
    tft.init();
    delay(100);
    Serial.println("✓ TFT Controller initialized");
    
    // Set to landscape mode for 320x240 (rotation 1 or 3)
    tft.setRotation(1);  // Landscape mode
    delay(50);
    Serial.printf("✓ TFT Rotation set to landscape (%dx%d)\n", tft.width(), tft.height());
    
    // Clear screen first
    tft.fillScreen(TFT_BLACK);
    delay(200);
    
    // Fill with bright colors to test (more visible sequence)
    Serial.println("Testing TFT with color sequence...");
    tft.fillScreen(TFT_RED);
    delay(1000);  // Longer delay for visibility
    tft.fillScreen(TFT_GREEN);
    delay(1000);
    tft.fillScreen(TFT_BLUE);
    delay(1000);
    tft.fillScreen(TFT_WHITE);
    delay(1000);
    
    backlightOn = true;
    
    Serial.println("✓ TFT Display Initialized and color tested!");
    
    // Show welcome screen with larger text
    tft.fillScreen(TFT_DARKBLUE);
    tft.setTextColor(TFT_WHITE, TFT_DARKBLUE);
    tft.setTextSize(4);  // Larger text
    tft.setCursor(40, 60);
    tft.println("SMART");
    tft.setCursor(60, 100);
    tft.println("FARM");
    tft.setTextSize(2);
    tft.setCursor(70, 150);
    tft.println("GATEWAY v2.0");
    tft.setTextSize(1);
    tft.setCursor(100, 160);
    tft.println("Initializing...");
    
    delay(2000);
    
    initialized = true;
    return true;
}

bool GatewayDisplay::isInitialized() {
    return initialized;
}

void GatewayDisplay::drawHeader(String title) {
    // Header bar
    tft.fillRect(0, 0, 320, 30, TFT_BLUE);
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.setTextSize(2);
    tft.setCursor(10, 8);
    tft.println(title);
}

void GatewayDisplay::drawStatusBar(const GatewayStatus& status) {
    // Bottom status bar
    tft.fillRect(0, 210, 320, 30, TFT_DARKGREY);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    
    // WiFi status
    drawConnectionIcon(5, 215, status.wifiConnected);
    tft.setCursor(25, 218);
    tft.print("WiFi");
    
    // MQTT status
    drawConnectionIcon(70, 215, status.mqttConnected);
    tft.setCursor(90, 218);
    tft.print("MQTT");
    
    // LoRa status
    drawConnectionIcon(135, 215, status.loraActive);
    tft.setCursor(155, 218);
    tft.print("LoRa");
    
    // Uptime
    tft.setCursor(200, 218);
    tft.print("Up:");
    tft.print(status.uptime / 60000);
    tft.print("m");
}

void GatewayDisplay::drawConnectionIcon(int x, int y, bool connected) {
    uint16_t color = connected ? TFT_GREEN : TFT_RED;
    tft.fillCircle(x + 5, y + 5, 5, color);
}

void GatewayDisplay::drawBatteryIcon(int x, int y, int percent) {
    uint16_t color;
    if (percent > 50) color = TFT_GREEN;
    else if (percent > 20) color = TFT_YELLOW;
    else color = TFT_RED;
    
    // Battery outline
    tft.drawRect(x, y, 20, 10, TFT_WHITE);
    tft.fillRect(x + 20, y + 3, 2, 4, TFT_WHITE);
    
    // Battery fill
    int fillWidth = (percent * 18) / 100;
    tft.fillRect(x + 1, y + 1, fillWidth, 8, color);
}

void GatewayDisplay::drawHealthIndicator(int x, int y, String status) {
    uint16_t color;
    if (status == "HEALTHY" || status == "GOOD") color = TFT_GREEN;
    else if (status == "WARNING" || status == "FAIR") color = TFT_YELLOW;
    else color = TFT_RED;
    
    tft.fillCircle(x, y, 8, color);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(x + 15, y - 5);
    tft.print(status);
}

void GatewayDisplay::drawProgressBar(int x, int y, int w, int h, int percent, uint16_t color) {
    // Border
    tft.drawRect(x, y, w, h, TFT_WHITE);
    
    // Fill
    int fillWidth = ((w - 2) * percent) / 100;
    tft.fillRect(x + 1, y + 1, fillWidth, h - 2, color);
}

void GatewayDisplay::showDashboard(const GatewayStatus& status) {
    tft.fillScreen(TFT_BLACK);
    drawHeader("SYSTEM DASHBOARD");
    
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    
    // Row 1: Connection Status
    tft.setCursor(10, 40);
    tft.print("WiFi:");
    tft.setTextColor(status.wifiConnected ? TFT_GREEN : TFT_RED);
    tft.print(status.wifiConnected ? " ONLINE" : " OFFLINE");
    
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 65);
    tft.print("MQTT:");
    tft.setTextColor(status.mqttConnected ? TFT_GREEN : TFT_RED);
    tft.print(status.mqttConnected ? " ONLINE" : " OFFLINE");
    
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 90);
    tft.print("LoRa:");
    tft.setTextColor(status.loraActive ? TFT_GREEN : TFT_RED);
    tft.print(status.loraActive ? " ACTIVE" : " INACTIVE");
    
    // Row 2: Device Counts
    tft.setTextColor(TFT_CYAN);
    tft.setTextSize(3);
    tft.setCursor(10, 125);
    tft.print("Cows: ");
    tft.setTextColor(TFT_YELLOW);
    tft.print(status.cowNodesActive);
    
    tft.setTextColor(TFT_CYAN);
    tft.setCursor(10, 155);
    tft.print("Milk: ");
    tft.setTextColor(TFT_YELLOW);
    tft.print(status.milkStationsActive);
    
    // Memory usage
    tft.setTextSize(1);
    tft.setTextColor(TFT_LIGHTGREY);
    tft.setCursor(200, 125);
    tft.print("Free Heap:");
    tft.setCursor(200, 140);
    tft.print(status.freeHeap / 1024);
    tft.print(" KB");
    
    // RSSI
    tft.setCursor(200, 160);
    tft.print("RSSI:");
    tft.setCursor(200, 175);
    tft.print(status.rssi);
    tft.print(" dBm");
    
    drawStatusBar(status);
}

void GatewayDisplay::showCowList(CowData cowData[], int count) {
    tft.fillScreen(TFT_BLACK);
    drawHeader("COW HEALTH STATUS");
    
    tft.setTextSize(1);
    
    int y = 40;
    for (int i = 0; i < min(count, 6); i++) {  // Show max 6 cows
        if (!cowData[i].isValid) continue;
        
        tft.setTextColor(TFT_WHITE);
        tft.setCursor(10, y);
        tft.print(cowData[i].deviceId);
        
        // Temperature
        tft.setCursor(80, y);
        tft.print(cowData[i].temperature, 1);
        tft.print("C");
        
        // Heart Rate
        tft.setCursor(140, y);
        tft.print((int)cowData[i].heartRate);
        tft.print(" bpm");
        
        // Battery
        drawBatteryIcon(220, y, cowData[i].batteryLevel);
        
        // Health indicator
        drawHealthIndicator(280, y + 5, cowData[i].healthStatus);
        
        y += 25;
    }
    
    drawStatusBar(GatewayStatus());  // Dummy status for bar
}

void GatewayDisplay::showMilkList(MilkData milkData[], int count) {
    tft.fillScreen(TFT_BLACK);
    drawHeader("MILK QUALITY STATUS");
    
    tft.setTextSize(1);
    
    int y = 40;
    for (int i = 0; i < min(count, 6); i++) {
        if (!milkData[i].isValid) continue;
        
        tft.setTextColor(TFT_WHITE);
        tft.setCursor(10, y);
        tft.print(milkData[i].deviceId);
        
        // Temperature
        tft.setCursor(90, y);
        tft.print(milkData[i].temperature, 1);
        tft.print("C");
        
        // pH
        tft.setCursor(150, y);
        tft.print("pH:");
        tft.print(milkData[i].ph, 1);
        
        // Volume
        tft.setCursor(210, y);
        tft.print(milkData[i].volume, 1);
        tft.print("L");
        
        // Quality indicator
        drawHealthIndicator(280, y + 5, milkData[i].quality);
        
        y += 25;
    }
    
    drawStatusBar(GatewayStatus());
}

void GatewayDisplay::showDetailedCowData(const CowData& data) {
    tft.fillScreen(TFT_BLACK);
    drawHeader(data.deviceId + " DETAILS");
    
    tft.setTextSize(2);
    tft.setTextColor(TFT_YELLOW);
    
    int y = 45;
    
    // Temperature
    tft.setCursor(10, y);
    tft.print("Temp: ");
    tft.setTextColor(TFT_WHITE);
    tft.print(data.temperature, 1);
    tft.print(" C");
    y += 25;
    
    // Heart Rate
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, y);
    tft.print("HR:   ");
    tft.setTextColor(TFT_WHITE);
    tft.print((int)data.heartRate);
    tft.print(" bpm");
    y += 25;
    
    // Activity
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, y);
    tft.print("Act:  ");
    tft.setTextColor(TFT_WHITE);
    tft.print((int)data.activity);
    tft.print(" %");
    y += 25;
    
    // Battery
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, y);
    tft.print("Batt: ");
    tft.setTextColor(TFT_WHITE);
    tft.print((int)data.batteryLevel);
    tft.print(" %");
    y += 25;
    
    // Health Status
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, y);
    tft.print("Status: ");
    drawHealthIndicator(150, y + 10, data.healthStatus);
    
    drawStatusBar(GatewayStatus());
}

void GatewayDisplay::showDetailedMilkData(const MilkData& data) {
    tft.fillScreen(TFT_BLACK);
    drawHeader(data.deviceId + " DETAILS");
    
    tft.setTextSize(2);
    tft.setTextColor(TFT_YELLOW);
    
    int y = 45;
    
    // Temperature
    tft.setCursor(10, y);
    tft.print("Temp: ");
    tft.setTextColor(TFT_WHITE);
    tft.print(data.temperature, 1);
    tft.print(" C");
    y += 25;
    
    // pH
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, y);
    tft.print("pH:   ");
    tft.setTextColor(TFT_WHITE);
    tft.print(data.ph, 2);
    y += 25;
    
    // Conductivity
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, y);
    tft.print("Cond: ");
    tft.setTextColor(TFT_WHITE);
    tft.print(data.conductivity, 1);
    tft.print(" mS");
    y += 25;
    
    // Volume
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, y);
    tft.print("Vol:  ");
    tft.setTextColor(TFT_WHITE);
    tft.print(data.volume, 2);
    tft.print(" L");
    y += 25;
    
    // Quality
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, y);
    tft.print("Quality: ");
    drawHealthIndicator(180, y + 10, data.quality);
    
    drawStatusBar(GatewayStatus());
}

void GatewayDisplay::update(const GatewayStatus& status, CowData cowData[], int cowCount,
                            MilkData milkData[], int milkCount) {
    if (!initialized) return;
    
    // Auto-rotate pages every 5 seconds
    if (millis() - lastUpdate > 5000) {
        currentPage = (currentPage + 1) % 3;
        lastUpdate = millis();
    }
    
    switch (currentPage) {
        case 0:
            showDashboard(status);
            break;
        case 1:
            showCowList(cowData, cowCount);
            break;
        case 2:
            showMilkList(milkData, milkCount);
            break;
    }
}

void GatewayDisplay::displayMessage(String title, String message, uint16_t color) {
    tft.fillScreen(TFT_BLACK);
    drawHeader(title);
    
    tft.setTextSize(2);
    tft.setTextColor(color);
    tft.setCursor(20, 100);
    tft.println(message);
}

void GatewayDisplay::displayAlert(String message, bool critical) {
    uint16_t color = critical ? TFT_RED : TFT_YELLOW;
    uint16_t bgColor = critical ? TFT_MAROON : TFT_OLIVE;
    
    tft.fillScreen(bgColor);
    tft.setTextSize(2);
    tft.setTextColor(color);
    tft.setCursor(10, 10);
    tft.println("ALERT!");
    
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 50);
    tft.println(message);
}

void GatewayDisplay::nextPage() {
    currentPage = (currentPage + 1) % 3;
    lastUpdate = millis();
}

void GatewayDisplay::previousPage() {
    currentPage = (currentPage - 1 + 3) % 3;
    lastUpdate = millis();
}

void GatewayDisplay::setPage(int page) {
    currentPage = page % 3;
    lastUpdate = millis();
}

void GatewayDisplay::setBrightness(int level) {
    // PWM control for backlight (0-100%)
    int pwmValue = map(level, 0, 100, 0, 255);
    analogWrite(TFT_BL, pwmValue);
}

void GatewayDisplay::clear() {
    tft.fillScreen(TFT_BLACK);
}

void GatewayDisplay::sleep() {
    digitalWrite(TFT_BL, LOW);
    backlightOn = false;
}

void GatewayDisplay::wake() {
    digitalWrite(TFT_BL, HIGH);
    backlightOn = true;
}

int GatewayDisplay::getCurrentPage() {
    return currentPage;
}
