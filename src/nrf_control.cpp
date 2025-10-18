#include "nrf_control.h"
#include "ui.h"
#include <RF24.h>
#include "config.h"
#include "settings.h"
#include "menu.h"

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

  nrf1.setPALevel(RF24_PA_HIGH);   
  nrf2.setPALevel(RF24_PA_HIGH);  
  nrf1.setDataRate(RF24_2MBPS);    
  nrf2.setDataRate(RF24_2MBPS);

  uint8_t addr1[5];
  uint8_t addr2[5];

  if (settings.containsKey("nrf_address1") && settings.containsKey("nrf_address2")) {
    //Copy from settings
    const char* s1 = settings["nrf_address1"];
    const char* s2 = settings["nrf_address2"];
    memcpy(addr1, s1, 5);  
    memcpy(addr2, s2, 5);
  } else {
    //Fallback
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

  drawButton(20, 40, 280, 40, "Send Test (1x)", TFT_GREEN);
  drawButton(20, 90, 280, 40, "Send Burst (100x)", TFT_GREEN);
  drawButton(20, 140, 280, 40, "Continuous Burst (ON/OFF)", TFT_ORANGE);
  drawButton(20, 190, 280, 40, "Receive Test", TFT_CYAN);
  drawButton(20, 240, 280, 40, "Back", TFT_RED);

  while (true) {

    if (continuousMode != 0) {
      nrf1.stopListening();
      nrf2.stopListening();
      for(int i = 0; i < channelNumber; i++) {
        nrf1.setChannel(i);
        nrf2.setChannel(i);
        const char* msg = "Hello NRF";
        nrf1.write(msg, strlen(msg));
        nrf2.write(msg, strlen(msg));
      }
      nrf1.startListening();  
      nrf2.startListening();  
      
    }

    
    if (isTouched(20, 40, 280, 40)) {
      nrf2.stopListening();
      const char* msg = "Hello NRF";
      nrf1.write(msg, strlen(msg));
      nrf2.startListening();
      tft.setCursor(10, 280);
      tft.print("Sent 1 message!");
      lastMessageTime = millis();
    }

    else if (isTouched(20, 90, 280, 40)) {
      nrf2.stopListening();
      sendBurst(100);
      nrf2.startListening();
      tft.setCursor(10, 280);
      tft.print("Sent 100 messages!");
      lastMessageTime = millis();
    }

    else if (isTouched(20, 140, 280, 40)) {
      continuousMode = (continuousMode + 1) % 3;  
      tft.setCursor(10, 280);
      if (continuousMode == 1) {
        channelNumber = 126;
        tft.print("(All channels) Continuous mode: ON");
      } else if (continuousMode == 2) {
        channelNumber = 80;
        tft.print("(Wi-Fi & BL) Continuous mode: ON");
      } else {
        tft.print("Continuous mode: OFF");
      }
      lastMessageTime = millis();
    }

    else if (isTouched(20, 190, 280, 40)) {
      if (nrf2.available()) {
        char buf[32] = {0};
        nrf2.read(buf, sizeof(buf));
        tft.setCursor(10, 280);
        tft.fillRect(10, 280, 300, 20, TFT_BLACK);
        tft.print("Received: ");
        tft.print(buf);
        Serial.print("Received: ");
        Serial.println(buf);
      }
    }

    else if (isTouched(20, 240, 280, 40)) {
      showMainMenu();
      return;
    }
  }
}
