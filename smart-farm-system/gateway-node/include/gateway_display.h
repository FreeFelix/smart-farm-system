#ifndef GATEWAY_DISPLAY_H
#define GATEWAY_DISPLAY_H

#include <TFT_eSPI.h>
#include "config.h"
#include "device_data.h"

// Color definitions (RGB565 format) - Only define what's not in TFT_eSPI
#define TFT_LIGHTBLUE   0x841F
#define TFT_DARKBLUE    0x0010

class GatewayDisplay {
private:
    TFT_eSPI tft;
    bool initialized;
    int currentPage;
    unsigned long lastUpdate;
    unsigned long lastBacklightToggle;
    bool backlightOn;
    
    // Display pages
    void showDashboard(const GatewayStatus& status);
    void showCowList(CowData cowData[], int count);
    void showMilkList(MilkData milkData[], int count);
    void showDetailedCowData(const CowData& data);
    void showDetailedMilkData(const MilkData& data);
    void showNetworkStatus(const GatewayStatus& status);
    void showStorageStatus();
    void showAlerts();
    
    // UI Components
    void drawHeader(String title);
    void drawStatusBar(const GatewayStatus& status);
    void drawProgressBar(int x, int y, int w, int h, int percent, uint16_t color);
    void drawHealthIndicator(int x, int y, String status);
    void drawConnectionIcon(int x, int y, bool connected);
    void drawBatteryIcon(int x, int y, int percent);
    
public:
    GatewayDisplay();
    bool begin();
    bool isInitialized();
    void update(const GatewayStatus& status, CowData cowData[], int cowCount, 
                MilkData milkData[], int milkCount);
    void displayMessage(String title, String message, uint16_t color);
    void displayAlert(String message, bool critical);
    void nextPage();
    void previousPage();
    void setPage(int page);
    void setBrightness(int level);  // 0-100
    void clear();
    void sleep();
    void wake();
    int getCurrentPage();
};

#endif // GATEWAY_DISPLAY_H
