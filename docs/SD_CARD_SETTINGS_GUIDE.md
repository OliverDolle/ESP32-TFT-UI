# SD Card Storage & Settings Guide

## Overview
Your ESP32 TFT UI system uses the **SD card slot on your TFT display module** to store all settings and images. The SD card is essential for the system to function properly.

## ✅ Yes, Settings ARE Stored on the SD Card!

### Settings File Location
- **File**: `/settings.json` (on SD card root)
- **Format**: JSON (human-readable text format)
- **Access**: Automatically loaded on boot, saved when changed

### What Gets Stored?
All your configurations are saved to the SD card:

```json
{
  "nrf_address1": "...",
  "nrf_address2": "...",
  "nrf_data_rate": "2MBPS",
  "cc1101_frequency": 433.92,
  "cc1101_modulation": "ASK",
  "cc1101_power": 10,
  "wifi_ssid": "ESP32_Extender",
  "wifi_pass": "password",
  "extender_mode": false,
  "sta_ssid": "",
  "sta_pass": "",
  "tft_cs": 5,
  "tft_dc": 17,
  "tft_rst": 16,
  "touch_cs": 21,
  "touch_irq": 22,
  "sd_cs": 4,
  "nrf1_ce": 25,
  "nrf1_csn": 26,
  "nrf2_ce": 27,
  "nrf2_csn": 32,
  "cc1101_csn": 33,
  "cc1101_gdo0": 14,
  "cc1101_gdo2": 12
}
```

### How Settings Persistence Works

1. **First Boot** (no SD card or no settings file):
   - System creates default settings
   - Saves them to `/settings.json` on SD card
   - Uses these defaults

2. **Subsequent Boots**:
   - Loads `/settings.json` from SD card
   - Applies all saved configurations
   - Your changes persist!

3. **When You Change Settings**:
   - Changes are immediately saved to SD card
   - If pin changes: device restarts to apply new pins
   - All other changes take effect immediately

## SD Card Setup

### SD Card Requirements
- **Format**: FAT32 (most important!)
- **Size**: Any size (even 1GB is more than enough)
- **Speed**: Class 4 or higher recommended

### How to Format SD Card (Windows)
1. Insert SD card into your computer
2. Right-click on the SD card drive
3. Select "Format..."
4. Choose:
   - File System: **FAT32** (NOT exFAT or NTFS!)
   - Allocation size: Default
   - Quick Format: ✓ Checked
5. Click "Start"

### Wiring for Your ILI9341 Display
Your 2.4" or 2.8" TFT display with ILI9341 chip typically has these connections:

```
TFT Module → ESP32
─────────────────────
VCC      → 3.3V or 5V (check your module spec)
GND      → GND
CS       → GPIO 5 (TFT_CS, configurable)
RESET    → GPIO 16 (TFT_RST, configurable)
DC/RS    → GPIO 17 (TFT_DC, configurable)
SDI/MOSI → GPIO 23 (shared SPI)
SCK      → GPIO 18 (shared SPI)
LED      → 3.3V (backlight)
SDO/MISO → GPIO 19 (shared SPI)

Touch Panel:
T_CLK    → GPIO 18 (shared with SCK)
T_CS     → GPIO 21 (TOUCH_CS, configurable)
T_DIN    → GPIO 23 (shared with MOSI)
T_DO     → GPIO 19 (shared with MISO)
T_IRQ    → GPIO 22 (TOUCH_IRQ, configurable)

SD Card Slot:
SD_CS    → GPIO 4 (SD_CS, configurable)
SD_MOSI  → GPIO 23 (shared SPI)
SD_MISO  → GPIO 19 (shared SPI)
SD_SCK   → GPIO 18 (shared SPI)
```

## Splash Screen / Startup Image

### ✅ Splash Image Already Implemented!

Your system **already supports** a custom splash screen that displays on startup!

### How It Works
- **File**: `/splash.bmp` on SD card root
- **Display Time**: 3 seconds on boot
- **Format**: 24-bit BMP (bitmap) image
- **Size**: **MUST be 320x240 pixels** (landscape) or 240x320 (portrait)

### Creating Your Splash Image

#### Method 1: Using Paint (Windows)
1. Create or find an image you like
2. Open in Paint
3. Resize: Home → Resize → **320 x 240 pixels** (uncheck "Maintain aspect ratio" if needed)
4. Save As → BMP (24-bit Bitmap)
5. Name it `splash.bmp`

#### Method 2: Using GIMP (Free, Cross-platform)
1. Open/create image in GIMP
2. Image → Scale Image → **320 x 240**
3. Image → Mode → RGB (ensure 24-bit)
4. File → Export As → `splash.bmp`
5. Options: 24-bit, no compression

#### Method 3: Using Online Tools
1. Go to: https://www.img2go.com/convert-to-bmp
2. Upload your image
3. Convert to BMP
4. Download and resize if needed
5. Rename to `splash.bmp`

### Installing the Splash Image
1. Format SD card as FAT32
2. Copy `splash.bmp` to the root of SD card
3. Insert SD card into TFT display module
4. Power on ESP32
5. Image will display for 3 seconds on boot!

### If Splash Doesn't Show
Check:
- ✅ File is named exactly `splash.bmp` (lowercase)
- ✅ File is in root of SD card (not in a folder)
- ✅ Image size is exactly 320x240 pixels
- ✅ Format is 24-bit BMP (not PNG, JPG, or 8-bit BMP)
- ✅ SD card is formatted as FAT32
- ✅ SD card is properly inserted

### Customizing Splash Duration
Currently hardcoded to 3 seconds. To change, edit `src/main.cpp`:
```cpp
drawBmp("/splash.bmp", 0, 0);
delay(3000);  // ← Change this value (milliseconds)
```

## Troubleshooting

### "SD init failed!" in Serial Monitor
**Problem**: SD card not detected
**Solutions**:
1. Check SD card is properly inserted
2. Verify SD_CS pin is correct (default GPIO 4)
3. Ensure SD card is formatted as FAT32
4. Check wiring to SD slot
5. Try a different SD card

### Settings Not Saving
**Problem**: Changes don't persist after reboot
**Solutions**:
1. Ensure SD card is present and working
2. Check serial monitor for "SD init failed"
3. Verify SD card is not write-protected
4. Try reformatting SD card as FAT32

### Splash Image Not Displaying
**Problem**: Blank screen on boot or error message
**Solutions**:
1. Verify file is exactly `splash.bmp`
2. Check image is 320x240 pixels
3. Ensure 24-bit BMP format (not 8-bit or compressed)
4. Verify SD card is readable
5. Check serial monitor for "BMP file not found" or "BMP size mismatch"

## Advanced: Multiple Splash Images

Want to customize the splash image through settings? Here's how to add that feature:

### Add to settings.json
```json
{
  "splash_file": "/splash.bmp"
}
```

### Modify src/main.cpp
Change this:
```cpp
drawBmp("/splash.bmp", 0, 0);
```

To this:
```cpp
String splashFile = settings.containsKey("splash_file") ? 
                    settings["splash_file"].as<String>() : "/splash.bmp";
drawBmp(splashFile.c_str(), 0, 0);
```

Now you can store multiple images (logo1.bmp, logo2.bmp) and switch between them!

## File Structure on SD Card

```
SD Card (FAT32)
├── settings.json      ← All your settings
├── splash.bmp         ← Startup image (320x240, 24-bit)
├── logo1.bmp          ← (Optional) Alternative images
├── logo2.bmp          ← (Optional)
└── custom/            ← (Optional) Organize in folders
    └── images/
        └── mylogo.bmp
```

## Settings Backup

### To Backup Your Settings
1. Remove SD card from display
2. Insert into computer
3. Copy `settings.json` to safe location
4. Keep backup for restoration

### To Restore Settings
1. Copy backed-up `settings.json` to SD card root
2. Insert SD card
3. Reboot ESP32
4. Settings restored!

## Tips & Best Practices

1. **Always Use FAT32**: Other formats (exFAT, NTFS) won't work!
2. **Keep SD Card In**: Settings save/load on every boot and change
3. **24-bit BMP Only**: PNG/JPG not supported for splash screen
4. **Exact Size**: Image must be 320x240 (or your screen size)
5. **Test on PC First**: Open settings.json on computer to verify it's valid JSON
6. **Backup Regularly**: Copy settings.json when you have a good configuration

## Summary

✅ **Settings ARE stored on SD card** (`/settings.json`)  
✅ **Splash screen IS already implemented** (`/splash.bmp`)  
✅ **All changes persist across reboots**  
✅ **SD card MUST be FAT32 format**  
✅ **Image must be 320x240 pixels, 24-bit BMP**  

Your system is already fully set up for persistent storage and custom splash screens!
