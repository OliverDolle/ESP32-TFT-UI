#ifndef CONFIG_H
#define CONFIG_H

// Screen dimensions
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

// Settings file on SD
#define SETTINGS_FILE "/settings.json"

// Core SPI pins (fixed for shared bus stability)
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCLK 18

// Dynamic pins (loaded from settings)
extern int TFT_CS_PIN;
extern int TFT_DC_PIN;
extern int TFT_RST_PIN;
extern int TOUCH_CS_PIN;
extern int TOUCH_IRQ_PIN;
extern int SD_CS_PIN;
extern int NRF1_CE_PIN;
extern int NRF1_CSN_PIN;
extern int NRF2_CE_PIN;
extern int NRF2_CSN_PIN;
extern int CC1101_CSN_PIN;
extern int CC1101_GDO0_PIN;
extern int CC1101_GDO2_PIN;

#endif