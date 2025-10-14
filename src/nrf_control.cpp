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
  // Load settings from JSON, e.g. addresses
  if (settings.containsKey("nrf_address")) {
    const char* addr = settings["nrf_address"];
    nrf1.openWritingPipe((uint8_t*)addr);
    nrf2.openReadingPipe(1, (uint8_t*)addr);
  } else {
    // Default
    const uint8_t addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    nrf1.openWritingPipe(addr);
    nrf2.openReadingPipe(1, addr);
  }
}

void nrfMenu() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("NRF Control");

  drawButton(20, 50, 280, 40, "Send Test", TFT_GREEN);
  drawButton(20, 100, 280, 40, "Receive Test", TFT_GREEN);
  drawButton(20, 150, 280, 40, "Back", TFT_RED);

  while (true) {
    if (isTouched(20, 50, 280, 40)) {
      // Send test via nrf1
      const char* msg = "Hello NRF";
      nrf1.write(msg, strlen(msg));
      tft.setCursor(10, 200);
      tft.print("Sent!");
      delay(1000);
    } else if (isTouched(20, 100, 280, 40)) {
      // Receive test via nrf2
      if (nrf2.available()) {
        char buf[32] = {0};
        nrf2.read(buf, 32);
        tft.setCursor(10, 200);
        tft.print("Received: ");
        tft.print(buf);
      }
      delay(1000);
    } else if (isTouched(20, 150, 280, 40)) {
      showMainMenu();
      return;
    }
  }
}