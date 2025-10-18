#include <Arduino.h>
#include "config.h"
#include "ui.h"
#include "menu.h"
#include "nrf_control.h"
#include "wifi_hotspot.h"
#include "bt_control.h"
#include "settings.h"

TFT_eSPI tft = TFT_eSPI();  // Pins set dynamically below
XPT2046_Touchscreen touch;  // Initialized with dynamic pin

void setup() {
  Serial.begin(115200);
  loadSettings();  // Load pins first
  initSD();  // Now with dynamic pin
  tft.begin();
  tft.setRotation(1);
  touch = XPT2046_Touchscreen(TOUCH_CS_PIN, TOUCH_IRQ_PIN);
  initTouch();
  // Splash
  drawBmp("/splash.bmp", 0, 0);
  delay(3000);
  tft.fillScreen(TFT_BLACK);
  initNRF();
  initWiFi();
  initBT();
  showMainMenu();
}

void loop() {
  if (touch.touched()) {
    TS_Point p = touch.getPoint();
    int x = map(p.x, 200, 3800, 0, SCREEN_WIDTH);
    int y = map(p.y, 300, 3600, 0, SCREEN_HEIGHT);
  }
  delay(50);
}