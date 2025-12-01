#include "config.h"
#if APP_DEMO_TFT==1

#ifndef _TEST_WINDOW_H
#define _TEST_WINDOW_H

#include <TFT_eSPI.h>
#include <string>

class TestWindow : public TftWindow {
public:
    void Setup(TFT_eSPI* driver) override {
        driver_ = driver;
    }

    void SetStatus(const std::string& status) override { }
    void SetText(uint8_t line, const std::string& text) override { }
   
    void Start();

private:

    TFT_eSPI* driver = nullptr;

};

#endif //_TEST_WINDOW_H

#endif