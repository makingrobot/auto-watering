/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_U8G2 == 1

#include "u8g2_display.h"
#include "src/sys/log.h"

#define TAG "U8g2Display"

U8g2Display::U8g2Display(U8G2* driver, int width, int height, const uint8_t* fonts) 
        : driver_(driver), fonts_(fonts) {
            
    Log::Debug(TAG, "U8g2Display constructor.");
    width_ = width; 
    height_ = height;
}

U8g2Display::~U8g2Display() {

}

bool U8g2Display::Lock(int timeout_ms) {
    return true;
}
 
void U8g2Display::Unlock() {
    
}

void U8g2Display::Init() {

    driver_->begin();
    driver_->enableUTF8Print();		
    driver_->setFont(fonts_);
}
    
void U8g2Display::SetStatus(const char* status) {
    status_ = status;

    Update();
}

void U8g2Display::SetText(const char* text) {
    text_ = text;

    Update();
}

void U8g2Display::Update() {
    
    driver_->clearBuffer();

    driver_->setCursor(4, 20);   
    driver_->print(status_); 

    driver_->setCursor(4, 40); 
    driver_->print(text_);

    driver_->sendBuffer();
}

#endif //CONFIG_USE_U8G2