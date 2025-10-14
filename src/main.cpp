#include <Arduino.h>
#include "config.h"
#include "ui.h"
#include "menu.h"
#include "nrf_control.h"
#include "wifi_hotspot.h"
#include "bt_control.h"
#include "settings.h"

TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

void setup() {
  Serial.begin(115200);
  initDisplay();
  initTouch();
  initSD();
  loadSettings();

  // Splash screen
  drawBmp("/splash.bmp", 0, 0);
  delay(3000);
  tft.fillScreen(TFT_BLACK);

  initNRF();
  initWiFi();
  initBT();
  showMainMenu();
}

void loop() {
  // Main loop handles touch events
  if (touch.touched()) {
    TS_Point p = touch.getPoint();
    int x = map(p.x, 200, 3800, 0, SCREEN_WIDTH);
    int y = map(p.y, 300, 3600, 0, SCREEN_HEIGHT);
    // Handling in menu functions
  }
  delay(50);
}