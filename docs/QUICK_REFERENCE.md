# Quick Reference Card - SD Card & Splash Screen

## ✅ YES - Settings ARE Stored on SD Card!

### Location: `/settings.json` on SD card root

---

## 💾 SD Card Setup

```
Required Format: FAT32
Recommended Size: 1GB or larger
Location: SD card slot on TFT display module
```

### Format on Windows:
```
Right-click SD drive → Format → FAT32 → Start
```

---

## 🎨 Splash Screen Quick Setup

### Image Requirements:
```
Size:     320 x 240 pixels
Format:   24-bit BMP
Filename: splash.bmp
Location: Root of SD card
```

### Create in Paint (1 minute):
```
1. Open image
2. Resize → 320 x 240
3. Save As → BMP
4. Name: splash.bmp
5. Copy to SD card root
```

---

## 🔧 Customize via UI

### Path: Main Menu → Settings → Splash Screen

```
┌─────────────────────────────────┐
│  Set Duration (1-10s)           │  ← Change display time
├─────────────────────────────────┤
│  Default (/splash.bmp)          │  ← Reset to default
├─────────────────────────────────┤
│  Custom Filename                │  ← Use logo1.bmp, etc.
├─────────────────────────────────┤
│  Test Splash Now                │  ← Preview immediately!
├─────────────────────────────────┤
│  Back                           │
└─────────────────────────────────┘
```

---

## 📁 SD Card File Structure

```
SD Card (FAT32)
│
├── settings.json      ← All your settings (auto-created)
│
├── splash.bmp         ← Your startup image (optional)
│
├── logo1.bmp          ← Alternative images (optional)
├── logo2.bmp          ← (optional)
└── custom.bmp         ← (optional)
```

---

## ⚙️ Default Settings

```json
{
  "splash_file": "/splash.bmp",
  "splash_duration": 3000,
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
  "cc1101_gdo2": 12,
  "cc1101_frequency": 433.92,
  "nrf_data_rate": "2MBPS"
}
```

---

## 🔌 TFT Display Wiring

```
ILI9341 TFT → ESP32
─────────────────────
VCC     → 3.3V/5V
GND     → GND
CS      → GPIO 5    (configurable)
RESET   → GPIO 16   (configurable)
DC      → GPIO 17   (configurable)
MOSI    → GPIO 23   (fixed SPI)
SCK     → GPIO 18   (fixed SPI)
MISO    → GPIO 19   (fixed SPI)
T_CS    → GPIO 21   (configurable)
T_IRQ   → GPIO 22   (configurable)
SD_CS   → GPIO 4    (configurable)
```

---

## 🚨 Troubleshooting

### No Splash Display?
```
✅ Check: splash.bmp in SD card root
✅ Check: Image is 320x240 pixels
✅ Check: 24-bit BMP format
✅ Check: SD card formatted FAT32
```

### Settings Not Saving?
```
✅ Check: SD card inserted
✅ Check: SD card not write-protected
✅ Check: Serial monitor for "SD init failed!"
✅ Try: Different SD card
```

### SD Init Failed?
```
✅ Check: SD_CS pin wiring (default GPIO 4)
✅ Check: SD card formatted FAT32
✅ Try: Reseat SD card
✅ Try: Different SD card
```

---

## 💡 Pro Tips

### Multiple Splash Images:
```
1. Create: logo1.bmp, logo2.bmp, startup.bmp
2. Copy all to SD card root
3. Settings → Splash Screen → Custom Filename
4. Enter: logo1 (system adds .bmp)
5. Test it!
```

### Test Before Reboot:
```
Settings → Splash Screen → Test Splash Now
No reboot needed!
```

### Backup Settings:
```
1. Remove SD card
2. Copy settings.json to PC
3. Keep safe!
4. Restore anytime
```

### Change Display Duration:
```
Settings → Splash Screen → Set Duration
Range: 1-10 seconds
Default: 3 seconds
```

---

## 📊 What Gets Saved?

### Automatically Saved:
- ✅ All pin configurations
- ✅ NRF settings (addresses, data rate)
- ✅ CC1101 settings (frequency, modulation, power)
- ✅ WiFi credentials
- ✅ Splash screen preferences
- ✅ All UI changes

### When Saved:
- ✅ Immediately when you change any setting
- ✅ No manual save button needed
- ✅ Persists across power cycles
- ✅ Survives ESP32 resets

---

## 🎯 Quick Start Checklist

```
□ Format SD card as FAT32
□ Create 320x240 BMP image
□ Name it splash.bmp
□ Copy to SD card root
□ Insert SD card into TFT module
□ Power on ESP32
□ See your splash screen!
□ Customize via Settings menu
```

---

## 📞 Support

### Check Serial Monitor:
```
115200 baud
Shows: SD status, file loading, errors
```

### Common Messages:
```
✅ "SD init failed!" → SD card issue
✅ "BMP file not found" → Missing splash.bmp (OK!)
✅ "BMP size mismatch" → Wrong image size
✅ "JSON parse failed" → Corrupted settings.json
```

---

## 🌟 System Features

```
✅ Persistent storage via SD card
✅ Automatic settings save
✅ Custom splash screens
✅ Multiple image support
✅ Configurable display duration
✅ Live preview function
✅ Fallback protection
✅ Pin reconfiguration
✅ No code editing needed!
```

---

**Remember**: The splash image is optional. System works perfectly without it, showing a default startup screen instead!
