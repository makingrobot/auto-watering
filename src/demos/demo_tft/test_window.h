#include "config.h"
#if APP_DEMO_TFT==1

#ifndef _TEST_WINDOW_H
#define _TEST_WINDOW_H

#include <TFT_eSPI.h>
#include <string>
#include <freertos/FreeRTOS.h>
#include "src/framework/display/tft_window.h"

class TestWindow : public TftWindow {
public:
    TestWindow();

    void Setup(TFT_eSPI* driver) override {
        driver_ = driver;
    }

    void SetStatus(const std::string& status) override { }
    void SetText(uint8_t line, const std::string& text) override { }
   
    void Start();

private:

    TFT_eSPI* driver_ = nullptr;
    TaskHandle_t test_handle_;

};

#endif //_TEST_WINDOW_H

#endif