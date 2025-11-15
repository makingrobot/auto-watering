/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _CONFIG_H
#define _CONFIG_H

//////////////////////////////////////////////////////////////////////////////////
// 应用（至多设置一个为1，其余必须为0）

#define APP_DEMO_LCD                        0   // LCD + LVGL显示DEMO
#define APP_DEMO_TFT                        0   // TFT_eSPI显示DEMO
#define APP_DEMO_U8G2                       0   // SSD1306 + U8G2 显示DEMO
#define APP_DEMO_MP3                        0   // MP3播放，需要音频外设


//////////////////////////////////////////////////////////////////////////////////
// 主板（只能设置一个为1，其余必须为0）

#define BOARD_XPSTEM_S3_LCD_2_80            0  // 小鹏AI语音机器人、ESP32-S3 2.8寸显示屏开发板
#define BOARD_XPSTEM_IOT_DEVKIT_SUIT        0  // 小鹏IoT开发套件
#define BOARD_XPSTEM_S3_ELECTRONIC_SUIT     0   // 小鹏ESP32-S3创意电子套件
#define BOARD_XINGZHI_MATRIXBIT_V3          0   // Matrixbit V3


#if BOARD_XPSTEM_S3_LCD_2_80 == 1
#include "src/boards/xpstem-s3-lcd-2.8/board_config.h"
#elif BOARD_XPSTEM_IOT_DEVKIT_SUIT == 1
#include "src/boards/xpstem-iot-devkit-suit/board_config.h"
#elif BOARD_XPSTEM_S3_ELECTRONIC_SUIT == 1
#include "src/boards/xpstem-s3-electronic-suit/board_config.h"
#elif BOARD_XINGZHI_MATRIXBIT_V3 == 1
#include "src/boards/xingzhi-matrixbit-v3/board_config.h"
#endif


#endif //_CONFIG_H