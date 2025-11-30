#if APP_DEMO_GFX==1

#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H

//////////////////////////////////////////////////////////////////////////////////
// 特性

// 使用ESP_LOG
#define CONFIG_USE_ESP_LOG                  0

// WIFI配置
#define CONFIG_USE_WIFI                     1
#define CONFIG_WIFI_CONFIGURE_ENABLE        0  

// 时钟（刷新界面）
#define CONFIG_CLOCK_ENABLE                 0  

// OTA
#define CONFIG_OTA_ENABLE                   0  
#define CONFIG_OTA_URL                      ""

#if CONFIG_USE_GFX_LIBRARY==0
#error "本DEMO需要使用GFX Library图形库，请在开发板的board_config.h内设置。"
#endif

#endif //_APP_CONFIG_H

#endif