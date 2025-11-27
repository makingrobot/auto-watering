/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_LED_RGB==1

#ifndef _RGB_LED_H_
#define _RGB_LED_H_

#include "led.h"
#include <driver/gpio.h>
#include <atomic>
#include <mutex>
#include <cstdint>
#include "src/sys/timer.h"
#include "src/sys/sw_timer.h"

/**
 * 三色LED
 */
class RgbLed : public Led {
 public:
    RgbLed(gpio_num_t r_pin, gpio_num_t g_pin, gpio_num_t b_pin, bool output_invert=false);
    virtual ~RgbLed();

    void TurnOn() override;
    void TurnOff() override;
    void BlinkOnce() override;
    void Blink(int times, int interval_ms) override;
    void SetBrightness(uint8_t brightness) override { }
    void SetColor(uint8_t r, uint8_t g, uint8_t b) override;

    void OnBlinkTimer();

 private:
    std::mutex mutex_;
    gpio_num_t r_pin_;
    gpio_num_t g_pin_;
    gpio_num_t b_pin_;
    bool output_invert_ = false;
    uint8_t r_val_ = 0;
    uint8_t g_val_ = 0;
    uint8_t b_val_ = 0;
    int blink_counter_ = 0;
    int blink_interval_ms_ = 0;
    Timer* timer_ = nullptr;

    void StartBlinkTask(int times, int interval_ms);
    void StartContinuousBlink(int interval_ms);

};

#endif  // _RGB_LED_H_

#endif //CONFIG_USE_LED_RGB