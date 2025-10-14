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
bool isTouched(int x, int y, int w, int h);

#endif