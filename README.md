# ESP32 Arduino Framework
Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。

[开发框架说明](FRAMEWORK.md)

[项目说明](PROJECT.md) (*用于Fork项目*)

## 一、概要

`ESP32 Arduino Framework`是专门针对ESP32开发板的Arduino应用开发框架，为用户开发IOT应用、HMI应用提供一致的开发体验。
目前支持以下功能:
* 提供常用LCD、OLED显示屏设备的驱动
* 提供常用音频编解码器设备的驱动
* 提供LED、灯珠等设备的驱动
* 提供统一的抽象化接口来操作UI、音频编解码等
* 提供WiFi信息配置界面
* 提供OTA在线升级
* 内置支持LVGL、TFT_eSPI、U8G2、GFX_Library图形化开发库

## 二、DEMO和内置开发板

|                           | DEMO_LVGL | DEMO_GFX | DEMO_TFT | DEMO_U8G2 | DEMO_AUDIO |
|:--------------------------|:-------:|:--------:|:--------:|:--------:|:---------:|
| 小鹏ESP32-S3 2.8寸开发板 [链接](https://www.xpstem.com/product/board-esp32s3-tft280)  | OK &#x2705;  |      |      |         | OK &#x2705;  |
| 小鹏ESP32-S3 创意电子套件 [链接](https://www.xpstem.com/product/xp-esp32s3-electronic-suit)  | OK  &#x2705;  |     |      |        | OK  &#x2705;  |
| 小鹏ESP32 物联网开发套件 [链接](https://www.xpstem.com/product/iot-devkit-suit)  | OK  &#x2705; |      | OK  &#x2705;  | OK  &#x2705;   |          |
| MatrixBit3 [链接](https://www.xpstem.com/product/matrixbit-v3)  |     |    | OK  &#x2705;   |         |      |
| 小鹏ESP32-S3 4.3寸开发板   |    |  OK  &#x2705;   |     |         |      |


### 三、运行DEMO

### 若本开发框架内置了你的开发板

1. 打开config.h文件，找到开发板，将定义值修改为1

2. 将要运行应用 APP_DEMO_xxx 的值修改为1

3. 设置开发板的型号、FLASH大小、PSRAM、分区等选项

4. 编译并上传运行

### 若本开发框架未内置你的开发板

1. 在boards文件夹创建一个目录，如youe_board，新建一个继承WifiBoard类（若不使用WiFi功能，也可以继承Board类）的自定义开发板类，然后实现Led、显示、音频等设备的驱动

2. 打开config.h文件，将要运行应用 APP_DEMO_xxx 的值修改为1

3. 设置开发板的型号、FLASH大小、PSRAM、分区等选项

4. 编译并上传运行

## 四、基于本框架的应用
* 小鹏AI语音机器人 
[产品链接](https://www.xpstem.com/product/xp-aichat-bot-2.8/) 
[源码链接](https://gitee.com/billyzh/XiaoZhi-Arduino)

* 小鹏IoT自动浇花套件 
[产品链接](https://www.xpstem.com/product/auto-watering/)
[源码链接](https://gitee.com/billyzh/auto-watering)

若你的开源应用使用了本框架并想显示在这里，请联系vx: billy_zh

-----
【&#x1f44d;赞赏&#x1f44d;】

![赞赏码](assets/like.png)