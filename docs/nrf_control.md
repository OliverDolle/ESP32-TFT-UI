# NRF Control Documentation

## Overview
This module handles control of two nRF24L01+ radios attached to the ESP32. One acts as transmitter, the other as receiver for testing purposes. In a real setup, connect to external devices.

## Complex Functions
- `initNRF()`: Initializes SPI and sets up pipes using addresses from settings.json on SD.
- `nrfMenu()`: Draws UI buttons for send/receive tests. Uses `RF24::write` and `RF24::read` for communication.

## Adding Functionality
To add a new NRF feature (e.g., custom payload):
1. Add a new button in `nrfMenu()`.
2. Implement the callback logic.
3. Update settings.json if needed for persistence.