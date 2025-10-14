#include "ui.h"

void initDisplay() {
  tft.begin();
  tft.setRotation(1);  // Landscape
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
}

void initTouch() {
  touch.begin();
}

void drawButton(int x, int y, int w, int h, const char* label, uint16_t color) {
  tft.fillRoundRect(x, y, w, h, 10, color);
  tft.drawRoundRect(x, y, w, h, 10, TFT_WHITE);
  tft.setCursor(x + 10, y + (h/2) - 8);
  tft.print(label);
}

bool isTouched(int x, int y, int w, int h) {
  if (touch.touched()) {
    TS_Point p = touch.getPoint();
    int tx = map(p.x, 200, 3800, 0, SCREEN_WIDTH);
    int ty = map(p.y, 300, 3600, 0, SCREEN_HEIGHT);
    return (tx >= x && tx <= x + w && ty >= y && ty <= y + h);
  }
  return false;
}