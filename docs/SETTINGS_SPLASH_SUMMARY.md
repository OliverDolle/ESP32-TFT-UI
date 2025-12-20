# Settings & Splash Screen - Implementation Summary

## ✅ Your Questions Answered

### Q: Is the settings stored on the SD card?
**A: YES!** All settings are stored in `/settings.json` on the SD card slot of your TFT display module.

### Q: How do I make sure settings are saved?
**A: Automatic!** The system automatically:
- Loads settings on boot from SD card
- Saves settings immediately when you change anything
- Persists all changes across reboots

### Q: Can I save a picture for startup?
**A: YES!** Already implemented + enhanced with new features!

---

## 🎯 What Was Already Working

Your system already had:
- ✅ SD card storage for settings (`/settings.json`)
- ✅ Splash screen support (`/splash.bmp`)
- ✅ 3-second startup display
- ✅ All settings persist across reboots

---

## 🚀 New Features Added

### 1. **Enhanced Splash Screen System**
- **Customizable duration** (1-10 seconds via settings)
- **Customizable filename** (use different images)
- **Fallback display** (shows default screen if image missing)
- **Test function** (preview splash without rebooting)
- **Settings stored** in `settings.json`

### 2. **Splash Screen Configuration Menu**
New menu: **Settings → Splash Screen**

Options:
- **Set Duration (1-10s)**: Change how long splash displays
- **Default (/splash.bmp)**: Reset to default filename
- **Custom Filename**: Use different BMP files
- **Test Splash Now**: Preview immediately
- **Back**: Return to settings

### 3. **Better Error Handling**
- System checks if splash file exists
- Shows default screen if file missing
- No more blank screens or crashes
- Helpful error message if no splash.bmp

---

## 📁 Files Modified

### Updated Files:
| File | Changes |
|------|---------|
| `src/main.cpp` | Enhanced splash loading with fallback |
| `src/settings.cpp` | Added splash_file and splash_duration |
| `src/menu.cpp` | Added splashConfigMenu() function |
| `include/menu.h` | Added splashConfigMenu() declaration |

### New Documentation:
| File | Purpose |
|------|---------|
| `docs/SD_CARD_SETTINGS_GUIDE.md` | Complete SD card & settings guide |
| `docs/SPLASH_IMAGE_GUIDE.md` | How to create splash images |

---

## 💾 SD Card Setup

### What You Need:
- **SD card** (any size, even 1GB works)
- **Format**: FAT32 (critical!)
- **Files to add**:
  - `splash.bmp` - Your startup image (optional)

### Format SD Card (Windows):
1. Insert SD card into computer
2. Right-click drive → Format
3. Choose **FAT32** file system
4. Quick Format → Start

### Add Splash Image:
1. Create 320x240 pixel image
2. Save as 24-bit BMP
3. Name it `splash.bmp`
4. Copy to SD card root (not in folder!)

---

## 🎨 Creating Splash Images

### Quick Methods:

#### Windows Paint (Easiest):
```
1. Open image in Paint
2. Resize → 320 x 240 pixels
3. Save As → BMP
4. Name: splash.bmp
5. Copy to SD card
```

#### Online Tool (No software):
```
1. Go to: img2go.com/resize-image
2. Upload image
3. Resize to 320 x 240
4. Convert to BMP
5. Download and copy to SD card
```

### Requirements:
- **Size**: Exactly 320 x 240 pixels
- **Format**: 24-bit BMP (not PNG/JPG)
- **Filename**: `splash.bmp` (or custom via settings)

---

## 🔧 New Settings Available

### In settings.json:
```json
{
  "splash_file": "/splash.bmp",
  "splash_duration": 3000,
  ...other settings...
}
```

### Configure via UI:
**Path**: Main Menu → Settings → Splash Screen

**Options**:
- **Duration**: 1-10 seconds (default: 3)
- **Filename**: Any .bmp file on SD card
- **Test**: Preview without reboot!

---

## 📋 How It Works

### On Boot:
```
1. Load settings from SD card (/settings.json)
2. Check if splash file exists
3. If YES → Display splash for configured duration
4. If NO → Show default "ESP32 TFT UI Initializing..." screen
5. Continue to main menu
```

### Settings Persistence:
```
1. User changes setting via UI
2. System saves to /settings.json immediately
3. SD card stores the change
4. Next boot: Setting is loaded and applied
```

### Multiple Splash Images:
```
Store on SD card:
  - splash.bmp     ← Default
  - logo1.bmp      ← Alternative
  - startup.bmp    ← Alternative
  - custom.bmp     ← Alternative

Then use Settings → Splash Screen → Custom Filename
to switch between them!
```

---

## 🧪 Testing

### Test Splash Screen:
1. Main Menu → Settings → Splash Screen
2. Click "Test Splash Now"
3. Image displays immediately
4. Returns to menu after duration

### Test Settings Persistence:
1. Change any setting (e.g., splash duration to 5s)
2. Reboot ESP32
3. Check if setting persisted (should display 5s)

### Test Without Splash Image:
1. Remove splash.bmp from SD card
2. Reboot ESP32
3. Should show default "Initializing..." screen
4. No errors or crashes

---

## 🐛 Troubleshooting

### Splash Not Showing:
- ✅ Check SD card inserted and formatted FAT32
- ✅ Verify `splash.bmp` is in root (not folder)
- ✅ Ensure image is 320x240 pixels exactly
- ✅ Confirm 24-bit BMP format (not PNG/JPG)

### Settings Not Saving:
- ✅ Ensure SD card is working (check Serial Monitor)
- ✅ Verify SD card not write-protected
- ✅ Check "SD init failed!" message in serial
- ✅ Try different SD card

### "BMP size mismatch" Error:
- Image must be exactly 320x240 pixels
- Resize using Paint or GIMP

### Default Screen Shows:
- This is normal if no splash.bmp exists
- Add splash.bmp to show custom image
- System works fine either way!

---

## 📊 System Architecture

### Storage Flow:
```
ESP32 Boot
    ↓
Load /settings.json from SD card
    ↓
Apply all settings (pins, splash config, etc.)
    ↓
Check if splash file exists
    ↓
Display splash OR default screen
    ↓
Initialize modules (NRF, CC1101, WiFi, BT)
    ↓
Show Main Menu
```

### Settings Update Flow:
```
User changes setting via UI
    ↓
settings[key] = newValue
    ↓
saveSettings() called
    ↓
Write to /settings.json on SD card
    ↓
If pin changed → ESP.restart()
    ↓
Otherwise → Continue normally
```

---

## 🎯 Key Points

1. **SD Card is Required**: System needs SD card for settings storage
2. **FAT32 Only**: Other formats won't work
3. **Automatic Saves**: All changes saved immediately
4. **Splash is Optional**: System works fine without splash.bmp
5. **Customizable**: Duration, filename, all configurable via UI
6. **Test Function**: Preview splash without reboot
7. **Fallback Safe**: Shows default screen if image missing

---

## 🚦 Quick Start Guide

### First Time Setup:
1. **Format SD card** → FAT32
2. **Create splash image** → 320x240 BMP
3. **Name it** → `splash.bmp`
4. **Copy to SD card root**
5. **Insert SD card** → Into TFT module
6. **Power on ESP32**
7. **Enjoy custom splash!**

### Changing Splash Later:
1. **Main Menu** → Settings → Splash Screen
2. **Set Duration** → 1-10 seconds
3. **Custom Filename** → Use different image
4. **Test Splash Now** → Preview it!

### Backup Settings:
1. Remove SD card
2. Copy `settings.json` to computer
3. Keep as backup
4. Restore by copying back to SD card

---

## 📖 Documentation Files

| Document | Purpose |
|----------|---------|
| `SD_CARD_SETTINGS_GUIDE.md` | Complete guide to SD card, settings, wiring |
| `SPLASH_IMAGE_GUIDE.md` | Step-by-step splash image creation |
| `CC1101_INTEGRATION.md` | CC1101 module integration details |

---

## ✨ Summary

Your system now has:
- ✅ **Persistent settings** on SD card
- ✅ **Customizable splash screen** with test function
- ✅ **Multiple image support** (switch between files)
- ✅ **Configurable duration** (1-10 seconds)
- ✅ **Fallback protection** (default screen if image missing)
- ✅ **Easy UI configuration** (no code editing needed!)

Everything is saved automatically and persists across reboots!
