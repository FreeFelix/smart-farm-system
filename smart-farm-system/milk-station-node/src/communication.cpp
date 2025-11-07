// Milk Station Node - Communication Implementation
// LoRa data transmission for milk production data

#include <LoRa.h>
#include <SPI.h>
#include "config.h"
#include "communication.h"

bool setupLoRa() {
    // Initialize SPI for LoRa
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
    
    // Setup LoRa module
    LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
    
    if (!LoRa.begin(LORA_FREQUENCY)) {
        Serial.println("LoRa initialization failed!");
        return false;
    }
    
    // Configure LoRa parameters
    LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);
    LoRa.setSyncWord(0x12);
    LoRa.enableCrc();
    
    return true;
}

void transmitMilkData(const String &cowID, const CowHealthData &data) {
    // Format: MILK|COW_ID|VOLUME|WEIGHT|PH|TEMP|COND|QUALITY|TIMESTAMP
    String payload = "MILK|";
    payload += cowID + "|";
    payload += String(data.milkVolume, 2) + "|";
    payload += String(data.milkWeight, 2) + "|";
    payload += String(data.milkPH, 2) + "|";
    payload += String(data.milkTemperature, 1) + "|";
    payload += String(data.milkConductivity, 2) + "|";
    payload += String((data.milkPHAlert || data.mastitisAlert ? 0.3 : 0.9), 2) + "|";
    payload += String(millis());
    
    // Transmit via LoRa
    LoRa.beginPacket();
    LoRa.print(payload);
    LoRa.endPacket();
    
    Serial.print("Transmitted: ");
    Serial.println(payload);
}
