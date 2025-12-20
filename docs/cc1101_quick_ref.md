# CC1101 Quick Reference

## Default Pin Configuration
```
CC1101_CSN_PIN   = GPIO 33
CC1101_GDO0_PIN  = GPIO 14
CC1101_GDO2_PIN  = GPIO 12
SPI_MOSI         = GPIO 23 (shared)
SPI_MISO         = GPIO 19 (shared)
SPI_SCLK         = GPIO 18 (shared)
```

## Default Settings
```json
{
  "cc1101_frequency": 433.92,
  "cc1101_modulation": "ASK",
  "cc1101_power": 10
}
```

## Supported Frequencies
- 315.00 MHz
- 433.92 MHz (default)
- 868.00 MHz
- 915.00 MHz

## Supported Modulations
- ASK/OOK (default)
- 2-FSK
- GFSK
- MSK

## Menu Path
Main Menu → CC1101 Control

## Settings Path
Main Menu → Settings → Pin Configuration → CC1101 pins
