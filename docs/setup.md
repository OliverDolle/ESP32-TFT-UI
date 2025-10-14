# Setup Instructions

## Hardware Wiring
- ESP32 to ILI9341 Display:
  - MOSI -> 23
  - SCLK -> 18
  - CS -> 5
  - DC -> 17
  - RST -> 16
  - MISO -> 19 (if needed)
- Touch (XPT2046):
  - CS -> 21
  - IRQ -> 22 (optional)
- SD Slot: CS -> 4 (shared SPI bus)
- NRF1 (nRF24L01+):
  - CE -> 25
  - CSN -> 26
  - SPI shared
- NRF2:
  - CE -> 27
  - CSN -> 32
- Power: 3.3V for all modules (ESP32 is 3.3V logic).

## Software Setup
1. Install PlatformIO.
2. Create project and add files.
3. Build: `pio run`
4. Upload: `pio run --target upload`
5. Monitor: `pio device monitor`

Calibrate touch by adjusting map() values in main.cpp based on raw readings.