#include <Arduino.h>
#include "config.h"
#include "ui.h"
#include "menu.h"
#include "nrf_control.h"
#include "cc1101_control.h"
#include "wifi_hotspot.h"
#include "bt_control.h"
#include "settings.h"

TFT_eSPI tft = TFT_eSPI();  // Pins set dynamically below
XPT2046_Touchscreen touch;  // Initialized with dynamic pin

void setup() {
  Serial.begin(115200);
  delay(500);  // Give serial time to initialize
  
  Serial.println("\n\n==================================");
  Serial.println("ESP32 TFT UI Control System");
  Serial.println("==================================\n");
  
  // Load settings first (uses default SD_CS pin initially)
  loadSettings();
  
  // Initialize SD card with potentially updated SD_CS pin
  initSD();
  
  // Initialize display
  tft.begin();
  tft.setRotation(1);
  touch = XPT2046_Touchscreen(TOUCH_CS_PIN, TOUCH_IRQ_PIN);
  initTouch();
  
  // Display splash screen
  String splashFile = settings.containsKey("splash_file") ? 
                      settings["splash_file"].as<String>() : "/splash.bmp";
  int splashDuration = settings.containsKey("splash_duration") ? 
                       settings["splash_duration"].as<int>() : 3000;
  
  // Try to load splash image, show default screen if not found
  File testFile = SD.open(splashFile);
  if (testFile) {
    testFile.close();
    drawBmp(splashFile.c_str(), 0, 0);
    delay(splashDuration);
  } else {
    // No splash image - show default startup screen
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_CYAN);
    tft.setTextSize(3);
    tft.setCursor(40, 80);
    tft.print("ESP32 TFT UI");
    tft.setTextSize(2);
    tft.setCursor(60, 120);
    tft.print("Initializing...");
    tft.setTextSize(1);
    tft.setCursor(80, 200);
    tft.setTextColor(TFT_YELLOW);
    tft.print("No splash.bmp found");
    delay(splashDuration);
  }
  
  tft.fillScreen(TFT_BLACK);
  
  // Initialize modules
  Serial.println("\nInitializing RF modules...");
  initNRF();
  initCC1101();
  
  Serial.println("Initializing connectivity...");
  initWiFi();
  initBT();
  
  Serial.println("\nSystem ready!");
  Serial.println("==================================\n");
  
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

void showSDCardError() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(3);
  tft.setCursor(30, 60);
  tft.print("SD CARD ERROR");
  
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 100);
  tft.print("Please check:");
  
  tft.setTextSize(1);
  tft.setCursor(10, 130);
  tft.print("1. SD card is inserted");
  tft.setCursor(10, 145);
  tft.print("2. Formatted as FAT32");
  tft.setCursor(10, 160);
  tft.print("3. SD_CS pin connected (GPIO 4)");
  
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 190);
  tft.print("Using defaults...");
  tft.setTextSize(1);
  tft.setCursor(10, 210);
  tft.print("Settings won't be saved");
  
  delay(5000);  // Show for 5 seconds
}