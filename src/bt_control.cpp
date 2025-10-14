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
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("Bluetooth Control");

  drawButton(20, 50, 280, 40, "Send Test", TFT_GREEN);
  drawButton(20, 100, 280, 40, "Receive Test", TFT_GREEN);
  drawButton(20, 150, 280, 40, "Back", TFT_RED);

  while (true) {
    if (isTouched(20, 50, 280, 40)) {
      SerialBT.println("Hello BT");
      tft.setCursor(10, 200);
      tft.print("Sent!");
      delay(1000);
    } else if (isTouched(20, 100, 280, 40)) {
      if (SerialBT.available()) {
        String msg = SerialBT.readString();
        tft.setCursor(10, 200);
        tft.print("Received: ");
        tft.print(msg);
      }
      delay(1000);
    } else if (isTouched(20, 150, 280, 40)) {
      showMainMenu();
      return;
    }
  }
}