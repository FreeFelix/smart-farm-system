// RFID Reader Module - Implementation
// Reads cow identification tags at milking station

#include <MFRC522.h>
#include <SPI.h>
#include "config.h"
#include "rfid_reader.h"

// RFID reader instance
MFRC522 rfid(RFID_SDA, RFID_RST);

// Last detected tag
static String lastCowID = "";
static unsigned long lastDetectionTime = 0;

void setupRFID() {
    SPI.begin();
    rfid.PCD_Init();
    Serial.println("RFID Reader initialized");
    Serial.print("Reader version: ");
    rfid.PCD_DumpVersionToSerial();
}

bool readCowTag(String &cowID) {
    // Check for new card
    if (!rfid.PICC_IsNewCardPresent()) {
        return false;
    }

    // Read card serial
    if (!rfid.PICC_ReadCardSerial()) {
        return false;
    }

    // Build cow ID from UID
    cowID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
        if (rfid.uid.uidByte[i] < 0x10) {
            cowID += "0";
        }
        cowID += String(rfid.uid.uidByte[i], HEX);
    }
    cowID.toUpperCase();

    // Update last detection
    lastCowID = cowID;
    lastDetectionTime = millis();

    // Halt PICC
    rfid.PICC_HaltA();
    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();

    Serial.print("Cow detected: ");
    Serial.println(cowID);

    return true;
}

bool isCowPresent() {
    // Check if cow was detected recently (within 30 seconds)
    if (lastCowID.length() > 0 && (millis() - lastDetectionTime) < 30000) {
        return true;
    }
    return false;
}
