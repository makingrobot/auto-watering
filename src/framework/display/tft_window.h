/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_TFT_ESPI == 1

#ifndef TFT_WINDOW_H
#define TFT_WINDOW_H

#include <TFT_eSPI.h>
#include <string>
#include <vector>

class TftWindow {
public:
    TftWindow() { }

    virtual void Setup(TFT_eSPI* driver);
    virtual void SetStatus(const std::string& status);
    virtual void SetText(uint8_t line, const std::string& text);
   
private:

    TFT_eSPI* driver_ = nullptr;
    std::string status_ = "";
    std::vector<std::string> text_line_(5);
};

#endif //TFT_WINDOW_H

#endif //CONFIG_USE_TFT_ESPI