#ifndef CONFIG_H
#define CONFIG_H

// Display pins (SPI)
#define TFT_CS   5   // Chip select
#define TFT_DC   17  // Data/Command
#define TFT_RST  16  // Reset
#define TFT_MOSI 23  // MOSI
#define TFT_SCLK 18  // SCLK
#define TFT_MISO 19  // MISO (not always used)

// Touch pins
#define TOUCH_CS  21  // Touch chip select
#define TOUCH_IRQ 22  // Touch interrupt (optional)

// SD pins (shared SPI, but CS separate)
#define SD_CS     4   // SD chip select

// NRF pins (two modules)
#define NRF1_CE   25  // CE for first NRF
#define NRF1_CSN  26  // CSN for first NRF
#define NRF2_CE   27  // CE for second NRF
#define NRF2_CSN  32  // CSN for second NRF

// Screen dimensions
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

// Settings file on SD
#define SETTINGS_FILE "/settings.json"

#endif