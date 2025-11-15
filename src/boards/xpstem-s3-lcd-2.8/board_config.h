#include "config.h"
#if BOARD_XPSTEM_S3_LCD_2_80 == 1

#ifndef _BOARD_CONFIG_H
#define _BOARD_CONFIG_H

#include <driver/gpio.h>

//*******************************************************************
// 引脚定义

// I2c
#define I2C_SDA_PIN                         GPIO_NUM_16
#define I2C_SCL_PIN                         GPIO_NUM_15

// ES8311
#define AUDIO_CODEC_PA_PIN                  GPIO_NUM_1
#define AUDIO_CODEC_ES8311_ADDR             ES8311_CODEC_DEFAULT_ADDR

#define AUDIO_I2S_MCLK_PIN                  GPIO_NUM_4
#define AUDIO_I2S_BCLK_PIN                  GPIO_NUM_5
#define AUDIO_I2S_DOUT_PIN                  GPIO_NUM_8
#define AUDIO_I2S_WS_PIN                    GPIO_NUM_7
#define AUDIO_I2S_DIN_PIN                   GPIO_NUM_6

#define AUDIO_INPUT_SAMPLE_RATE             16000
#define AUDIO_OUTPUT_SAMPLE_RATE            16000

// Button
#define BOOT_BUTTON_PIN                     GPIO_NUM_0

// Led
#define BUILTIN_LED_PIN                     GPIO_NUM_42

// SPI LCD ILI9341
#define DISPLAY_MOSI_PIN                    GPIO_NUM_11
#define DISPLAY_CLK_PIN                     GPIO_NUM_12
#define DISPLAY_BACKLIGHT_PIN               GPIO_NUM_45
#define DISPLAY_DC_PIN                      GPIO_NUM_46
#define DISPLAY_CS_PIN                      GPIO_NUM_10
#define DISPLAY_RST_PIN                     GPIO_NUM_NC

// Touch FT6336
#define TOUCH_RST_PIN                       GPIO_NUM_18
#define TOUCH_INT_PIN                       GPIO_NUM_17
#define TOUCH_FT6336_ADDR                   0x38

// Battery ADC
#define BATTERY_ADC_PIN                     GPIO_NUM_9
#define BATTERY_ADC_UNIT                    ADC_UNIT_1
#define BATTERY_ADC_CHANNEL                 ADC_CHANNEL_8

// SD MMC
#define SD_MMC_CLK                          38
#define SD_MMC_CMD                          40
#define SD_MMC_D1                           39
#define SD_MMC_D2                           41
#define SD_MMC_D3                           48
#define SD_MMC_D4                           47

#define DISPLAY_WIDTH                       240
#define DISPLAY_HEIGHT                      320
#define DISPLAY_MIRROR_X                    true
#define DISPLAY_MIRROR_Y                    false
#define DISPLAY_SWAP_XY                     false
#define DISPLAY_INVERT_COLOR                true
#define DISPLAY_RGB_ORDER                   LCD_RGB_ELEMENT_ORDER_BGR
#define DISPLAY_OFFSET_X                    0
#define DISPLAY_OFFSET_Y                    0
#define DISPLAY_BACKLIGHT_OUTPUT_INVERT     false
#define DISPLAY_SPI_MODE                    0


//**********************************************************************
// 配置定义

// 外设
#define CONFIG_USE_DISPLAY                  1   // 显示
#define CONFIG_USE_AUDIO                    1   // 音频
#define CONFIG_USE_FS                       1   // 文件系统
#define CONFIG_USE_CAMERA                   0   // 摄像头

// LVGL
#define CONFIG_USE_LVGL                     1  // LVGL
#define LV_LVGL_H_INCLUDE_SIMPLE            1

// 图形化（最多设置一个为1，其余必须为0）
#define CONFIG_USE_LCD_PANEL                1   // 直接驱动
#define CONFIG_USE_U8G2                     0   // U8G2
#define CONFIG_USE_TFT_ESPI                 0   // TFT_eSPI
#define CONFIG_USE_GFX_LIBRARY              0   // GFX_LIBRARY, 可与LVGL整合使用

// 显示驱动（最多设置一个为1，其余必须为0）
#define CONFIG_USE_DISPLAY_ILI9341          1
#define CONFIG_USE_DISPLAY_ST7789           0
#define CONFIG_USE_DISPLAY_ST7796           0
#define CONFIG_USE_DISPLAY_SSD1306          0

// 音频驱动（最多设置一个为1，其余必须为0）
#define CONFIG_USE_AUDIO_ES8311             1
#define CONFIG_USE_AUDIO_ES8374             0
#define CONFIG_USE_AUDIO_ES8388             0

// LED驱动
#define CONFIG_USE_LED_GPIO                 0
#define CONFIG_USE_LED_WS2812               1


#endif //_BOARD_CONFIG_H

#endif