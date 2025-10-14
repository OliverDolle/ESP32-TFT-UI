#include "menu.h"
#include "ui.h"
#include "nrf_control.h"
#include "wifi_hotspot.h"
#include "bt_control.h"
#include "settings.h"

// Modular menu system: Add items here for easy extension
struct MenuItem {
  const char* label;
  void (*callback)();
};

MenuItem menuItems[] = {
  {"NRF Control", nrfMenu},
  {"WiFi Hotspot", wifiMenu},
  {"Bluetooth", btMenu},
  // Add new items here, e.g. {"New Function", newFunctionMenu}
};

int numItems = sizeof(menuItems) / sizeof(MenuItem);

void showMainMenu() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(10, 10);
  tft.print("Main Menu");

  int btnY = 50;
  for (int i = 0; i < numItems; i++) {
    drawButton(20, btnY, 280, 40, menuItems[i].label, TFT_BLUE);
    btnY += 50;
  }
}

void addMenuItem(const char* label, void (*callback)()) {
  // To add dynamically, but for simplicity, add to array above and update numItems
  // For runtime addition, use a vector or dynamic array
}