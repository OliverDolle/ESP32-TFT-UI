#include "settings.h"
#include "config.h"

DynamicJsonDocument settings(1024);

void initSD() {
  if (!SD.begin(SD_CS)) {
    Serial.println("SD init failed!");
  }
}

void loadSettings() {
  File file = SD.open(SETTINGS_FILE, FILE_READ);
  if (file) {
    DeserializationError error = deserializeJson(settings, file);
    if (error) {
      Serial.println("JSON parse failed");
    }
    file.close();
  } else {
    // Default settings
    settings["nrf_address"] = "E7E7E7E7E7";
    settings["wifi_ssid"] = "ESP32_Hotspot";
    settings["wifi_pass"] = "password";
    saveSettings();
  }
}

void saveSettings() {
  File file = SD.open(SETTINGS_FILE, FILE_WRITE);
  if (file) {
    serializeJson(settings, file);
    file.close();
  }
}