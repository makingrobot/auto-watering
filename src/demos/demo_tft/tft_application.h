/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_TFT==1

#ifndef _TFT_APPLICATION_H
#define _TFT_APPLICATION_H

#include "src/framework/app/application.h"

class TftApplication : public Application {
public:
    TftApplication();
    ~TftApplication();
    
    void OnInit() override;

    const std::string& GetAppName() const override { return "DEMO_TFT"; }
    const std::string& GetAppVersion() const override { return "1.0.0"; }

private:

};

#endif //_TFT_APPLICATION_H

#endif //APP_DEMO_TFT