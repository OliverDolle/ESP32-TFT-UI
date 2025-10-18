#include "settings.h"
#include "config.h"

DynamicJsonDocument settings(1024);

int TFT_CS_PIN;
int TFT_DC_PIN;
int TFT_RST_PIN;
int TOUCH_CS_PIN;
int TOUCH_IRQ_PIN;
int SD_CS_PIN;
int NRF1_CE_PIN;
int NRF1_CSN_PIN;
int NRF2_CE_PIN;
int NRF2_CSN_PIN;

void initSD() {
  if (!SD.begin(SD_CS_PIN)) {
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
    settings["wifi_ssid"] = "ESP32_Extender";
    settings["wifi_pass"] = "password";
    settings["extender_mode"] = false;
    settings["sta_ssid"] = "";
    settings["sta_pass"] = "";
    settings["tft_cs"] = 5;
    settings["tft_dc"] = 17;
    settings["tft_rst"] = 16;
    settings["touch_cs"] = 21;
    settings["touch_irq"] = 22;
    settings["sd_cs"] = 4;
    settings["nrf1_ce"] = 25;
    settings["nrf1_csn"] = 26;
    settings["nrf2_ce"] = 27;
    settings["nrf2_csn"] = 32;
    saveSettings();
  }
  loadPins();
}

void loadPins() {
  TFT_CS_PIN = settings["tft_cs"] | 5;
  TFT_DC_PIN = settings["tft_dc"] | 17;
  TFT_RST_PIN = settings["tft_rst"] | 16;
  TOUCH_CS_PIN = settings["touch_cs"] | 21;
  TOUCH_IRQ_PIN = settings["touch_irq"] | 22;
  SD_CS_PIN = settings["sd_cs"] | 4;
  NRF1_CE_PIN = settings["nrf1_ce"] | 25;
  NRF1_CSN_PIN = settings["nrf1_csn"] | 26;
  NRF2_CE_PIN = settings["nrf2_ce"] | 27;
  NRF2_CSN_PIN = settings["nrf2_csn"] | 32;
}

void saveSettings() {
  File file = SD.open(SETTINGS_FILE, FILE_WRITE);
  if (file) {
    serializeJson(settings, file);
    file.close();
  }
}