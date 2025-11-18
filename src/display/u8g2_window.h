/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_U8G2 == 1

#ifndef U8G2_WINDOW_H
#define U8G2_WINDOW_H

#include <U8g2lib.h>
#include <string>

class U8g2Window {
public:
    U8g2Window() { }
    virtual ~U8g2Window() = default;

    virtual void Setup(U8G2* driver, const uint8_t* fonts);
    virtual void SetStatus(const std::string& status);
    virtual void SetText(uint8_t line, const std::string& text);
   
private:
    void Update();

    U8G2* driver_ = nullptr;
    std::string status_ = "";
    std::string text_line_[3] = {"", "", ""};
};

#endif //U8g2_WINDOW_H

#endif //CONFIG_USE_U8G2