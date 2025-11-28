#include "config.h"
#if BOARD_XPSTEM_JC4827W543 == 1

#ifndef _BOARD_CONFIG_H
#define _BOARD_CONFIG_H

#include <driver/gpio.h>

// 芯片： ESP32-S3 N4R8  
// LCD： 4.3寸 480x272 NV3041A QSPI
// Touch： GT911
// 音频放大： NS4168
// 电池管理： IP5306

//*******************************************************************
// 引脚定义

// Button
#define BOOT_BUTTON_PIN                     GPIO_NUM_0

// QSPI LCD 
#define DISPLAY_CLK_PIN                     GPIO_NUM_47
#define DISPLAY_CS_PIN                      GPIO_NUM_45
#define DISPLAY_D0_PIN                      GPIO_NUM_21
#define DISPLAY_D1_PIN                      GPIO_NUM_48
#define DISPLAY_D2_PIN                      GPIO_NUM_40
#define DISPLAY_D3_PIN                      GPIO_NUM_39
#define DISPLAY_BACKLIGHT_PIN               GPIO_NUM_1
#define DISPLAY_BACKLIGHT_OUTPUT_INVERT     false

#define DISPLAY_WIDTH                       480
#define DISPLAY_HEIGHT                      272
#define DISPLAY_ROTATION                    2

// Touch
#define TOUCH_INT_PIN                       GPIO_NUM_3
#define TOUCH_SCL_PIN                       GPIO_NUM_4
#define TOUCH_SDA_PIN                       GPIO_NUM_8
#define TOUCH_GT911_ADD R                   0x5D

// Audio
#define RTP_CS_PIN                          GPIO_NUM_38
#define RTP_DIN_PIN                         GPIO_NUM_11
#define RTP_CLK_PIN                         GPIO_NUM_12
#define RTP_DOUT_PIN                        GPIO_NUM_13
#define RTP_IRQ_PIN                         GPIO_NUM_3  //复用

#define SPECK_LRCLK_PIN                     GPIO_NUM_2
#define SPECK_DIN_PIN                       GPIO_NUM_41
#define SPECK_BCLK_PIN                      GPIO_NUM_42

// SD TF
#define SD_TF_CS                            GPIO_NUM_10
#define SD_TF_MISO                          GPIO_NUM_11  //复用
#define SD_TF_CLK                           GPIO_NUM_12  //复用
#define SD_TF_MOSI                          GPIO_NUM_13  //复用

// for GFX_LIBRARY
#define CANVAS

//**********************************************************************
// 配置定义

// 外设
#define CONFIG_USE_DISPLAY                  1   // 显示
#define CONFIG_USE_AUDIO                    0   // 音频
#define CONFIG_USE_FS                       0   // 文件系统
#define CONFIG_USE_CAMERA                   0   // 摄像头

// LVGL
#define CONFIG_USE_LVGL                     0  // LVGL
#define LV_LVGL_H_INCLUDE_SIMPLE            0

// 图形化（最多设置一个为1，其余必须为0）
#define CONFIG_USE_LCD_PANEL                0   // 直接驱动
#define CONFIG_USE_U8G2                     0   // U8G2
#define CONFIG_USE_TFT_ESPI                 0   // TFT_eSPI
#define CONFIG_USE_GFX_LIBRARY              1   // GFX_LIBRARY, 可与LVGL整合使用

// 显示驱动（最多设置一个为1，其余必须为0）
#define CONFIG_USE_DISPLAY_ILI9341          0
#define CONFIG_USE_DISPLAY_ST7789           0
#define CONFIG_USE_DISPLAY_ST7796           0
#define CONFIG_USE_DISPLAY_SSD1306          0

// 音频驱动（最多设置一个为1，其余必须为0）
#define CONFIG_USE_AUDIO_ES8311             0
#define CONFIG_USE_AUDIO_ES8374             0
#define CONFIG_USE_AUDIO_ES8388             0

// LED驱动
#define CONFIG_USE_LED_GPIO                 0
#define CONFIG_USE_LED_RGB                  0
#define CONFIG_USE_LED_WS2812               0


#endif //_BOARD_CONFIG_H

#endif