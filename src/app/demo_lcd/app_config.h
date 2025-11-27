#if APP_DEMO_LCD==1

#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H

//////////////////////////////////////////////////////////////////////////////////
// 特性

// 使用ESP_LOG
#define CONFIG_USE_ESP_LOG                  0

// WIFI配置
#define CONFIG_USE_WIFI                     1
#define CONFIG_WIFI_CONFIGURE_ENABLE        1  

// 时钟（刷新界面）
#define CONFIG_CLOCK_ENABLE                 1  

// OTA
#define CONFIG_OTA_ENABLE                   0  
#define CONFIG_OTA_URL                      ""


#endif //_APP_CONFIG_H

#endif