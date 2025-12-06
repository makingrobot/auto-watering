#if APP_DEMO_LVGL==1

#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H

//////////////////////////////////////////////////////////////////////////////////
// 特性

// WIFI配置
#define CONFIG_USE_WIFI                     1
#define CONFIG_WIFI_CONFIGURE_ENABLE        1  
#define CONFIG_WIFI_CONFIGURE_WEBSERVER        1  

// 时钟（刷新界面）
#define CONFIG_CLOCK_ENABLE                 1  

// OTA
#define CONFIG_OTA_ENABLE                   0  
#define CONFIG_OTA_URL                      ""

#if CONFIG_USE_LVGL==0
#error "本DEMO需要使用LVGL图形库，请在开发板的board_config.h内设置。"
#endif

#endif //_APP_CONFIG_H

#endif