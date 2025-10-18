#include "nrf_control.h"
#include "ui.h"
#include "menu.h"
#include <RF24.h>
#include "config.h"
#include "settings.h"

RF24 nrf1(NRF1_CE_PIN, NRF1_CSN_PIN);
RF24 nrf2(NRF2_CE_PIN, NRF2_CSN_PIN);

unsigned long lastMessageTime = 0;
int continuousMode = 0;
unsigned long lastSendTime = 0;
unsigned int channelNumber = 125;

void initNRF() {
  SPI.begin();
  nrf1.begin();
  nrf2.begin();

  // Set data rate from settings (default to 2 Mbps)
  String dataRate = settings.containsKey("nrf_data_rate") ? settings["nrf_data_rate"].as<String>() : "2MBPS";
  rf24_datarate_e rate;
  if (dataRate == "250KBPS") {
    rate = RF24_250KBPS;
  } else if (dataRate == "1MBPS") {
    rate = RF24_1MBPS;
  } else {
    rate = RF24_2MBPS;  // Highest rate
  }
  nrf1.setDataRate(rate);
  nrf2.setDataRate(rate);

  nrf1.setPALevel(RF24_PA_HIGH);
  nrf2.setPALevel(RF24_PA_HIGH);

  // Set addresses
  uint8_t addr1[5];
  uint8_t addr2[5];
  if (settings.containsKey("nrf_address1") && settings.containsKey("nrf_address2")) {
    const char* s1 = settings["nrf_address1"];
    const char* s2 = settings["nrf_address2"];
    memcpy(addr1, s1, 5);
    memcpy(addr2, s2, 5);
  } else {
    const uint8_t defaultAddr[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    const uint8_t defaultAddr2[5] = {0xD7, 0xD7, 0xD7, 0xD7, 0xD7};
    memcpy(addr1, defaultAddr, 5);
    memcpy(addr2, defaultAddr2, 5);
  }

  nrf1.openWritingPipe(addr1);
  nrf1.openReadingPipe(1, addr2);
  nrf2.openWritingPipe(addr2);
  nrf2.openReadingPipe(1, addr1);

  nrf1.startListening();
  nrf2.startListening();
}

void sendBurst(int count) {
  const char* msg = "Hello NRF";
  for (int i = 0; i < count; i++) {
    nrf1.write(msg, strlen(msg));
  }
}

void nrfMenu() {
  tft.fillScreen(TFT_BLACK);
  drawTitleBar("NRF Control");

  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.print("Data Rate: ");
  tft.print(settings.containsKey("nrf_data_rate") ? settings["nrf_data_rate"].as<String>() : "2MBPS");

  drawButton(20, 70, 280, 40, "Send Test (1x)", TFT_GREEN);
  drawButton(20, 120, 280, 40, "Send Burst (100x)", TFT_GREEN);
  drawButton(20, 170, 280, 40, "Continuous (ON/OFF)", TFT_ORANGE);
  drawButton(20, 220, 280, 40, "Receive Test", TFT_CYAN);
  drawButton(20, 270, 280, 40, "Set Data Rate", TFT_CYAN);
  drawButton(20, 320, 280, 40, "Back", TFT_RED);

  while (true) {
    if (continuousMode != 0) {
      nrf1.stopListening();
      nrf2.stopListening();
      for (int i = 0; i < channelNumber; i++) {
        nrf1.setChannel(i);
        nrf2.setChannel(i);
        const char* msg = "Hello NRF";
        nrf1.write(msg, strlen(msg));
        nrf2.write(msg, strlen(msg));
      }
      nrf1.startListening();
      nrf2.startListening();
    }

    if (isTouched(20, 70, 280, 40)) {
      nrf2.stopListening();
      const char* msg = "Hello NRF";
      nrf1.write(msg, strlen(msg));
      nrf2.startListening();
      tft.setCursor(10, 360);
      tft.fillRect(10, 360, 300, 20, TFT_BLACK);
      tft.print("Sent 1 message!");
      lastMessageTime = millis();
    } else if (isTouched(20, 120, 280, 40)) {
      nrf2.stopListening();
      sendBurst(100);
      nrf2.startListening();
      tft.setCursor(10, 360);
      tft.fillRect(10, 360, 300, 20, TFT_BLACK);
      tft.print("Sent 100 messages!");
      lastMessageTime = millis();
    } else if (isTouched(20, 170, 280, 40)) {
      continuousMode = (continuousMode + 1) % 3;
      tft.setCursor(10, 360);
      tft.fillRect(10, 360, 300, 20, TFT_BLACK);
      if (continuousMode == 1) {
        channelNumber = 126;
        tft.print("(All channels) Continuous: ON");
      } else if (continuousMode == 2) {
        channelNumber = 80;
        tft.print("(Wi-Fi & BL) Continuous: ON");
      } else {
        tft.print("Continuous mode: OFF");
      }
      lastMessageTime = millis();
    } else if (isTouched(20, 220, 280, 40)) {
      if (nrf2.available()) {
        char buf[32] = {0};
        nrf2.read(buf, sizeof(buf));
        tft.setCursor(10, 360);
        tft.fillRect(10, 360, 300, 20, TFT_BLACK);
        tft.print("Received: ");
        tft.print(buf);
        Serial.print("Received: ");
        Serial.println(buf);
      }
    } else if (isTouched(20, 270, 280, 40)) {
      tft.fillScreen(TFT_BLACK);
      drawTitleBar("Set NRF Data Rate");

      drawButton(20, 40, 280, 40, "250 kbps", TFT_CYAN);
      drawButton(20, 90, 280, 40, "1 Mbps", TFT_CYAN);
      drawButton(20, 140, 280, 40, "2 Mbps", TFT_CYAN);
      drawButton(20, 190, 280, 40, "Back", TFT_RED);

      while (true) {
        if (isTouched(20, 40, 280, 40)) {
          settings["nrf_data_rate"] = "250KBPS";
          saveSettings();
          initNRF();
          nrfMenu();
          return;
        } else if (isTouched(20, 90, 280, 40)) {
          settings["nrf_data_rate"] = "1MBPS";
          saveSettings();
          initNRF();
          nrfMenu();
          return;
        } else if (isTouched(20, 140, 280, 40)) {
          settings["nrf_data_rate"] = "2MBPS";
          saveSettings();
          initNRF();
          nrfMenu();
          return;
        } else if (isTouched(20, 190, 280, 40)) {
          nrfMenu();
          return;
        }
        delay(50);
      }
    } else if (isTouched(20, 320, 280, 40)) {
      showMainMenu();
      return;
    }
    // Clear status after 2 seconds
    if (millis() - lastMessageTime > 2000) {
      tft.fillRect(10, 360, 300, 20, TFT_BLACK);
      lastMessageTime = millis();
    }
    delay(50);
  }
}