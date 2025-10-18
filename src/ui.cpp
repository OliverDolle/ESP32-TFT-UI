#include "ui.h"
#include <SD.h>
#include "config.h"  // For screen dims
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

void drawTitleBar(const char* title) {
  tft.fillRect(0, 0, SCREEN_WIDTH, 30, TFT_NAVY);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(10, 5);
  tft.print(title);
}

uint8_t getRed(uint16_t color) {
  return ((color >> 11) & 0x1F) << 3;  // 5-bit to 8-bit
}

uint8_t getGreen(uint16_t color) {
  return ((color >> 5) & 0x3F) << 2;  // 6-bit to 8-bit
}

uint8_t getBlue(uint16_t color) {
  return (color & 0x1F) << 3;  // 5-bit to 8-bit
}

int getNumberInput(const char* prompt, int currentVal) {
  tft.fillScreen(TFT_BLACK);
  drawTitleBar(prompt);
  String input = String(currentVal);
  tft.setTextSize(4);
  tft.setCursor(10, 40);
  tft.print(input);

  // Draw numpad
  int btnSize = 60;
  int startX = 20, startY = 80;
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 3; col++) {
      int num = (3 - row) * 3 + col + 1;
      if (row == 3) {
        if (col == 0) num = 0;  // 0
        else continue;  // Skip for backspace/OK
      }
      drawButton(startX + col * (btnSize + 10), startY + row * (btnSize + 10), btnSize, btnSize, String(num).c_str(), TFT_CYAN);
    }
  }
  // Backspace
  drawButton(startX + 1 * (btnSize + 10), startY + 3 * (btnSize + 10), btnSize, btnSize, "<", TFT_YELLOW);
  // OK
  drawButton(startX + 2 * (btnSize + 10), startY + 3 * (btnSize + 10), btnSize, btnSize, "OK", TFT_GREEN);
  // Cancel
  drawButton(SCREEN_WIDTH - 70, 10, 60, 30, "Cancel", TFT_RED);

  while (true) {
    if (touch.touched()) {
      TS_Point p = touch.getPoint();
      int tx = map(p.x, 200, 3800, 0, SCREEN_WIDTH);
      int ty = map(p.y, 300, 3600, 0, SCREEN_HEIGHT);

      // Check numpad buttons
      for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {
          int bx = startX + col * (btnSize + 10);
          int by = startY + row * (btnSize + 10);
          if (tx >= bx && tx <= bx + btnSize && ty >= by && ty <= by + btnSize) {
            if (row == 3 && col == 1) {  // Backspace
              if (input.length() > 0) input.remove(input.length() - 1);
            } else if (row == 3 && col == 2) {  // OK
              int val = input.toInt();
              if (val >= 0 && val <= 39) return val;
              else {
                tft.setCursor(10, 200);
                tft.print("Invalid GPIO (0-39)");
                delay(2000);
                tft.fillRect(10, 200, 300, 20, TFT_BLACK);
              }
              return -1;  // Invalid, but handled in caller
            } else {
              int num = (row < 3) ? (3 - row) * 3 + col + 1 : 0;
              input += String(num);
            }
            tft.fillRect(10, 40, 300, 30, TFT_BLACK);
            tft.setCursor(10, 40);
            tft.print(input);
            delay(200);  // Debounce
          }
        }
      }
      // Cancel
      if (tx >= SCREEN_WIDTH - 70 && tx <= SCREEN_WIDTH - 10 && ty >= 10 && ty <= 40) {
        return currentVal;  // No change
      }
    }
    delay(50);
  }
}

void drawButton(int x, int y, int w, int h, const char* label, uint16_t color) {
  // Gradient fill: from color to darker
  uint8_t r = getRed(color) / 2;
  uint8_t g = getGreen(color) / 2;
  uint8_t b = getBlue(color) / 2;
  uint16_t darkColor = tft.color565(r, g, b);

  for (int i = 0; i < h; i++) {
    uint16_t gradColor = tft.alphaBlend(i * 255 / h, color, darkColor);
    tft.drawFastHLine(x, y + i, w, gradColor);
  }
  tft.drawRoundRect(x, y, w, h, 10, TFT_WHITE);
  // Center text
  tft.setTextSize(2);
  int textWidth = strlen(label) * 12;  // Approx for size 2
  tft.setCursor(x + (w - textWidth)/2, y + (h/2) - 8);
  tft.print(label);
}

bool isTouched(int x, int y, int w, int h, bool withFeedback) {
  if (touch.touched()) {
    TS_Point p = touch.getPoint();
    int tx = map(p.x, 200, 3800, 0, SCREEN_WIDTH);
    int ty = map(p.y, 300, 3600, 0, SCREEN_HEIGHT);
    if (tx >= x && tx <= x + w && ty >= y && ty <= y + h) {
      if (withFeedback) {
        // Invert for feedback
        tft.invertDisplay(true);
        delay(100);
        tft.invertDisplay(false);
      }
      return true;
    }
  }
  return false;
}

// Custom BMP drawer (24-bit only, handles bottom-up)
void drawBmp(const char* filename, int16_t x, int16_t y) {
  File bmpFile = SD.open(filename);
  if (!bmpFile) {
    Serial.println("BMP file not found");
    return;
  }

  // Read BMP header (assume valid 24-bit BMP)
  bmpFile.seek(10);
  uint32_t dataOffset;
  bmpFile.read((uint8_t*)&dataOffset, 4);

  bmpFile.seek(18);
  int32_t width;
  bmpFile.read((uint8_t*)&width, 4);
  int32_t height;
  bmpFile.read((uint8_t*)&height, 4);

  if (width != SCREEN_WIDTH || abs(height) != SCREEN_HEIGHT) {
    Serial.println("BMP size mismatch");
    bmpFile.close();
    return;
  }

  bool flip = (height > 0);  // Positive height means bottom-up
  height = abs(height);

  bmpFile.seek(dataOffset);

  for (int32_t row = 0; row < height; row++) {
    int32_t yPos = flip ? (height - 1 - row) : row;
    for (int32_t col = 0; col < width; col++) {
      uint8_t b = bmpFile.read();
      uint8_t g = bmpFile.read();
      uint8_t r = bmpFile.read();
      tft.drawPixel(x + col, y + yPos, tft.color565(r, g, b));
    }
    // Padding to 4-byte boundary
    while ((width * 3) % 4 != 0) bmpFile.read();
  }

  bmpFile.close();
}