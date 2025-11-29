/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_GFX==1

#ifndef _GFX_APPLICATION_H
#define _GFX_APPLICATION_H

#include "src/framework/app/application.h"
#include "benchmark_window.h"

class GfxApplication : public Application {
public:
    GfxApplication();
    ~GfxApplication();
    
    void OnInit() override;

    const std::string& GetAppName() const override { return "DEMO_GFX"; }
    const std::string& GetAppVersion() const override { return "1.0.0"; }

private:
    BenchmarkWindow* window_;
    
};

#endif //_GFX_APPLICATION_H

#endif //APP_DEMO_GFX