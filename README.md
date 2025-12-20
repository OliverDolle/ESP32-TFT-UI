# ESP32 TFT UI Control System

A feature-rich touchscreen UI system for ESP32 with RF module control (NRF24L01 & CC1101), WiFi management, and fully customizable settings.

[![Platform](https://img.shields.io/badge/platform-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Framework](https://img.shields.io/badge/framework-Arduino-00979D.svg)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

## ✨ Features

### 🖥️ Display & UI
- **320x240 TFT touchscreen** (ILI9341 driver)
- Touch-based navigation with visual feedback
- Custom splash screen support
- Configurable startup duration
- Live preview of splash images

### 📡 RF Modules
- **NRF24L01** dual module support
  - Send/receive packets
  - Burst transmission (100 packets)
  - Continuous transmission mode
  - Configurable data rates (250kbps, 1Mbps, 2Mbps)
  - Custom addresses

- **CC1101** sub-GHz transceiver
  - Multiple frequencies (315, 433.92, 868, 915 MHz)
  - 4 modulation modes (ASK/OOK, 2-FSK, GFSK, MSK)
  - Adjustable power levels
  - Burst & continuous modes

### 🌐 Connectivity
- **WiFi Hotspot** management
- **Bluetooth** control
- Web interface capable

### ⚙️ Settings Management
- **All settings stored on SD card** (`/settings.json`)
- **13 configurable GPIO pins** via UI
- No code changes needed for customization
- Automatic save on every change
- Backup/restore support

## 📸 Screenshots

```
┌─────────────────────────────────┐
│  ESP32 TFT UI - Main Menu      │
├─────────────────────────────────┤
│  [NRF Control]                  │
│  [CC1101 Control]               │
│  [WiFi Hotspot]                 │
│  [Bluetooth]                    │
│  [Settings]                     │
└─────────────────────────────────┘
```

## 🚀 Quick Start

### Hardware Requirements
- ESP32 development board
- ILI9341 TFT display (2.4" or 2.8") with touch panel and SD card slot
- NRF24L01 modules (optional, 1 or 2)
- CC1101 module (optional)
- MicroSD card (FAT32 format)

### Software Requirements
- [PlatformIO](https://platformio.org/) or Arduino IDE
- USB cable for programming

### Installation

1. **Clone the repository:**
   ```bash
   git clone https://github.com/yourusername/ESP32-TFT-UI.git
   cd ESP32-TFT-UI
   ```

2. **Install dependencies:**
   ```bash
   platformio lib install
   ```

3. **Build and upload:**
   ```bash
   platformio run --target upload
   ```

4. **Prepare SD card:**
   - Format as FAT32
   - (Optional) Add `splash.bmp` (320x240 pixels, 24-bit BMP)
   - Insert into TFT module

5. **Connect hardware** (see wiring diagrams below)

6. **Power on and enjoy!**

## 🔌 Wiring Diagrams

### TFT Display (ILI9341)

```
TFT Module    ESP32
──────────────────────
VCC       →   3.3V/5V
GND       →   GND
CS        →   GPIO 5  (configurable)
RESET     →   GPIO 16 (configurable)
DC        →   GPIO 17 (configurable)
MOSI      →   GPIO 23
MISO      →   GPIO 19
SCK       →   GPIO 18
T_CS      →   GPIO 21 (configurable)
T_IRQ     →   GPIO 22 (configurable)
SD_CS     →   GPIO 4  (configurable)
```

### NRF24L01 Modules

```
NRF #1        ESP32          NRF #2        ESP32
──────────────────────       ──────────────────────
VCC       →   3.3V           VCC       →   3.3V
GND       →   GND            GND       →   GND
CE        →   GPIO 25        CE        →   GPIO 27
CSN       →   GPIO 26        CSN       →   GPIO 32
MOSI      →   GPIO 23        MOSI      →   GPIO 23
MISO      →   GPIO 19        MISO      →   GPIO 19
SCK       →   GPIO 18        SCK       →   GPIO 18
```

### CC1101 Module

```
CC1101        ESP32
──────────────────────
VCC       →   3.3V
GND       →   GND
CSN       →   GPIO 33 (configurable)
GDO0      →   GPIO 14 (configurable)
GDO2      →   GPIO 12 (configurable)
MOSI      →   GPIO 23
MISO      →   GPIO 19
SCK       →   GPIO 18
```

## 📖 Documentation

### User Guides
- [**Quick Reference Card**](docs/QUICK_REFERENCE.md) - Fast lookup for settings and features
- [**SD Card & Settings Guide**](docs/SD_CARD_SETTINGS_GUIDE.md) - Complete setup instructions
- [**Splash Image Guide**](docs/SPLASH_IMAGE_GUIDE.md) - Create custom startup screens
- [**NRF Control**](docs/nrf_control.md) - NRF24L01 module usage
- [**CC1101 Control**](docs/cc1101_control.md) - CC1101 module usage

### Technical Documentation
- [**System Architecture**](docs/SYSTEM_ARCHITECTURE.md) - System diagrams and flow
- [**CC1101 Integration**](docs/CC1101_INTEGRATION.md) - Technical implementation details
- [**Implementation Summary**](IMPLEMENTATION_COMPLETE.md) - Complete feature list

### Quick References
- [**CC1101 Quick Ref**](docs/cc1101_quick_ref.md) - CC1101 cheat sheet
- [**Settings Summary**](docs/SETTINGS_SPLASH_SUMMARY.md) - Settings overview

## 💾 Settings Storage

All settings are automatically saved to `/settings.json` on the SD card:

```json
{
  "tft_cs": 5,
  "tft_dc": 17,
  "tft_rst": 16,
  "nrf1_ce": 25,
  "nrf1_csn": 26,
  "cc1101_csn": 33,
  "cc1101_frequency": 433.92,
  "cc1101_modulation": "ASK",
  "nrf_data_rate": "2MBPS",
  "splash_file": "/splash.bmp",
  "splash_duration": 3000
}
```

### Configurable via UI
- All 13 GPIO pins
- RF module settings
- Splash screen preferences
- WiFi credentials

**No code changes needed!**

## 🎨 Custom Splash Screen

Create a personalized startup screen:

1. Create 320x240 pixel image
2. Save as 24-bit BMP
3. Name it `splash.bmp`
4. Copy to SD card root
5. Configure duration: Settings → Splash Screen

**Test instantly** with "Test Splash Now" - no reboot required!

## 🛠️ Configuration Menu

```
Main Menu
├── NRF Control
│   ├── Send Test / Burst
│   ├── Continuous Mode
│   └── Set Data Rate
├── CC1101 Control
│   ├── Send Test / Burst
│   ├── Set Frequency
│   └── Set Modulation
├── WiFi Hotspot
├── Bluetooth
└── Settings
    ├── Pin Configuration (13 pins)
    └── Splash Screen
        ├── Set Duration
        ├── Custom Filename
        └── Test Splash Now
```

## 📚 Libraries Used

- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) - Display driver
- [XPT2046_Touchscreen](https://github.com/PaulStoffregen/XPT2046_Touchscreen) - Touch controller
- [RF24](https://github.com/nRF24/RF24) - NRF24L01 library
- [SmartRC-CC1101](https://github.com/LSatan/SmartRC-CC1101-Driver-Lib) - CC1101 library
- [ArduinoJson](https://arduinojson.org/) - JSON parsing
- SD (Arduino core) - SD card support

## 🔧 Troubleshooting

### SD Card Not Detected
- Ensure FAT32 format (NOT exFAT or NTFS)
- Check SD_CS pin connection (default GPIO 4)
- Try different SD card
- Verify wiring

### Splash Screen Not Showing
- Check filename is exactly `splash.bmp`
- Verify image is 320x240 pixels
- Ensure 24-bit BMP format (not PNG/JPG)
- Confirm file is in root directory

### Settings Not Saving
- SD card must be present and working
- Check SD card is not write-protected
- Monitor serial output (115200 baud)
- Verify FAT32 format

### RF Modules Not Working
- Verify power (3.3V for NRF/CC1101)
- Check SPI wiring (MOSI, MISO, SCK)
- Ensure correct CS/CE pins
- Check Serial Monitor for initialization messages

## 📊 Project Structure

```
ESP32-TFT-UI/
├── platformio.ini          # Build configuration
├── include/                # Header files
│   ├── config.h
│   ├── ui.h
│   ├── menu.h
│   ├── nrf_control.h
│   ├── cc1101_control.h
│   └── settings.h
├── src/                    # Source files
│   ├── main.cpp
│   ├── ui.cpp
│   ├── menu.cpp
│   ├── nrf_control.cpp
│   ├── cc1101_control.cpp
│   └── settings.cpp
└── docs/                   # Documentation
    ├── QUICK_REFERENCE.md
    ├── SD_CARD_SETTINGS_GUIDE.md
    └── ...
```

## 🎯 Key Features

✅ **Persistent Settings** - All configurations stored on SD card  
✅ **Touch UI** - Intuitive menu navigation  
✅ **RF Control** - NRF24L01 & CC1101 support  
✅ **Custom Splash** - Personalized startup screens  
✅ **Pin Configuration** - Change any GPIO via UI  
✅ **Live Preview** - Test changes without rebooting  
✅ **Auto-Save** - Never lose your settings  
✅ **Modular Design** - Easy to extend  

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- TFT_eSPI library by Bodmer
- RF24 library by TMRh20
- SmartRC-CC1101 library by LSatan
- ArduinoJson by Benoit Blanchon
- PlatformIO for the excellent development platform

## 📞 Support

- 📖 Check the [documentation](docs/) first
- 🐛 Report bugs via [Issues](https://github.com/yourusername/ESP32-TFT-UI/issues)
- 💬 Ask questions in [Discussions](https://github.com/yourusername/ESP32-TFT-UI/discussions)
- 📧 Email: your.email@example.com

## 🌟 Show Your Support

Give a ⭐️ if this project helped you!

---

**Made with ❤️ for the ESP32 community**
