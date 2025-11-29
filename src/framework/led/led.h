/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#include <cstdint>

#ifndef _LED_H_
#define _LED_H_

#define DEFAULT_BRIGHTNESS      4
#define HIGH_BRIGHTNESS         16
#define LOW_BRIGHTNESS          2

#define BLINK_INFINITE          -1

class Led {
public:
    virtual ~Led() = default;

    virtual void TurnOn() = 0;
    virtual void TurnOff() = 0;
    virtual void BlinkOnce() = 0;
    virtual void Blink(int times, int interval_ms) = 0;
    virtual void SetBrightness(uint8_t brightness) = 0;
    virtual void SetColor(uint8_t r, uint8_t g, uint8_t b) = 0;
};


class NoLed : public Led {
public:
    void TurnOn() override { }
    void TurnOff() override { }
    void BlinkOnce() override { }
    void Blink(int times, int interval_ms) override { }
    void SetBrightness(uint8_t brightness) override { }
    void SetColor(uint8_t r, uint8_t g, uint8_t b) override { }
};

#endif // _LED_H_