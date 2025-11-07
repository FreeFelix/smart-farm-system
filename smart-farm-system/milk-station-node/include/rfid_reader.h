// RFID Reader Module - Header
// Reads cow identification tags at milking station

#ifndef RFID_READER_H
#define RFID_READER_H

#include <Arduino.h>

void setupRFID();
bool readCowTag(String &cowID);
bool isCowPresent();

#endif
