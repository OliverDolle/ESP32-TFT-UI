# CC1101 Module Integration Summary

## Overview
Successfully integrated CC1101 sub-GHz RF transceiver module into the ESP32-TFT-UI project, following the same configuration pattern as the existing NRF24L01 module.

## Changes Made

### 1. Library Added
**File**: `platformio.ini`
- Added SmartRC-CC1101-Driver-Lib library
```ini
https://github.com/LSatan/SmartRC-CC1101-Driver-Lib.git
```

### 2. New Files Created
- **`include/cc1101_control.h`**: Header file with function declarations
- **`src/cc1101_control.cpp`**: Full implementation with menu system
- **`docs/cc1101_control.md`**: Comprehensive documentation

### 3. Configuration Updates

#### `include/config.h`
Added CC1101 pin declarations:
```cpp
extern int CC1101_CSN_PIN;
extern int CC1101_GDO0_PIN;
extern int CC1101_GDO2_PIN;
```

#### `src/settings.cpp`
- Added CC1101 pin variables
- Added default pin assignments:
  - CC1101_CSN: GPIO 33
  - CC1101_GDO0: GPIO 14
  - CC1101_GDO2: GPIO 12
- Added CC1101 settings defaults:
  - Frequency: 433.92 MHz
  - Modulation: ASK/OOK
  - Power: 10 (max)
- Added pins to loadPins() function

#### `src/menu.cpp`
- Added `#include "cc1101_control.h"`
- Added "CC1101 Control" to main menu items
- Added CC1101 pins to pin configuration array:
  - CC1101 CSN
  - CC1101 GDO0
  - CC1101 GDO2

#### `src/main.cpp`
- Added `#include "cc1101_control.h"`
- Added `initCC1101()` call in setup()

## Features Implemented

### Menu Options
1. **Send Test (1x)**: Send single test packet
2. **Send Burst (100x)**: Send 100 packets rapidly
3. **Continuous Mode (ON/OFF)**: Toggle continuous transmission
4. **Set Frequency**: Choose from 315, 433.92, 868, 915 MHz
5. **Set Modulation**: Choose from ASK/OOK, 2-FSK, GFSK, MSK
6. **Back**: Return to main menu

### Configurable Settings
All settings are stored in `settings.json` on SD card:
- **cc1101_csn**: Chip select pin
- **cc1101_gdo0**: General digital output 0 pin
- **cc1101_gdo2**: General digital output 2 pin
- **cc1101_frequency**: Operating frequency in MHz
- **cc1101_modulation**: Modulation type
- **cc1101_power**: Transmission power (0-10)

### Pin Configuration
Users can change all CC1101 pins through:
Main Menu → Settings → Pin Configuration → CC1101 pins

Changes are saved and device restarts automatically to apply new pin assignments.

## Wiring Diagram

```
CC1101 Module → ESP32
────────────────────────
VCC      → 3.3V
GND      → GND
MOSI     → GPIO 23 (shared SPI bus)
MISO     → GPIO 19 (shared SPI bus)
SCK      → GPIO 18 (shared SPI bus)
CSN      → GPIO 33 (configurable)
GDO0     → GPIO 14 (configurable)
GDO2     → GPIO 12 (configurable)
```

## Code Architecture

### Initialization Flow
1. `loadSettings()` - Load pins from SD card
2. `initCC1101()` - Initialize CC1101 with custom pins and settings
3. Library configures SPI with custom CSN pin
4. Set frequency, modulation, and power from settings

### Menu System
- Follows same pattern as NRF control
- Status messages displayed at bottom (y=360)
- 2-second timeout for status messages
- Sub-menus for frequency and modulation selection
- All changes saved to SD card and module reinitialized

### Settings Persistence
- All configurations saved in JSON format
- Changes persist across reboots
- Factory defaults applied on first boot
- Pin changes trigger automatic restart

## Testing Recommendations

1. **Verify Pin Connections**: Ensure all SPI pins and CS pin are correctly wired
2. **Check Frequency**: Start with 433.92 MHz (widely compatible)
3. **Test Transmission**: Use "Send Test (1x)" to verify basic operation
4. **Verify Settings**: Check that settings persist after restart
5. **Pin Configuration**: Test changing pins through UI to verify settings system

## Compliance Notes

⚠️ **Important**: CC1101 operates on sub-GHz frequencies subject to regional regulations:
- **315 MHz**: North America, Asia
- **433 MHz**: ISM band (worldwide)
- **868 MHz**: Europe
- **915 MHz**: North America, Australia

Ensure compliance with local RF transmission regulations before use.

## Next Steps

1. Build and upload the project to ESP32
2. Verify CC1101 module is detected on startup (check Serial monitor)
3. Test basic transmission functionality
4. Customize pins if needed through Settings menu
5. Adjust frequency and modulation for your application

## Files Modified Summary

| File | Changes |
|------|---------|
| `platformio.ini` | Added CC1101 library |
| `include/config.h` | Added 3 CC1101 pin declarations |
| `include/cc1101_control.h` | New file - function declarations |
| `src/cc1101_control.cpp` | New file - full implementation (240+ lines) |
| `src/settings.cpp` | Added CC1101 settings and pins |
| `src/menu.cpp` | Added CC1101 menu item and pin config |
| `src/main.cpp` | Added CC1101 initialization |
| `docs/cc1101_control.md` | New file - comprehensive documentation |

All changes follow the existing code patterns and architecture!
