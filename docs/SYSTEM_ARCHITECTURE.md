# System Architecture Diagram

## Complete System Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                         ESP32 DevKit                             │
│                                                                  │
│  ┌────────────────────────────────────────────────────────┐    │
│  │                    Your Firmware                        │    │
│  │  - NRF24L01 Control      - CC1101 Control              │    │
│  │  - WiFi Hotspot          - Bluetooth                    │    │
│  │  - Settings Management   - UI System                    │    │
│  └────────────────────────────────────────────────────────┘    │
│                              │                                   │
│                              ↓                                   │
│                         SPI Bus (Shared)                         │
│                    MOSI(23) MISO(19) SCLK(18)                   │
└─────────────────────────────────────────────────────────────────┘
                               │
            ┌──────────────────┴──────────────────┐
            │                                      │
            ↓                                      ↓
┌───────────────────────────┐        ┌───────────────────────────┐
│   2.4/2.8" TFT Display    │        │     RF Modules            │
│   (ILI9341 Controller)    │        │                           │
│                           │        │  ┌─────────────────────┐  │
│  ┌─────────────────────┐ │        │  │   NRF24L01 #1       │  │
│  │   320x240 Screen    │ │        │  │   CE:  GPIO 25      │  │
│  │                     │ │        │  │   CSN: GPIO 26      │  │
│  │   TFT Controller    │ │        │  └─────────────────────┘  │
│  │   CS:  GPIO 5       │ │        │                           │
│  │   DC:  GPIO 17      │ │        │  ┌─────────────────────┐  │
│  │   RST: GPIO 16      │ │        │  │   NRF24L01 #2       │  │
│  └─────────────────────┘ │        │  │   CE:  GPIO 27      │  │
│                           │        │  │   CSN: GPIO 32      │  │
│  ┌─────────────────────┐ │        │  └─────────────────────┘  │
│  │   Touch Panel       │ │        │                           │
│  │   (XPT2046)         │ │        │  ┌─────────────────────┐  │
│  │   CS:  GPIO 21      │ │        │  │   CC1101            │  │
│  │   IRQ: GPIO 22      │ │        │  │   CSN:  GPIO 33     │  │
│  └─────────────────────┘ │        │  │   GDO0: GPIO 14     │  │
│                           │        │  │   GDO2: GPIO 12     │  │
│  ┌─────────────────────┐ │        │  └─────────────────────┘  │
│  │   SD Card Slot      │ │        └───────────────────────────┘
│  │   CS: GPIO 4        │ │
│  │                     │ │
│  │  ┌───────────────┐ │ │
│  │  │settings.json  │ │ │  ← ALL SETTINGS STORED HERE!
│  │  │splash.bmp     │ │ │  ← STARTUP IMAGE HERE!
│  │  └───────────────┘ │ │
│  └─────────────────────┘ │
└───────────────────────────┘
```

---

## Settings Storage Flow

```
┌─────────────────────────────────────────────────────────────┐
│                      Boot Sequence                           │
└─────────────────────────────────────────────────────────────┘
                               │
                               ↓
                    ┌──────────────────────┐
                    │  loadSettings()      │
                    │  from SD card        │
                    └──────────────────────┘
                               │
                               ↓
                    ┌──────────────────────┐
                    │  settings.json       │
                    │  exists?             │
                    └──────────────────────┘
                         │            │
                    YES  │            │  NO
                         ↓            ↓
              ┌─────────────┐  ┌─────────────────┐
              │  Load JSON  │  │ Create defaults │
              │  from file  │  │ Save to SD card │
              └─────────────┘  └─────────────────┘
                         │            │
                         └─────┬──────┘
                               ↓
                    ┌──────────────────────┐
                    │  Apply Settings:     │
                    │  - Pin assignments   │
                    │  - RF configs        │
                    │  - WiFi settings     │
                    │  - Splash settings   │
                    └──────────────────────┘
                               │
                               ↓
                    ┌──────────────────────┐
                    │  Display Splash      │
                    │  (if file exists)    │
                    └──────────────────────┘
                               │
                               ↓
                    ┌──────────────────────┐
                    │  Initialize Modules  │
                    │  - NRF, CC1101       │
                    │  - WiFi, Bluetooth   │
                    └──────────────────────┘
                               │
                               ↓
                    ┌──────────────────────┐
                    │  Show Main Menu      │
                    └──────────────────────┘
```

---

## User Changes Settings

```
┌─────────────────────────────────────────────────────────────┐
│              User Interaction Flow                           │
└─────────────────────────────────────────────────────────────┘
                               │
                               ↓
                    ┌──────────────────────┐
                    │  User navigates      │
                    │  Settings menu       │
                    └──────────────────────┘
                               │
                               ↓
                    ┌──────────────────────┐
                    │  Change setting      │
                    │  via touch UI        │
                    └──────────────────────┘
                               │
                               ↓
                    ┌──────────────────────┐
                    │  Update in memory:   │
                    │  settings[key]=value │
                    └──────────────────────┘
                               │
                               ↓
                    ┌──────────────────────┐
                    │  saveSettings()      │
                    │  Write to SD card    │
                    └──────────────────────┘
                               │
                               ↓
                    ┌──────────────────────┐
                    │  Pin change?         │
                    └──────────────────────┘
                         │            │
                    YES  │            │  NO
                         ↓            ↓
              ┌─────────────┐  ┌─────────────────┐
              │ESP.restart()│  │  Show "Saved!"  │
              │  (reboot)   │  │  Continue       │
              └─────────────┘  └─────────────────┘
```

---

## Splash Screen System

```
┌─────────────────────────────────────────────────────────────┐
│              Splash Screen Flow                              │
└─────────────────────────────────────────────────────────────┘
                               │
                               ↓
                    ┌──────────────────────┐
                    │  Read settings:      │
                    │  - splash_file       │
                    │  - splash_duration   │
                    └──────────────────────┘
                               │
                               ↓
                    ┌──────────────────────┐
                    │  Check if file       │
                    │  exists on SD card   │
                    └──────────────────────┘
                         │            │
                   FOUND │            │  NOT FOUND
                         ↓            ↓
              ┌─────────────┐  ┌──────────────────┐
              │  Load BMP   │  │  Show default    │
              │  Display it │  │  "Initializing"  │
              │             │  │  screen          │
              └─────────────┘  └──────────────────┘
                         │            │
                         └─────┬──────┘
                               ↓
                    ┌──────────────────────┐
                    │  Wait for duration   │
                    │  (default: 3 sec)    │
                    └──────────────────────┘
                               │
                               ↓
                    ┌──────────────────────┐
                    │  Clear screen        │
                    │  Continue boot       │
                    └──────────────────────┘
```

---

## Menu Structure

```
Main Menu
│
├── NRF Control
│   ├── Send Test (1x)
│   ├── Send Burst (100x)
│   ├── Continuous (ON/OFF)
│   ├── Receive Test
│   ├── Set Data Rate
│   │   ├── 250 kbps
│   │   ├── 1 Mbps
│   │   └── 2 Mbps
│   └── Back
│
├── CC1101 Control
│   ├── Send Test (1x)
│   ├── Send Burst (100x)
│   ├── Continuous (ON/OFF)
│   ├── Set Frequency
│   │   ├── 315 MHz
│   │   ├── 433.92 MHz
│   │   ├── 868 MHz
│   │   └── 915 MHz
│   ├── Set Modulation
│   │   ├── ASK/OOK
│   │   ├── 2-FSK
│   │   ├── GFSK
│   │   └── MSK
│   └── Back
│
├── WiFi Hotspot
│   └── [WiFi controls]
│
├── Bluetooth
│   └── [BT controls]
│
└── Settings
    ├── Pin Configuration
    │   ├── TFT CS
    │   ├── TFT DC
    │   ├── TFT RST
    │   ├── Touch CS
    │   ├── Touch IRQ
    │   ├── SD CS
    │   ├── NRF1 CE
    │   ├── NRF1 CSN
    │   ├── NRF2 CE
    │   ├── NRF2 CSN
    │   ├── CC1101 CSN
    │   ├── CC1101 GDO0
    │   ├── CC1101 GDO2
    │   └── Back
    │
    ├── Splash Screen                    ← NEW!
    │   ├── Set Duration (1-10s)         ← NEW!
    │   ├── Default (/splash.bmp)        ← NEW!
    │   ├── Custom Filename              ← NEW!
    │   ├── Test Splash Now              ← NEW!
    │   └── Back
    │
    └── Back
```

---

## Data Persistence

```
┌─────────────────────────────────────────────────────────────┐
│                What Gets Saved Where                         │
└─────────────────────────────────────────────────────────────┘

SD Card: /settings.json
┌─────────────────────────────────────────────────────────────┐
│ {                                                            │
│   // Display pins                                           │
│   "tft_cs": 5,                                              │
│   "tft_dc": 17,                                             │
│   "tft_rst": 16,                                            │
│   "touch_cs": 21,                                           │
│   "touch_irq": 22,                                          │
│   "sd_cs": 4,                                               │
│                                                              │
│   // NRF24L01 pins                                          │
│   "nrf1_ce": 25,                                            │
│   "nrf1_csn": 26,                                           │
│   "nrf2_ce": 27,                                            │
│   "nrf2_csn": 32,                                           │
│                                                              │
│   // CC1101 pins                                            │
│   "cc1101_csn": 33,                                         │
│   "cc1101_gdo0": 14,                                        │
│   "cc1101_gdo2": 12,                                        │
│                                                              │
│   // NRF settings                                           │
│   "nrf_address1": "...",                                    │
│   "nrf_address2": "...",                                    │
│   "nrf_data_rate": "2MBPS",                                 │
│                                                              │
│   // CC1101 settings                                        │
│   "cc1101_frequency": 433.92,                               │
│   "cc1101_modulation": "ASK",                               │
│   "cc1101_power": 10,                                       │
│                                                              │
│   // Splash settings                                        │
│   "splash_file": "/splash.bmp",      ← NEW!                │
│   "splash_duration": 3000,           ← NEW!                │
│                                                              │
│   // WiFi settings                                          │
│   "wifi_ssid": "ESP32_Extender",                           │
│   "wifi_pass": "password",                                  │
│   "extender_mode": false,                                   │
│   "sta_ssid": "",                                           │
│   "sta_pass": ""                                            │
│ }                                                            │
└─────────────────────────────────────────────────────────────┘

SD Card: /splash.bmp
┌─────────────────────────────────────────────────────────────┐
│  Binary BMP file (320x240 pixels, 24-bit color)            │
│  Your custom startup image!                                 │
└─────────────────────────────────────────────────────────────┘
```

---

## Summary

### ✅ All Settings Stored on SD Card
- Location: `/settings.json`
- Format: JSON (human-readable)
- Automatic save on changes
- Persists across reboots

### ✅ Splash Screen Supported
- Location: `/splash.bmp` (or custom)
- Size: 320x240 pixels
- Format: 24-bit BMP
- Configurable via UI

### ✅ Fully Configurable
- All pins changeable
- RF settings adjustable
- Splash customizable
- No code editing needed!

**Your ESP32 TFT UI system is production-ready!** 🚀
