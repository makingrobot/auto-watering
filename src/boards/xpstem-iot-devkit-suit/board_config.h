#if BOARD_XPSTEM_IOT_DEVKIT_SUIT == 1

#ifndef _BOARD_CONFIG_H
#define _BOARD_CONFIG_H

#include <driver/gpio.h>

//*******************************************************************
// 芯片： ESP32 N4
// 电机驱动： AT8833
// USB-Serial： CH340
//
// 套件：
//   三色LED模块
//   人体感应传感器
//   继电器
//   USB小灯
//   土壤湿度传感器
//   小电机驱动板（L9110）
//   按键模块
//   OLED显示屏（SSD1306）
//   TFT-LCD 2.4寸显示屏（ILI9341）
//   温湿度传感器（DHT11）
//   红外遥控模块（遥控器+红外接收）
//   9g舵机
//   中号马达
//   电源模块
//*******************************************************************

#ifndef ARDUINO_ESP32_DEV
#error "开发板设置错误，请指定为ESP32-DEV。"
#endif

//*******************************************************************
// 引脚定义

#define BOOT_BUTTON_PIN                     GPIO_NUM_0  // Button

#define BUILTIN_LED_PIN                     GPIO_NUM_4

#define I2C_SCL_PIN                         GPIO_NUM_22  //  
#define I2C_SDA_PIN                         GPIO_NUM_21  //  

#define SSD1306_I2C_ADDR                    0x3D

// LCD模块 ST7796
#define DISPLAY_LED_PIN                     GPIO_NUM_15
#define DISPLAY_SCK_PIN                     GPIO_NUM_16
#define DISPLAY_MOSI_PIN                    GPIO_NUM_17
#define DISPLAY_MISO_PIN                    GPIO_NUM_14
#define DISPLAY_DC_PIN                      GPIO_NUM_25
#define DISPLAY_RST_PIN                     GPIO_NUM_26
#define DISPLAY_CS_PIN                      GPIO_NUM_27

#define DISPLAY_WIDTH                       320
#define DISPLAY_HEIGHT                      240
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
#define CONFIG_USE_AUDIO                    0   // 音频
#define CONFIG_USE_FS                       0   // 文件系统
#define CONFIG_USE_CAMERA                   0   // 摄像头

// LVGL
#define CONFIG_USE_LVGL                     1  // LVGL
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
#define CONFIG_USE_LED_GPIO                 1
#define CONFIG_USE_LED_RGB                  0
#define CONFIG_USE_LED_WS2812               0

// 使用ESP_LOG（无串口转换芯片设置为1）
#define CONFIG_USE_ESP_LOG                  0

#endif //_BOARD_CONFIG_H

#endif