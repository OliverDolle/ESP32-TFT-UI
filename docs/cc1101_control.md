# CC1101 Control Module

The CC1101 Control module provides a user interface for controlling the CC1101 sub-GHz RF transceiver module.

## Features

- **Test Transmission**: Send single test packets
- **Burst Mode**: Send 100 packets in rapid succession
- **Continuous Mode**: Continuously transmit packets
- **Frequency Configuration**: Support for common frequencies (315, 433.92, 868, 915 MHz)
- **Modulation Settings**: ASK/OOK, 2-FSK, GFSK, MSK modulation support
- **Custom Pin Configuration**: All CC1101 pins can be configured via settings

## Pin Configuration

The CC1101 module uses the following configurable pins:

- **CC1101_CSN_PIN**: Chip Select (default: GPIO 33)
- **CC1101_GDO0_PIN**: General Digital Output 0 (default: GPIO 14)
- **CC1101_GDO2_PIN**: General Digital Output 2 (default: GPIO 12)

Additionally, it shares the SPI bus pins (MOSI, MISO, SCLK) defined in `config.h`.

## Settings Storage

The module stores the following settings in `settings.json`:

```json
{
  "cc1101_csn": 33,
  "cc1101_gdo0": 14,
  "cc1101_gdo2": 12,
  "cc1101_frequency": 433.92,
  "cc1101_modulation": "ASK",
  "cc1101_power": 10
}
```

### Frequency Options
- **315 MHz**: 315.00 MHz
- **433.92 MHz**: 433.92 MHz (default, ISM band)
- **868 MHz**: 868.00 MHz (Europe ISM band)
- **915 MHz**: 915.00 MHz (US ISM band)

### Modulation Options
- **ASK/OOK**: Amplitude Shift Keying / On-Off Keying (default)
- **2-FSK**: 2-Level Frequency Shift Keying
- **GFSK**: Gaussian Frequency Shift Keying
- **MSK**: Minimum Shift Keying

### Power Levels
- Range: 0-10 (10 = maximum power)
- Default: 10

## Usage

### Accessing the Menu
From the main menu, tap "CC1101 Control" to access the CC1101 control interface.

### Sending Test Packets
1. Tap "Send Test (1x)" to transmit a single test packet
2. Status message will confirm transmission

### Burst Transmission
1. Tap "Send Burst (100x)" to send 100 packets rapidly
2. Useful for testing range and reliability

### Continuous Mode
1. Tap "Continuous (ON/OFF)" to toggle continuous transmission
2. In continuous mode, the module will continuously send packets
3. Tap again to disable

### Changing Frequency
1. Tap "Set Frequency"
2. Select from preset frequencies (315, 433.92, 868, 915 MHz)
3. Settings are saved and module is reinitialized

### Changing Modulation
1. Tap "Set Modulation"
2. Select modulation type (ASK/OOK, 2-FSK, GFSK, MSK)
3. Settings are saved and module is reinitialized

### Changing Pins
1. Go to Main Menu → Settings → Pin Configuration
2. Find CC1101 pins (CSN, GDO0, GDO2)
3. Tap to modify each pin
4. Device will restart after saving

## Library Used

The module uses the **SmartRC-CC1101-Driver-Lib** library by LSatan:
- Repository: https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
- Provides comprehensive CC1101 control for ESP32

## Wiring Example

```
CC1101 Module -> ESP32
VCC          -> 3.3V
GND          -> GND
MOSI         -> GPIO 23 (fixed SPI MOSI)
MISO         -> GPIO 19 (fixed SPI MISO)
SCK          -> GPIO 18 (fixed SPI SCLK)
CSN          -> GPIO 33 (configurable)
GDO0         -> GPIO 14 (configurable)
GDO2         -> GPIO 12 (configurable)
```

## Code Structure

### Files
- `include/cc1101_control.h`: Header file with function declarations
- `src/cc1101_control.cpp`: Implementation of CC1101 control logic

### Key Functions
- `void initCC1101()`: Initialize CC1101 with saved settings
- `void cc1101Menu()`: Main menu interface
- `void sendCC1101Packet(const char* data, int length)`: Send a single packet
- `void sendCC1101Burst(int count)`: Send multiple packets

## Notes

- The CC1101 operates in sub-GHz frequencies (300-928 MHz)
- Ensure compliance with local regulations for RF transmission
- Default frequency (433.92 MHz) is commonly used in ISM band
- GDO0 and GDO2 pins can be used for advanced features (interrupts, carrier sense, etc.)
- All settings persist across reboots via SD card storage
