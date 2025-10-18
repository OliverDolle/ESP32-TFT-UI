#include "menu.h"
#include "ui.h"
#include "nrf_control.h"
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
};

int numPins = sizeof(pinConfigs) / sizeof(PinConfig);

struct MenuItem {
  const char* label;
  void (*callback)();
};

MenuItem menuItems[] = {
  {"NRF Control", nrfMenu},
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
  drawButton(20, 90, 280, 40, "Back", TFT_RED);

  while (true) {
    if (isTouched(20, 40, 280, 40)) {
      pinConfigMenu();
    } else if (isTouched(20, 90, 280, 40)) {
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