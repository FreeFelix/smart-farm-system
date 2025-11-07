#ifndef LORA_COMM_H
#define LORA_COMM_H

#include <LoRa.h>
#include <ArduinoJson.h>
#include "config.h"
#include "device_data.h"

class LoRaComm {
private:
    bool initialized;
    unsigned long lastPacketTime;
    int packetsReceived;
    
    bool parsePacket(String packet, String& deviceType, String& jsonData);
    
public:
    LoRaComm();
    bool begin();
    bool isInitialized();
    bool checkForData();
    bool receiveCowData(CowData& data);
    bool receiveMilkData(MilkData& data);
    bool sendCommand(String deviceId, String command);
    int getPacketRSSI();
    float getSNR();
    void printStatus();
};

#endif // LORA_COMM_H
