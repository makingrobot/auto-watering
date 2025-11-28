/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_U8G2==1

#ifndef _U8G2_APPLICATION_H
#define _U8G2_APPLICATION_H

#include "../application.h"

class U8g2Application : public Application {
public:
    U8g2Application();
    ~U8g2Application();
    
    void OnInit() override;

    const std::string& GetAppName() const override { return "DEMO_U8G2"; }
    const std::string& GetAppVersion() const override { return "1.0.0"; }

private:

};

#endif //_U8G2_APPLICATION_H

#endif //APP_DEMO_U8G2