#ifndef SETTINGS_H
#define SETTINGS_H

#include <ArduinoJson.h>
#include <SD.h>

void initSD();
void loadSettings();
bool saveSettings();  // Returns true if save successful
void createDefaultSettings();
void validateAndFillMissingSettings();
bool isSDCardAvailable();  // Check if SD card is working
extern DynamicJsonDocument settings;
extern bool sdCardAvailable;  // Global SD card status flag

// Load dynamic pins
void loadPins();

#endif