# UI Documentation

## Overview
Handles display, touch, and graphics. Includes custom BMP rendering for splash screens.

## Complex Functions
- `drawBmp(filename, x, y)`: Loads and draws a 24-bit BMP from SD. Handles bottom-up storage. Assumes image matches screen size (320x240).
- `drawButton(x, y, w, h, label, color)`: Draws gradient-filled rounded button with centered text.
- `isTouched(x, y, w, h, withFeedback)`: Checks touch; adds invert feedback if enabled.

## Adding Functionality
To add icons: Draw simple shapes (e.g., tft.fillCircle) next to buttons.
For advanced fonts: Enable in TFT_eSPI setup and use tft.setFreeFont().

## Notes
- BMP Preparation: Use 24-bit color, no compression. If image appears flipped, invert in editor (e.g., GIMP: Image > Flip Vertical).
- Performance: BMP loading is pixel-by-pixel; for faster, consider JPEG with Bodmer's JPEGDecoder library (add to platformio.ini).