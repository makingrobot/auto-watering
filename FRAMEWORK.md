# ESP32 Arduino Framework
Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。

## 概要

`ESP32 Arduino Framework`是专门针对ESP32开发板的Arduino应用开发框架，为用户开发IOT应用、HMI应用提供一致的开发体验。
目前支持以下功能:
* 提供常用LCD、OLED显示屏设备的驱动
* 提供常用音频编解码器设备的驱动
* 提供LED、灯珠等设备的驱动
* 提供统一的抽象化接口来操作UI、音频编解码等
* 提供WiFi信息配置界面
* 提供OTA在线升级
* 内置支持LVGL、TFT_eSPI、U8G2、GFX_Library图形化开发库

## 硬件支持
### 显示屏芯片
#### ESP-LCD库驱动的芯片
* ILI9341、ST7796、ST7789、SSD1306
#### U8G2库支持的芯片
#### TFT_eSPI库支持的芯片
#### GFX_Library库支持的芯片

### 音频编解码芯片
* ES8311
* ES8374
* ES8388

## 使用的库

### 集成的库
* esp_codec_dev - 音频编解码库 [链接](https://components.espressif.com/components/espressif/esp_codec_dev)
* esp_lcd_ili9341 - ILI9341驱动库 [链接](https://components.espressif.com/components/espressif/esp_lcd_ili9341)
* esp_lcd_st7796 - ST7796 LCD驱动库 [链接](https://components.espressif.com/components/espressif/esp_lcd_st7796)
* esp_lvgl_port - ESP32 LVGL库 [链接](https://components.espressif.com/components/espressif/esp_lvgl_port)

### ESP32库引用
* WebServer - 网站服务库
* WiFi - WiFi操作库
* SD - SD卡操作库
* SD_MMC - SD卡操作库（四线方式）
* FS - 文件系统库
* HTTPClient - HTTP客户端库

### 第三方库引用
* LVGL v9.2.2 - 图形化开发库 [链接](https://lvgl.io)
* U8g2 - OLED屏显示开发库
* TFT_eSPI - TFT-LCD屏显示开发库
* GFX_Library v1.4.9 - TFT-LCD屏显示开发库
* OneButton v2.6.1 - 按键库
* Adafruit_NeoPixel - WS2812驱动库
* ESP32-audioI2S - I2S音频库 （DEMO_MP3使用）

## 架构预览

图如下:


## 使用方法
### 1.实现开发板类
继承WiFiBoard类，对板上的硬件进行初始化

1) LED

2) 按键

3) 显示屏

4) 音频编解码

5) 可充电电池电压检测

6) 低功耗处理

### 2.实现显示类
若使用TFT-LCD屏

* 图形化交互开发建议使用LvglDisplay类 或 继承LvglDisplay类
* 自由绘制开发建议使用TtfDisplay类 或 继承TftDisplay类

若使用OLED屏

* 使用U8g2Display类 或 继承U8g2Display类

### 3.实现业务类
继承Application类

