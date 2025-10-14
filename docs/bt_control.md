# Bluetooth Control Documentation

## Overview
Uses ESP32's Classic Bluetooth (SerialBT) for simple send/receive.

## Complex Functions
- `initBT()`: Starts Bluetooth device.
- `btMenu()`: UI for testing send/receive.

## Adding Functionality
For BLE support:
1. Include <BLEDevice.h> etc.
2. Add a new submenu or replace.
3. Update `platformio.ini` if needed.