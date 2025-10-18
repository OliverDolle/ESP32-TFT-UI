#ifndef UI_H
#define UI_H

#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "config.h"

extern TFT_eSPI tft;
extern XPT2046_Touchscreen touch;

void initDisplay();
void initTouch();
void drawButton(int x, int y, int w, int h, const char* label, uint16_t color);
bool isTouched(int x, int y, int w, int h, bool withFeedback = true);
void drawBmp(const char* filename, int16_t x, int16_t y);
void drawTitleBar(const char* title);

// Color helpers
uint8_t getRed(uint16_t color);
uint8_t getGreen(uint16_t color);
uint8_t getBlue(uint16_t color);

// Input functions
int getNumberInput(const char* prompt, int currentVal);
String getTextInput(const char* prompt, String currentVal);

#endif