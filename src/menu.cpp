#include "menu.h"
#include "ui.h"
#include "nrf_control.h"
#include "cc1101_control.h"
#include "wifi_hotspot.h"
#include "bt_control.h"
#include "settings.h"

// Configurable pins array for easy extension
struct PinConfig {
  const char* name;
  const char* jsonKey;
  int currentVal;
};

PinConfig pinConfigs[] = {
  {"TFT CS", "tft_cs", TFT_CS_PIN},
  {"TFT DC", "tft_dc", TFT_DC_PIN},
  {"TFT RST", "tft_rst", TFT_RST_PIN},
  {"Touch CS", "touch_cs", TOUCH_CS_PIN},
  {"Touch IRQ", "touch_irq", TOUCH_IRQ_PIN},
  {"SD CS", "sd_cs", SD_CS_PIN},
  {"NRF1 CE", "nrf1_ce", NRF1_CE_PIN},
  {"NRF1 CSN", "nrf1_csn", NRF1_CSN_PIN},
  {"NRF2 CE", "nrf2_ce", NRF2_CE_PIN},
  {"NRF2 CSN", "nrf2_csn", NRF2_CSN_PIN},
  {"CC1101 CSN", "cc1101_csn", CC1101_CSN_PIN},
  {"CC1101 GDO0", "cc1101_gdo0", CC1101_GDO0_PIN},
  {"CC1101 GDO2", "cc1101_gdo2", CC1101_GDO2_PIN},
};

int numPins = sizeof(pinConfigs) / sizeof(PinConfig);

struct MenuItem {
  const char* label;
  void (*callback)();
};

MenuItem menuItems[] = {
  {"NRF Control", nrfMenu},
  {"CC1101 Control", cc1101Menu},
  {"WiFi Hotspot", wifiMenu},
  {"Bluetooth", btMenu},
  {"Settings", settingsMenu},
};

int numItems = sizeof(menuItems) / sizeof(MenuItem);

void showMainMenu() {
  tft.fillScreen(TFT_BLACK);
  drawTitleBar("Main Menu");

  int btnY = 40;
  for (int i = 0; i < numItems; i++) {
    drawButton(20, btnY, 280, 40, menuItems[i].label, TFT_BLUE);
    btnY += 50;
  }
}

void settingsMenu() {
  tft.fillScreen(TFT_BLACK);
  drawTitleBar("Settings");

  drawButton(20, 40, 280, 40, "Pin Configuration", TFT_GREEN);
  drawButton(20, 90, 280, 40, "Splash Screen", TFT_GREEN);
  drawButton(20, 140, 280, 40, "Back", TFT_RED);

  while (true) {
    if (isTouched(20, 40, 280, 40)) {
      pinConfigMenu();
    } else if (isTouched(20, 90, 280, 40)) {
      splashConfigMenu();
    } else if (isTouched(20, 140, 280, 40)) {
      showMainMenu();
      return;
    }
    delay(50);
  }
}

void pinConfigMenu() {
  tft.fillScreen(TFT_BLACK);
  drawTitleBar("Pin Configuration");

  int btnY = 40;
  for (int i = 0; i < numPins; i++) {
    String label = String(pinConfigs[i].name) + ": " + String(pinConfigs[i].currentVal);
    drawButton(20, btnY, 280, 40, label.c_str(), TFT_CYAN);
    btnY += 50;
    if (btnY > 200) break;  // Scroll if too many
  }
  drawButton(20, btnY, 280, 40, "Back", TFT_RED);

  while (true) {
    int btnY = 40;
    for (int i = 0; i < numPins; i++) {
      if (isTouched(20, btnY, 280, 40)) {
        String prompt = "Set " + String(pinConfigs[i].name) + " (0-39)";
        int newVal = getNumberInput(prompt.c_str(), pinConfigs[i].currentVal);
        if (newVal != pinConfigs[i].currentVal && newVal >= 0) {
          settings[pinConfigs[i].jsonKey] = newVal;
          saveSettings();
          tft.setCursor(10, 200);
          tft.print("Saved! Restarting...");
          delay(2000);
          ESP.restart();
        }
        pinConfigMenu();  // Refresh
      }
      btnY += 50;
    }
    if (isTouched(20, btnY, 280, 40)) {
      settingsMenu();
      return;
    }
    delay(50);
  }
}

void addMenuItem(const char* label, void (*callback)()) {
  // Dynamic addition possible here
}

void splashConfigMenu() {
  tft.fillScreen(TFT_BLACK);
  drawTitleBar("Splash Screen Config");

  int currentDuration = settings.containsKey("splash_duration") ? 
                        settings["splash_duration"].as<int>() : 3000;
  String splashFile = settings.containsKey("splash_file") ? 
                      settings["splash_file"].as<String>() : "/splash.bmp";

  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.print("File: ");
  tft.print(splashFile);
  tft.setCursor(10, 60);
  tft.print("Duration: ");
  tft.print(currentDuration / 1000);
  tft.print("s");

  drawButton(20, 90, 280, 40, "Set Duration (1-10s)", TFT_CYAN);
  drawButton(20, 140, 280, 40, "Default (/splash.bmp)", TFT_CYAN);
  drawButton(20, 190, 280, 40, "Custom Filename", TFT_CYAN);
  drawButton(20, 240, 280, 40, "Test Splash Now", TFT_GREEN);
  drawButton(20, 290, 280, 40, "Back", TFT_RED);

  while (true) {
    if (isTouched(20, 90, 280, 40)) {
      int seconds = getNumberInput("Splash Duration (1-10)", currentDuration / 1000);
      if (seconds > 0 && seconds <= 10) {
        settings["splash_duration"] = seconds * 1000;
        saveSettings();
        tft.setCursor(10, 360);
        tft.fillRect(10, 360, 300, 20, TFT_BLACK);
        tft.print("Saved!");
        delay(1000);
        splashConfigMenu();
        return;
      }
    } else if (isTouched(20, 140, 280, 40)) {
      settings["splash_file"] = "/splash.bmp";
      saveSettings();
      tft.setCursor(10, 360);
      tft.fillRect(10, 360, 300, 20, TFT_BLACK);
      tft.print("Set to default");
      delay(1000);
      splashConfigMenu();
      return;
    } else if (isTouched(20, 190, 280, 40)) {
      String newFile = getTextInput("Enter filename", splashFile);
      if (newFile.length() > 0) {
        if (!newFile.startsWith("/")) {
          newFile = "/" + newFile;
        }
        if (!newFile.endsWith(".bmp")) {
          newFile += ".bmp";
        }
        settings["splash_file"] = newFile;
        saveSettings();
        tft.setCursor(10, 360);
        tft.fillRect(10, 360, 300, 20, TFT_BLACK);
        tft.print("Saved!");
        delay(1000);
        splashConfigMenu();
        return;
      }
    } else if (isTouched(20, 240, 280, 40)) {
      // Test splash screen
      File testFile = SD.open(splashFile);
      if (testFile) {
        testFile.close();
        drawBmp(splashFile.c_str(), 0, 0);
        delay(currentDuration);
        splashConfigMenu();
        return;
      } else {
        tft.fillRect(10, 360, 300, 20, TFT_BLACK);
        tft.setCursor(10, 360);
        tft.setTextColor(TFT_RED);
        tft.print("File not found!");
        delay(2000);
        tft.setTextColor(TFT_WHITE);
      }
    } else if (isTouched(20, 290, 280, 40)) {
      settingsMenu();
      return;
    }
    delay(50);
  }
}