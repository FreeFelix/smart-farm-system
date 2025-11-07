// LCD Display Module - Implementation
// 20x4 LCD display for milk station

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "config.h"
#include "lcd_display.h"

// LCD instance (20 columns, 4 rows)
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

void setupLCD() {
    Wire.begin(I2C_SDA, I2C_SCL);
    lcd.init();
    lcd.backlight();
    lcd.clear();
    Serial.println("LCD Display initialized");
}

void displayWelcome() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  MILK STATION     ");
    lcd.setCursor(0, 1);
    lcd.print("  Smart Dairy Farm ");
    lcd.setCursor(0, 2);
    lcd.print("  System Ready     ");
    lcd.setCursor(0, 3);
    lcd.print("                    ");
}

void displayWaitingForCow() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waiting for cow...  ");
    lcd.setCursor(0, 1);
    lcd.print("Scan RFID tag       ");
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print("                    ");
}

void displayMilkingSession(const String &cowID, float volume, float weight) {
    lcd.setCursor(0, 0);
    lcd.print("Cow: ");
    lcd.print(cowID.substring(0, 15));
    
    lcd.setCursor(0, 1);
    lcd.print("Vol: ");
    lcd.print(volume, 1);
    lcd.print(" L        ");
    
    lcd.setCursor(0, 2);
    lcd.print("Wt: ");
    lcd.print(weight, 1);
    lcd.print(" kg       ");
}

void displayMilkQuality(const CowHealthData &data) {
    lcd.setCursor(0, 3);
    lcd.print("T:");
    lcd.print(data.milkTemperature, 1);
    lcd.print(" pH:");
    lcd.print(data.milkPH, 1);
    lcd.print(" ");
    
    // Show quality indicator
    if ((data.milkPHAlert || data.mastitisAlert ? 0.3 : 0.9) > 0.8) {
        lcd.print("OK ");
    } else if ((data.milkPHAlert || data.mastitisAlert ? 0.3 : 0.9) > 0.5) {
        lcd.print("WRN");
    } else {
        lcd.print("BAD");
    }
}

void displaySessionComplete(float totalVolume, float totalWeight) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Session Complete    ");
    lcd.setCursor(0, 1);
    lcd.print("Total: ");
    lcd.print(totalVolume, 1);
    lcd.print(" L      ");
    lcd.setCursor(0, 2);
    lcd.print("Weight: ");
    lcd.print(totalWeight, 1);
    lcd.print(" kg   ");
    lcd.setCursor(0, 3);
    lcd.print("Thank you!          ");
}

void displayAlert(const String &message) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*** ALERT ***       ");
    lcd.setCursor(0, 1);
    
    // Wrap message across rows 1-3
    int len = message.length();
    if (len <= 20) {
        lcd.print(message);
    } else if (len <= 40) {
        lcd.print(message.substring(0, 20));
        lcd.setCursor(0, 2);
        lcd.print(message.substring(20));
    } else {
        lcd.print(message.substring(0, 20));
        lcd.setCursor(0, 2);
        lcd.print(message.substring(20, 40));
        lcd.setCursor(0, 3);
        if (len > 40) {
            lcd.print(message.substring(40, 60));
        }
    }
}
