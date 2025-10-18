NRF Control Documentation
Overview
This module controls two nRF24L01+ radios attached to the ESP32. NRF1 is primarily a transmitter, NRF2 a receiver, for testing. Supports single message, burst (100 messages), continuous channel-scanning modes, and dynamic data rate configuration.
Complex Functions

initNRF(): Initializes SPI, sets data rate (250 kbps, 1 Mbps, 2 Mbps), power level (high), and dual addresses from settings.json. Uses rf24_datarate_e for data rate settings.
nrfMenu(): Displays UI with buttons for sending (single/burst/continuous), receiving, and setting data rate. Continuous mode scans all channels (126) or WiFi/Bluetooth-safe channels (80).
sendBurst(count): Sends multiple messages in a loop.

Adding Functionality
To add custom payloads:

Modify sendBurst() or add a new function.
Update UI in nrfMenu() with a new button.
Add to settings.json if persistent.

To add address editing:

Use getTextInput() from ui.h for 5-byte input.
Add UI option in nrfMenu().

Notes

Data Rate: Configurable via UI (250 kbps, 1 Mbps, 2 Mbps). Default is 2 Mbps (highest). Lower rates improve range/noise immunity, especially with WiFi active. Uses rf24_datarate_e enum from RF24.h.
Continuous Mode: Scans channels (all or 80+) in a loop when ON. Use channel 80+ to avoid WiFi interference (2.4 GHz band).
Testing: Ensure modules are close (<10m) at 2 Mbps. Check Serial Monitor for received messages if UI fails.
Dependencies: Requires RF24 library in platformio.ini (e.g., lib_deps = RF24).
