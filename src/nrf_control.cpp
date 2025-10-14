#include "nrf_control.h"
#include "ui.h"
#include <RF24.h>
#include "config.h"
#include "settings.h"
#include "menu.h"

RF24 nrf1(NRF1_CE, NRF1_CSN);
RF24 nrf2(NRF2_CE, NRF2_CSN);

void initNRF() {
  SPI.begin();
  nrf1.begin();
  nrf2.begin();
  if (settings.containsKey("nrf_address")) {
    const char* addr = settings["nrf_address"];
    nrf1.openWritingPipe((uint8_t*)addr);
    nrf2.openReadingPipe(1, (uint8_t*)addr);
  } else {
    const uint8_t addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    nrf1.openWritingPipe(addr);
    nrf2.openReadingPipe(1, addr);
  }
}

void nrfMenu() {
  tft.fillScreen(TFT_BLACK);
  drawTitleBar("NRF Control");

  drawButton(20, 40, 280, 40, "Send Test", TFT_GREEN);
  drawButton(20, 90, 280, 40, "Receive Test", TFT_GREEN);
  drawButton(20, 140, 280, 40, "Back", TFT_RED);

  while (true) {
    if (isTouched(20, 40, 280, 40)) {
      const char* msg = "Hello NRF";
      nrf1.write(msg, strlen(msg));
      tft.setCursor(10, 190);
      tft.print("Sent!");
      delay(1000);
      tft.fillRect(10, 190, 300, 20, TFT_BLACK);  // Clear message
    } else if (isTouched(20, 90, 280, 40)) {
      if (nrf2.available()) {
        char buf[32] = {0};
        nrf2.read(buf, 32);
        tft.setCursor(10, 190);
        tft.print("Received: ");
        tft.print(buf);
      }
      delay(1000);
      tft.fillRect(10, 190, 300, 20, TFT_BLACK);
    } else if (isTouched(20, 140, 280, 40)) {
      showMainMenu();
      return;
    }
    delay(50);
  }
}