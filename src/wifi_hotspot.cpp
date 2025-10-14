#include "wifi_hotspot.h"
#include "ui.h"
#include <WiFi.h>
#include "config.h"
#include "settings.h"
#include "menu.h"

void initWiFi() {
  // Load SSID/PASS from settings
  String ssid = settings.containsKey("wifi_ssid") ? settings["wifi_ssid"].as<String>() : "ESP32_Hotspot";
  String pass = settings.containsKey("wifi_pass") ? settings["wifi_pass"].as<String>() : "password";
  WiFi.softAP(ssid.c_str(), pass.c_str());
}

void wifiMenu() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("WiFi Hotspot");

  tft.setCursor(10, 50);
  tft.print("SSID: ");
  tft.print(WiFi.softAPSSID());
  tft.setCursor(10, 70);
  tft.print("IP: ");
  tft.print(WiFi.softAPIP());

  drawButton(20, 100, 280, 40, "Change SSID", TFT_GREEN);
  drawButton(20, 150, 280, 40, "Back", TFT_RED);

  while (true) {
    if (isTouched(20, 100, 280, 40)) {
      // Placeholder for input; in real, add text input UI
      settings["wifi_ssid"] = "NewSSID";
      saveSettings();
      initWiFi();  // Restart hotspot
      wifiMenu();  // Refresh
    } else if (isTouched(20, 150, 280, 40)) {
      showMainMenu();
      return;
    }
  }
}