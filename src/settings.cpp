#include "settings.h"
#include "config.h"

DynamicJsonDocument settings(1024);

bool sdCardAvailable = false;  // Track SD card status globally

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
int CC1101_CSN_PIN;
int CC1101_GDO0_PIN;
int CC1101_GDO2_PIN;

void initSD() {
  Serial.print("Initializing SD card on pin ");
  Serial.print(SD_CS_PIN);
  Serial.println("...");
  
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("╔════════════════════════════════╗");
    Serial.println("║ SD CARD INITIALIZATION FAILED  ║");
    Serial.println("╚════════════════════════════════╝");
    Serial.println("Please check:");
    Serial.println("  1. SD card is inserted");
    Serial.println("  2. SD card is formatted as FAT32");
    Serial.println("  3. SD_CS pin wiring (default GPIO 4)");
    Serial.println("\n⚠️  WARNING: Settings will NOT be saved!");
    Serial.println("System will run with defaults from RAM only\n");
    sdCardAvailable = false;
  } else {
    Serial.println("✓ SD card initialized successfully");
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.print("  Card Size: ");
    Serial.print(cardSize);
    Serial.println(" MB");
    
    uint8_t cardType = SD.cardType();
    Serial.print("  Card Type: ");
    switch(cardType) {
      case CARD_MMC:  Serial.println("MMC"); break;
      case CARD_SD:   Serial.println("SD"); break;
      case CARD_SDHC: Serial.println("SDHC"); break;
      default:        Serial.println("Unknown"); break;
    }
    sdCardAvailable = true;
  }
}

bool isSDCardAvailable() {
  return sdCardAvailable;
}

void loadSettings() {
  Serial.println("Loading settings...");
  
  // Always create defaults first (in case SD fails)
  createDefaultSettings();
  
  if (!sdCardAvailable) {
    Serial.println("⚠️  SD card not available - using defaults from RAM");
    loadPins();
    return;
  }
  
  File file = SD.open(SETTINGS_FILE, FILE_READ);
  if (file) {
    Serial.println("✓ Settings file found, reading...");
    DeserializationError error = deserializeJson(settings, file);
    file.close();
    
    if (error) {
      Serial.print("ERROR: JSON parse failed: ");
      Serial.println(error.c_str());
      Serial.println("Using default settings and creating new file...");
      createDefaultSettings();
      saveSettings();
    } else {
      Serial.println("✓ Settings loaded successfully");
      // Validate and add any missing keys with defaults
      validateAndFillMissingSettings();
    }
  } else {
    Serial.println("Settings file not found");
    Serial.println("Creating default settings.json...");
    if (saveSettings()) {
      Serial.println("✓ Default settings.json created");
    } else {
      Serial.println("⚠️  Could not create settings.json");
      Serial.println("System will use defaults from RAM only");
    }
  }
  loadPins();
}

void createDefaultSettings() {
  Serial.println("Applying default configuration...");
  
  // NRF settings
  settings["nrf_address1"] = "\xE7\xE7\xE7\xE7\xE7";
  settings["nrf_address2"] = "\xD7\xD7\xD7\xD7\xD7";
  settings["nrf_data_rate"] = "2MBPS";
  
  // CC1101 settings
  settings["cc1101_frequency"] = 433.92;
  settings["cc1101_modulation"] = "ASK";
  settings["cc1101_power"] = 10;
  
  // WiFi settings
  settings["wifi_ssid"] = "ESP32_Extender";
  settings["wifi_pass"] = "password";
  settings["extender_mode"] = false;
  settings["sta_ssid"] = "";
  settings["sta_pass"] = "";
  
  // Display pins
  settings["tft_cs"] = 5;
  settings["tft_dc"] = 17;
  settings["tft_rst"] = 16;
  settings["touch_cs"] = 21;
  settings["touch_irq"] = 22;
  settings["sd_cs"] = 4;
  
  // NRF pins
  settings["nrf1_ce"] = 25;
  settings["nrf1_csn"] = 26;
  settings["nrf2_ce"] = 27;
  settings["nrf2_csn"] = 32;
  
  // CC1101 pins
  settings["cc1101_csn"] = 33;
  settings["cc1101_gdo0"] = 14;
  settings["cc1101_gdo2"] = 12;
  
  // Splash screen settings
  settings["splash_file"] = "/splash.bmp";
  settings["splash_duration"] = 3000;
}

void validateAndFillMissingSettings() {
  bool modified = false;
  
  // Check and add missing settings with defaults
  if (!settings.containsKey("nrf_data_rate")) {
    settings["nrf_data_rate"] = "2MBPS";
    modified = true;
  }
  if (!settings.containsKey("cc1101_frequency")) {
    settings["cc1101_frequency"] = 433.92;
    modified = true;
  }
  if (!settings.containsKey("cc1101_modulation")) {
    settings["cc1101_modulation"] = "ASK";
    modified = true;
  }
  if (!settings.containsKey("cc1101_power")) {
    settings["cc1101_power"] = 10;
    modified = true;
  }
  if (!settings.containsKey("splash_file")) {
    settings["splash_file"] = "/splash.bmp";
    modified = true;
  }
  if (!settings.containsKey("splash_duration")) {
    settings["splash_duration"] = 3000;
    modified = true;
  }
  
  // Save if we added any missing keys
  if (modified) {
    Serial.println("Added missing settings, updating file...");
    saveSettings();
  }
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
  CC1101_CSN_PIN = settings["cc1101_csn"] | 33;
  CC1101_GDO0_PIN = settings["cc1101_gdo0"] | 14;  CC1101_GDO2_PIN = settings["cc1101_gdo2"] | 12;
}

bool saveSettings() {
  if (!sdCardAvailable) {
    Serial.println("⚠️  Cannot save: SD card not available");
    return false;
  }
  
  Serial.println("Saving settings to SD card...");
  
  // Delete old file first to ensure clean write
  if (SD.exists(SETTINGS_FILE)) {
    SD.remove(SETTINGS_FILE);
  }
  
  File file = SD.open(SETTINGS_FILE, FILE_WRITE);
  if (file) {
    serializeJsonPretty(settings, file);  // Pretty print for human readability
    file.close();
    Serial.println("✓ Settings saved successfully");
    
    // Verify the save worked
    File verify = SD.open(SETTINGS_FILE, FILE_READ);
    if (verify) {
      Serial.print("  File size: ");
      Serial.print(verify.size());
      Serial.println(" bytes");
      verify.close();
      return true;
    }
  } else {
    Serial.println("ERROR: Could not open settings file for writing");
    Serial.println("Check SD card is not write-protected");
  }
  return false;
}