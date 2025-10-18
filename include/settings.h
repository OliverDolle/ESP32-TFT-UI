#ifndef SETTINGS_H
#define SETTINGS_H

#include <ArduinoJson.h>
#include <SD.h>

void initSD();
void loadSettings();
void saveSettings();
extern DynamicJsonDocument settings;

// Load dynamic pins
void loadPins();

#endif