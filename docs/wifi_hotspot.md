# WiFi Hotspot/Extender Documentation

## Overview
Creates a local WiFi hotspot (AP mode) or extends an existing WiFi network (STA+AP mode). Settings (AP SSID/password, upstream SSID/password, mode) stored on SD.

## Complex Functions
- `initWiFi()`: Initializes AP or STA+AP mode based on `extender_mode` in settings. Connects to upstream router if enabled.
- `wifiMenu()`: Displays AP SSID/IP and extender status; allows AP SSID changes.
- `configureExtender()`: UI to set upstream SSID/password and toggle extender mode.
- `getTextInput(prompt, currentVal)`: Keyboard UI for entering SSID/password.

## Adding Functionality
To improve routing:
1. Add IP forwarding logic (advanced; requires lwIP tweaks).
2. Update `initWiFi()` to handle channel sync with upstream router.
To add client mode only:
1. Add a menu item in `menu.cpp`.
2. Implement `wifiClientMode()` using `WiFi.begin()` without AP.

## Notes
- Extender mode requires a valid upstream network. If connection fails, AP still runs.
- Same-channel operation recommended for performance.
- Text input is basic; extend `getTextInput()` for special characters if needed.