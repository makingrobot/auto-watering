/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_GFX_LIBRARY == 1

#ifndef TFT_WINDOW_H
#define TFT_WINDOW_H

#include <Arduino_GFX_Library.h>
#include <string>

class GfxWindow {
public:
    virtual void Setup(Arduino_GFX* driver);
    virtual void SetStatus(const std::string& status);
    virtual void SetText(const std::string& text);
   
private:
    void Update();
    
    Arduino_GFX* driver_ = nullptr;
    std::string status_ = "";
    std::string text_ = "";
};

#endif //TFT_WINDOW_H

#endif //CONFIG_USE_TFT_ESPI