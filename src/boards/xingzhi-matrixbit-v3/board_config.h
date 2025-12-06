#if BOARD_XINGZHI_MATRIXBIT_V3 == 1

#ifndef _BOARD_CONFIG_H
#define _BOARD_CONFIG_H

#include <driver/gpio.h>

//*******************************************************************
// 芯片： ESP32-S3 N16R8
// 显示屏： 1.54 TFT-LCD 240x240 ST7789
// 音频编解码：ES8311  I2C：0x30
// 音频功放：NS5150B
// 麦克风
// 温湿度计：AHT30
// 加速度计：SC7A20HTR
// 光敏电阻
// RGB: WS2812 x 3
// I/O控制： RP2040
// 摄像头
// 电量检测： U 10K / L 10K
// USB-Serial: CH334P
//*******************************************************************

#ifndef ARDUINO_ESP32S3_DEV
#error "开发板设置错误，请指定为ESP32S3-DEV。"
#endif

//*******************************************************************
// 引脚定义

// 音频
#define AUDIO_INPUT_SAMPLE_RATE             24000
#define AUDIO_OUTPUT_SAMPLE_RATE            24000

#define AUDIO_INPUT_REFERENCE               true
#define AUDIO_I2S_WS_PIN                    GPIO_NUM_41
#define AUDIO_I2S_MCLK_PIN                  GPIO_NUM_12
#define AUDIO_I2S_BCLK_PIN                  GPIO_NUM_13
#define AUDIO_I2S_DIN_PIN                   GPIO_NUM_14
#define AUDIO_I2S_DOUT_PIN                  GPIO_NUM_45

#define AUDIO_CODEC_PA_PIN                  GPIO_NUM_21
#define AUDIO_CODEC_I2C_SDA_PIN             GPIO_NUM_1
#define AUDIO_CODEC_I2C_SCL_PIN             GPIO_NUM_2
#define AUDIO_CODEC_ES8311_ADDR             ES8311_CODEC_DEFAULT_ADDR

// 按键
#define BOOT_BUTTON_PIN                     GPIO_NUM_0

// 显示屏
#define DISPLAY_MOSI_PIN                    GPIO_NUM_39
#define DISPLAY_MISO_PIN                    GPIO_NUM_NC
#define DISPLAY_SCK_PIN                     GPIO_NUM_40
#define DISPLAY_DC_PIN                      GPIO_NUM_38
#define DISPLAY_CS_PIN                      GPIO_NUM_NC
#define DISPLAY_RST_PIN                     GPIO_NUM_NC
#define DISPLAY_BACKLIGHT_PIN               GPIO_NUM_11

#define DISPLAY_BACKLIGHT_OUTPUT_INVERT     false
#define DISPLAY_INVERT_COLOR                true
#define DISPLAY_RGB_ORDER                   LCD_RGB_ELEMENT_ORDER_RGB
#define DISPLAY_WIDTH                       240
#define DISPLAY_HEIGHT                      240
#define DISPLAY_SWAP_XY                     false
#define DISPLAY_MIRROR_X                    false
#define DISPLAY_MIRROR_Y                    false
#define DISPLAY_OFFSET_X                    0
#define DISPLAY_OFFSET_Y                    0

// 4g
#define NETWORK_MODULE_POWER_IN             GPIO_NUM_NC
#define ML307_RX_PIN                        GPIO_NUM_48
#define ML307_TX_PIN                        GPIO_NUM_47
#define NETWORK_ZXC                         GPIO_NUM_NC

// 电源管理
#define POWER_USB_IN                        GPIO_NUM_NC
#define Power_Control                       GPIO_NUM_NC
#define Power_Dec                           GPIO_NUM_NC
#define POWER_CBS_ADC_UNIT                  ADC_UNIT_1   // battery adc检测unit GPIO10
#define POWER_BATTERY_ADC_CHANNEL           ADC_CHANNEL_9 // 电池电量检测 GPIO10

/* Camera pins */
#define CAMERA_PIN_PWDN                     -1
#define CAMERA_PIN_RESET                    -1
#define CAMERA_PIN_XCLK                     5
#define CAMERA_PIN_SIOD                     1
#define CAMERA_PIN_SIOC                     2

#define CAMERA_PIN_D7                       9
#define CAMERA_PIN_D6                       4
#define CAMERA_PIN_D5                       6
#define CAMERA_PIN_D4                       15
#define CAMERA_PIN_D3                       17
#define CAMERA_PIN_D2                       8
#define CAMERA_PIN_D1                       18
#define CAMERA_PIN_D0                       16
#define CAMERA_PIN_VSYNC                    3
#define CAMERA_PIN_HREF                     46
#define CAMERA_PIN_PCLK                     7

#define XCLK_FREQ_HZ                        24000000


//**********************************************************************
// 配置定义

// 外设
#define CONFIG_USE_DISPLAY                  1   // 显示
#define CONFIG_USE_AUDIO                    1   // 音频
#define CONFIG_USE_FS                       0   // 文件系统
#define CONFIG_USE_CAMERA                   0   // 摄像头

// LVGL
#define CONFIG_USE_LVGL                     0  // LVGL
#define LV_LVGL_H_INCLUDE_SIMPLE            0

// 图形化（最多设置一个为1，其余必须为0）
#define CONFIG_USE_LCD_PANEL                0   // 直接驱动
#define CONFIG_USE_TFT_ESPI                 0   // TFT_eSPI
#define CONFIG_USE_GFX_LIBRARY              1   // GFX_LIBRARY, 可与LVGL整合使用

// 直接驱动
#define CONFIG_USE_DISPLAY_ST7789           0

// 音频驱动（最多设置一个为1，其余必须为0）
#define CONFIG_USE_AUDIO_ES8311             1

// LED驱动
#define CONFIG_USE_LED_GPIO                 0
#define CONFIG_USE_LED_RGB                  0
#define CONFIG_USE_LED_WS2812               0

// 使用ESP_LOG（无串口转换芯片设置为1）
#define CONFIG_USE_ESP_LOG                  0

#endif //_BOARD_CONFIG_H

#endif