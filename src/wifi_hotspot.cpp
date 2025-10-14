#include "wifi_hotspot.h"
#include "ui.h"
#include <WiFi.h>
#include "config.h"
#include "settings.h"
#include "menu.h"

void initWiFi() {
  String ssid = settings.containsKey("wifi_ssid") ? settings["wifi_ssid"].as<String>() : "ESP32_Hotspot";
  String pass = settings.containsKey("wifi_pass") ? settings["wifi_pass"].as<String>() : "password";
  WiFi.softAP(ssid.c_str(), pass.c_str());
}

void wifiMenu() {
  tft.fillScreen(TFT_BLACK);
  drawTitleBar("WiFi Hotspot");

  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.print("SSID: ");
  tft.print(WiFi.softAPSSID());
  tft.setCursor(10, 60);
  tft.print("IP: ");
  tft.print(WiFi.softAPIP());

  drawButton(20, 90, 280, 40, "Change SSID", TFT_GREEN);
  drawButton(20, 140, 280, 40, "Back", TFT_RED);

  while (true) {
    if (isTouched(20, 90, 280, 40)) {
      // Placeholder input
      settings["wifi_ssid"] = "NewSSID";
      saveSettings();
      initWiFi();
      wifiMenu();  // Refresh
    } else if (isTouched(20, 140, 280, 40)) {
      showMainMenu();
      return;
    }
    delay(50);
  }
}