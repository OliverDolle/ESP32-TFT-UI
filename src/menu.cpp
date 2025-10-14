#include "menu.h"
#include "ui.h"
#include "nrf_control.h"
#include "wifi_hotspot.h"
#include "bt_control.h"
#include "settings.h"

// Menu items
struct MenuItem {
  const char* label;
  void (*callback)();
};

MenuItem menuItems[] = {
  {"NRF Control", nrfMenu},
  {"WiFi Hotspot", wifiMenu},
  {"Bluetooth", btMenu},
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

void addMenuItem(const char* label, void (*callback)()) {
  // For dynamic addition (expand array if needed)
}