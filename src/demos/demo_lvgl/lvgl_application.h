/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_LVGL==1

#ifndef _LVGL_APPLICATION_H
#define _LVGL_APPLICATION_H

#include "src/framework/app/application.h"
#include "src/framework/display/lvgl_text_window.h";

class LvglApplication : public Application {
public:
    LvglApplication();
    ~LvglApplication();
    
    void OnInit() override;

    const std::string& GetAppName() const override { return "DEMO_LCD"; }
    const std::string& GetAppVersion() const override { return "1.0.0"; }

private:
    LvglTextWindow *window_ = nullptr;

};

#endif //_LVGL_APPLICATION_H

#endif //APP_DEMO_LVGL