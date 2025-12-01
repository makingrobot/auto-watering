/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_GFX==1

#ifndef _BENCHMARK_WINDOW_H
#define _BENCHMARK_WINDOW_H

#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#include "src/framework/display/gfx_window.h"
#include "src/framework/sys/log.h"

#define TAG "BenchmarkWindow"

class BenchmarkWindow : public GfxWindow {
public:
    void Setup(Arduino_GFX* driver) override {
        driver_ = driver;
    }

    void SetStatus(const std::string& status) override { }
    void SetText(const std::string& text) override { }
   
    void Start();

private:

    Arduino_GFX* driver_ = nullptr;

};

#endif // _BENCHMARK_WINDOW_H

#endif