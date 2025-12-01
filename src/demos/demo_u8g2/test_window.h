#include "config.h"
#if APP_DEMO_U8G2==1

#ifndef _TEST_WINDOW_H
#define _TEST_WINDOW_H

#include "src/framework/display/u8g2_window.h"

#include <U8g2lib.h>

class TestWindow : public U8g2Window {
public:

    virtual void Setup(U8G2* driver, const uint8_t* fonts) {
        driver_ = driver;
    }

    virtual void SetStatus(const std::string& status) { }
    virtual void SetText(uint8_t line, const std::string& text) { }

    void Start();

private:

    U8G2* driver_ = nullptr;

};

#endif //_TEST_WINDOW_H

#endif