/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _BOARD_CONFIG_H
#define _BOARD_CONFIG_H

#include <driver/gpio.h>

static const char* kManualButton      = "manual_button";
static const char* kPumpControlName   = "pump_control";
static const char* kSoilMositureName  = "soil_mositure";

//*******************************************************************
// 引脚定义

#define BUILTIN_LED_PIN                     GPIO_NUM_4

#define I2C_SCL_PIN                         GPIO_NUM_22  //  
#define I2C_SDA_PIN                         GPIO_NUM_21  //  

#define OLED_I2C_ADDRESS                    0x3D
#define DISPLAY_WIDTH                       128
#define DISPLAY_HEIGHT                      64
#define DISPLAY_MIRROR_X                    false
#define DISPLAY_MIRROR_Y                    false
#define DISPLAY_SWAP_XY                     false

#define SOIL_MOISTURE_PIN                   GPIO_NUM_19
#define MANUAL_BUTTON_PIN                   GPIO_NUM_34
#define L9110_PIN_B                         GPIO_NUM_12
#define L9110_PIN_A                         GPIO_NUM_13
#define L9110_OUPUT_INVERT                  false


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
#define CONFIG_USE_U8G2                     1   // U8G2
#define CONFIG_USE_TFT_ESPI                 0   // TFT_eSPI
#define CONFIG_USE_GFX_LIBRARY              0   // GFX_LIBRARY, 可与LVGL整合使用

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
#define CONFIG_USE_LED_WS2812               0


#endif //_BOARD_CONFIG_H
