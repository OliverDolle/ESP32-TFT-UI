#include "cc1101_control.h"
#include "ui.h"
#include "menu.h"
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include "config.h"
#include "settings.h"

unsigned long lastCC1101MessageTime = 0;
int cc1101ContinuousMode = 0;
float cc1101Frequency = 433.92; // Default frequency in MHz

void initCC1101() {
  // Initialize CC1101 with custom pins
  ELECHOUSE_cc1101.setSpiPin(SPI_SCLK, SPI_MISO, SPI_MOSI, CC1101_CSN_PIN);
  
  if (ELECHOUSE_cc1101.getCC1101()) {
    Serial.println("CC1101 connected");
  } else {
    Serial.println("CC1101 not found!");
    return;
  }
  
  // Load frequency from settings (default to 433.92 MHz)
  if (settings.containsKey("cc1101_frequency")) {
    cc1101Frequency = settings["cc1101_frequency"].as<float>();
  } else {
    settings["cc1101_frequency"] = 433.92;
    saveSettings();
  }
  
  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.setMHZ(cc1101Frequency);
  
  // Set modulation from settings (default to ASK/OOK)
  String modulation = settings.containsKey("cc1101_modulation") ? settings["cc1101_modulation"].as<String>() : "ASK";
  if (modulation == "2FSK") {
    ELECHOUSE_cc1101.setModulation(2);  // 2FSK
  } else if (modulation == "GFSK") {
    ELECHOUSE_cc1101.setModulation(1);  // GFSK
  } else if (modulation == "MSK") {
    ELECHOUSE_cc1101.setModulation(3);  // MSK
  } else {
    ELECHOUSE_cc1101.setModulation(0);  // ASK/OOK
  }
  
  // Set power level (default to max)
  int powerLevel = settings.containsKey("cc1101_power") ? settings["cc1101_power"].as<int>() : 10;
  ELECHOUSE_cc1101.setPA(powerLevel);
  
  ELECHOUSE_cc1101.SetRx();  // Start in RX mode
}

void sendCC1101Packet(const char* data, int length) {
  ELECHOUSE_cc1101.SetTx();
  ELECHOUSE_cc1101.SendData((byte*)data, length);
  delay(50);
  ELECHOUSE_cc1101.SetRx();
}

void sendCC1101Burst(int count) {
  const char* msg = "CC1101 Test";
  for (int i = 0; i < count; i++) {
    sendCC1101Packet(msg, strlen(msg));
    delay(10);
  }
}

void cc1101Menu() {
  tft.fillScreen(TFT_BLACK);
  drawTitleBar("CC1101 Control");

  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.print("Freq: ");
  tft.print(cc1101Frequency, 2);
  tft.print(" MHz");

  drawButton(20, 70, 280, 40, "Send Test (1x)", TFT_GREEN);
  drawButton(20, 120, 280, 40, "Send Burst (100x)", TFT_GREEN);
  drawButton(20, 170, 280, 40, "Continuous (ON/OFF)", TFT_ORANGE);
  drawButton(20, 220, 280, 40, "Set Frequency", TFT_CYAN);
  drawButton(20, 270, 280, 40, "Set Modulation", TFT_CYAN);
  drawButton(20, 320, 280, 40, "Back", TFT_RED);

  while (true) {
    if (cc1101ContinuousMode != 0) {
      const char* msg = "CC1101 Continuous";
      sendCC1101Packet(msg, strlen(msg));
    }

    if (isTouched(20, 70, 280, 40)) {
      const char* msg = "CC1101 Test";
      sendCC1101Packet(msg, strlen(msg));
      tft.setCursor(10, 360);
      tft.fillRect(10, 360, 300, 20, TFT_BLACK);
      tft.print("Sent 1 packet!");
      lastCC1101MessageTime = millis();
    } else if (isTouched(20, 120, 280, 40)) {
      sendCC1101Burst(100);
      tft.setCursor(10, 360);
      tft.fillRect(10, 360, 300, 20, TFT_BLACK);
      tft.print("Sent 100 packets!");
      lastCC1101MessageTime = millis();
    } else if (isTouched(20, 170, 280, 40)) {
      cc1101ContinuousMode = (cc1101ContinuousMode + 1) % 2;
      tft.setCursor(10, 360);
      tft.fillRect(10, 360, 300, 20, TFT_BLACK);
      if (cc1101ContinuousMode == 1) {
        tft.print("Continuous mode: ON");
      } else {
        tft.print("Continuous mode: OFF");
      }
      lastCC1101MessageTime = millis();
    } else if (isTouched(20, 220, 280, 40)) {
      // Set frequency submenu
      tft.fillScreen(TFT_BLACK);
      drawTitleBar("Set CC1101 Frequency");

      drawButton(20, 40, 280, 40, "315 MHz", TFT_CYAN);
      drawButton(20, 90, 280, 40, "433.92 MHz", TFT_CYAN);
      drawButton(20, 140, 280, 40, "868 MHz", TFT_CYAN);
      drawButton(20, 190, 280, 40, "915 MHz", TFT_CYAN);
      drawButton(20, 240, 280, 40, "Back", TFT_RED);

      while (true) {
        if (isTouched(20, 40, 280, 40)) {
          settings["cc1101_frequency"] = 315.00;
          saveSettings();
          initCC1101();
          cc1101Menu();
          return;
        } else if (isTouched(20, 90, 280, 40)) {
          settings["cc1101_frequency"] = 433.92;
          saveSettings();
          initCC1101();
          cc1101Menu();
          return;
        } else if (isTouched(20, 140, 280, 40)) {
          settings["cc1101_frequency"] = 868.00;
          saveSettings();
          initCC1101();
          cc1101Menu();
          return;
        } else if (isTouched(20, 190, 280, 40)) {
          settings["cc1101_frequency"] = 915.00;
          saveSettings();
          initCC1101();
          cc1101Menu();
          return;
        } else if (isTouched(20, 240, 280, 40)) {
          cc1101Menu();
          return;
        }
        delay(50);
      }
    } else if (isTouched(20, 270, 280, 40)) {
      // Set modulation submenu
      tft.fillScreen(TFT_BLACK);
      drawTitleBar("Set CC1101 Modulation");

      drawButton(20, 40, 280, 40, "ASK/OOK", TFT_CYAN);
      drawButton(20, 90, 280, 40, "2-FSK", TFT_CYAN);
      drawButton(20, 140, 280, 40, "GFSK", TFT_CYAN);
      drawButton(20, 190, 280, 40, "MSK", TFT_CYAN);
      drawButton(20, 240, 280, 40, "Back", TFT_RED);

      while (true) {
        if (isTouched(20, 40, 280, 40)) {
          settings["cc1101_modulation"] = "ASK";
          saveSettings();
          initCC1101();
          cc1101Menu();
          return;
        } else if (isTouched(20, 90, 280, 40)) {
          settings["cc1101_modulation"] = "2FSK";
          saveSettings();
          initCC1101();
          cc1101Menu();
          return;
        } else if (isTouched(20, 140, 280, 40)) {
          settings["cc1101_modulation"] = "GFSK";
          saveSettings();
          initCC1101();
          cc1101Menu();
          return;
        } else if (isTouched(20, 190, 280, 40)) {
          settings["cc1101_modulation"] = "MSK";
          saveSettings();
          initCC1101();
          cc1101Menu();
          return;
        } else if (isTouched(20, 240, 280, 40)) {
          cc1101Menu();
          return;
        }
        delay(50);
      }
    } else if (isTouched(20, 320, 280, 40)) {
      showMainMenu();
      return;
    }
    // Clear status after 2 seconds
    if (millis() - lastCC1101MessageTime > 2000) {
      tft.fillRect(10, 360, 300, 20, TFT_BLACK);
      lastCC1101MessageTime = millis();
    }
    delay(50);
  }
}
