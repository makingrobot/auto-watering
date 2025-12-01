#if BOARD_XPSTEM_S3_ELECTRONIC_SUIT == 1

#ifndef _BOARD_CONFIG_H
#define _BOARD_CONFIG_H

// 芯片： ESP32-S3 N16R8

//*******************************************************************
// 引脚定义

#ifndef ARDUINO_ESP32S3_DEV
#error "开发板设置错误，请指定为ESP32S3-DEV。"
#endif

#define I2C_SCL_PIN                         GPIO_NUM_21  //40
#define I2C_SDA_PIN                         GPIO_NUM_7   //41

// LCD模块 ST7796
#define DISPLAY_DC_PIN                      GPIO_NUM_45
#define DISPLAY_CS_PIN                      GPIO_NUM_39
#define DISPLAY_SCK_PIN                     GPIO_NUM_3
#define DISPLAY_MOSI_PIN                    GPIO_NUM_5
#define DISPLAY_MISO_PIN                    GPIO_NUM_4
#define DISPLAY_RST_PIN                     GPIO_NUM_38
#define DISPLAY_LED_PIN                     GPIO_NUM_42

// 触摸模块
#define TOUCH_INT_PIN                       GPIO_NUM_6
#define TOUCH_RST_PIN                       GPIO_NUM_8
#define TOUCH_FT6336_ADDR                   0x38

// 三按键模块
#define BUTTON_0_PIN                        GPIO_NUM_0
#define BUTTON_1_PIN                        GPIO_NUM_1  
#define BUTTON_3_PTN                        GPIO_NUM_47

// 【6P插座】 SD卡模块（与LCD屏共用SPI数据口3，4，5）
#define SD_CS_PIN                           GPIO_NUM_2
#define SD_CLK_PIN                          DISPLAY_SCK_PIN
#define SD_MOSI_PIN                         DISPLAY_MOSI_PIN
#define SD_MISO_PIN                         DISPLAY_MISO_PIN

// 【3P插座】 温湿度传感器模块
#define WEATHER_ADC_PIN                     GPIO_NUM_16    

// 【3P插座】 电位器模块
#define POTENTIONMETER_ADC_PIN              GPIO_NUM_9   

// 【5P插座】 MEMS麦克风模块
#define AUDIO_MIC_SCK_PIN                   GPIO_NUM_12  
#define AUDIO_MIC_WS_PIN                    GPIO_NUM_10
#define AUDIO_MIC_DAT_PIN                   GPIO_NUM_11

// 【5P插座】 I2S音频放大模块
#define AUDIO_SPK_DAT_PIN                   GPIO_NUM_48   
#define AUDIO_SPK_BCLK_PIN                  GPIO_NUM_17
#define AUDIO_SPK_LRC_PIN                   GPIO_NUM_18

#define AUDIO_INPUT_SAMPLE_RATE             16000
#define AUDIO_OUTPUT_SAMPLE_RATE            16000

#define DISPLAY_WIDTH                       480
#define DISPLAY_HEIGHT                      320
#define DISPLAY_MIRROR_X                    false
#define DISPLAY_MIRROR_Y                    false
#define DISPLAY_SWAP_XY                     true
#define DISPLAY_INVERT_COLOR                false
#define DISPLAY_RGB_ORDER                   LCD_RGB_ELEMENT_ORDER_BGR
#define DISPLAY_OFFSET_X                    0
#define DISPLAY_OFFSET_Y                    0
#define DISPLAY_BACKLIGHT_OUTPUT_INVERT true
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

#endif // _BOARD_CONFIG_H

#endif