#include "wifi_hotspot.h"
#include "ui.h"
#include "menu.h"
#include <WiFi.h>
#include "config.h"
#include "settings.h"

bool isExtenderMode = false;

void initWiFi() {
  // Disconnect existing connections
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(100);

  String ap_ssid = settings.containsKey("wifi_ssid") ? settings["wifi_ssid"].as<String>() : "ESP32_Extender";
  String ap_pass = settings.containsKey("wifi_pass") ? settings["wifi_pass"].as<String>() : "password";

  if (settings.containsKey("extender_mode") && settings["extender_mode"].as<bool>()) {
    isExtenderMode = true;
    String sta_ssid = settings.containsKey("sta_ssid") ? settings["sta_ssid"].as<String>() : "";
    String sta_pass = settings.containsKey("sta_pass") ? settings["sta_pass"].as<String>() : "";
    
    // Start STA + AP mode
    WiFi.mode(WIFI_AP_STA);
    
    // Connect to upstream router
    if (sta_ssid != "") {
      WiFi.begin(sta_ssid.c_str(), sta_pass.c_str());
      int attempts = 0;
      while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        attempts++;
      }
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Failed to connect to upstream WiFi");
      }
    }
    
    // Start AP
    WiFi.softAP(ap_ssid.c_str(), ap_pass.c_str());
  } else {
    isExtenderMode = false;
    // Hotspot only
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid.c_str(), ap_pass.c_str());
  }
}

// Simple text input (adapted from getNumberInput)
String getTextInput(const char* prompt, String currentVal) {
  tft.fillScreen(TFT_BLACK);
  drawTitleBar(prompt);
  String input = currentVal;
  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.print(input);

  // Draw QWERTY-like keyboard
  const char* keys[] = {
    "1234567890",
    "QWERTYUIOP",
    "ASDFGHJKL",
    "ZXCVBNM"
  };
  int btnSize = 28;
  int startX = 10, startY = 80;

  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < strlen(keys[row]); col++) {
      char key[2] = {keys[row][col], '\0'};
      drawButton(startX + col * (btnSize + 2), startY + row * (btnSize + 10), btnSize, btnSize, key, TFT_CYAN);
    }
  }
  drawButton(startX, startY + 4 * (btnSize + 10), 60, btnSize, "Space", TFT_CYAN);
  drawButton(startX + 70, startY + 4 * (btnSize + 10), 60, btnSize, "<", TFT_YELLOW);  // Backspace
  drawButton(startX + 140, startY + 4 * (btnSize + 10), 60, btnSize, "OK", TFT_GREEN);
  drawButton(SCREEN_WIDTH - 70, 10, 60, 30, "Cancel", TFT_RED);

  while (true) {
    if (touch.touched()) {
      TS_Point p = touch.getPoint();
      int tx = map(p.x, 200, 3800, 0, SCREEN_WIDTH);
      int ty = map(p.y, 300, 3600, 0, SCREEN_HEIGHT);

      for (int row = 0; row < 4; row++) {
        for (int col = 0; col < strlen(keys[row]); col++) {
          int bx = startX + col * (btnSize + 2);
          int by = startY + row * (btnSize + 10);
          if (tx >= bx && tx <= bx + btnSize && ty >= by && ty <= by + btnSize) {
            input += keys[row][col];
            tft.fillRect(10, 40, 300, 20, TFT_BLACK);
            tft.setCursor(10, 40);
            tft.print(input);
            delay(200);
          }
        }
      }
      // Space
      if (tx >= startX && tx <= startX + 60 && ty >= startY + 4 * (btnSize + 10) && ty <= startY + 4 * (btnSize + 10) + btnSize) {
        input += " ";
        tft.fillRect(10, 40, 300, 20, TFT_BLACK);
        tft.setCursor(10, 40);
        tft.print(input);
        delay(200);
      }
      // Backspace
      if (tx >= startX + 70 && tx <= startX + 130 && ty >= startY + 4 * (btnSize + 10) && ty <= startY + 4 * (btnSize + 10) + btnSize) {
        if (input.length() > 0) input.remove(input.length() - 1);
        tft.fillRect(10, 40, 300, 20, TFT_BLACK);
        tft.setCursor(10, 40);
        tft.print(input);
        delay(200);
      }
      // OK
      if (tx >= startX + 140 && tx <= startX + 200 && ty >= startY + 4 * (btnSize + 10) && ty <= startY + 4 * (btnSize + 10) + btnSize) {
        return input;
      }
      // Cancel
      if (tx >= SCREEN_WIDTH - 70 && tx <= SCREEN_WIDTH - 10 && ty >= 10 && ty <= 40) {
        return currentVal;
      }
    }
    delay(50);
  }
}

void configureExtender() {
  tft.fillScreen(TFT_BLACK);
  drawTitleBar("Configure Extender");

  drawButton(20, 40, 280, 40, "Set Upstream SSID", TFT_GREEN);
  drawButton(20, 90, 280, 40, "Set Upstream Password", TFT_GREEN);
  drawButton(20, 140, 280, 40, isExtenderMode ? "Disable Extender" : "Enable Extender", TFT_YELLOW);
  drawButton(20, 190, 280, 40, "Back", TFT_RED);

  while (true) {
    if (isTouched(20, 40, 280, 40)) {
      String current = settings.containsKey("sta_ssid") ? settings["sta_ssid"].as<String>() : "";
      String newSSID = getTextInput("Enter Upstream SSID", current);
      if (newSSID != current) {
        settings["sta_ssid"] = newSSID;
        saveSettings();
        initWiFi();
      }
      configureExtender();  // Refresh
    } else if (isTouched(20, 90, 280, 40)) {
      String current = settings.containsKey("sta_pass") ? settings["sta_pass"].as<String>() : "";
      String newPass = getTextInput("Enter Upstream Password", current);
      if (newPass != current) {
        settings["sta_pass"] = newPass;
        saveSettings();
        initWiFi();
      }
      configureExtender();  // Refresh
    } else if (isTouched(20, 140, 280, 40)) {
      settings["extender_mode"] = !isExtenderMode;
      saveSettings();
      initWiFi();
      configureExtender();  // Refresh
    } else if (isTouched(20, 190, 280, 40)) {
      wifiMenu();
      return;
    }
    delay(50);
  }
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
  if (isExtenderMode) {
    tft.setCursor(10, 80);
    tft.print("Upstream: ");
    tft.print(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
  }

  drawButton(20, 110, 280, 40, "Change AP SSID", TFT_GREEN);
  drawButton(20, 160, 280, 40, "Configure Extender", TFT_GREEN);
  drawButton(20, 210, 280, 40, "Back", TFT_RED);

  while (true) {
    if (isTouched(20, 110, 280, 40)) {
      String current = settings.containsKey("wifi_ssid") ? settings["wifi_ssid"].as<String>() : "ESP32_Extender";
      String newSSID = getTextInput("Enter AP SSID", current);
      if (newSSID != current) {
        settings["wifi_ssid"] = newSSID;
        saveSettings();
        initWiFi();
        wifiMenu();  // Refresh
      }
    } else if (isTouched(20, 160, 280, 40)) {
      configureExtender();
    } else if (isTouched(20, 210, 280, 40)) {
      showMainMenu();
      return;
    }
    delay(50);
  }
}