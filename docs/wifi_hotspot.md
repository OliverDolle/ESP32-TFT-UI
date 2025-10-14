# WiFi Hotspot Documentation

## Overview
Creates a local WiFi hotspot using ESP32's softAP mode. Settings like SSID/password stored on SD.

## Complex Functions
- `initWiFi()`: Starts softAP with credentials from settings.
- `wifiMenu()`: Displays current hotspot info and allows changes (placeholder for input).

## Adding Functionality
To add WiFi client mode:
1. Add a new menu item in `menu.cpp`.
2. Create a new function like `wifiClientMenu()` in this file.
3. Use `WiFi.begin()` for connecting.