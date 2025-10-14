#include "bt_control.h"
#include "ui.h"
#include <BluetoothSerial.h>
#include "config.h"
#include "menu.h"

BluetoothSerial SerialBT;

void initBT() {
  SerialBT.begin("ESP32_BT");
}

void btMenu() {
  tft.fillScreen(TFT_BLACK);
  drawTitleBar("Bluetooth Control");

  drawButton(20, 40, 280, 40, "Send Test", TFT_GREEN);
  drawButton(20, 90, 280, 40, "Receive Test", TFT_GREEN);
  drawButton(20, 140, 280, 40, "Back", TFT_RED);

  while (true) {
    if (isTouched(20, 40, 280, 40)) {
      SerialBT.println("Hello BT");
      tft.setCursor(10, 190);
      tft.print("Sent!");
      delay(1000);
      tft.fillRect(10, 190, 300, 20, TFT_BLACK);
    } else if (isTouched(20, 90, 280, 40)) {
      if (SerialBT.available()) {
        String msg = SerialBT.readString();
        tft.setCursor(10, 190);
        tft.print("Received: ");
        tft.print(msg);
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