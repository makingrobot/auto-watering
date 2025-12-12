/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_LED_GPIO==1

#ifndef _GPIO_LED_H_
#define _GPIO_LED_H_

#include "led.h"
#include <driver/gpio.h>
#include <atomic>
#include <mutex>
#include <cstdint>
#include "../sys/timer.h"

/**
 * 单色LED
 */
class GpioLed : public Led {
 public:
    GpioLed(gpio_num_t gpio, bool pwm=true, bool output_invert=false);
    virtual ~GpioLed();

    void TurnOn() override;
    void TurnOff() override;
    void BlinkOnce() override;
    void Blink(int times, int interval_ms) override;
    void SetBrightness(uint8_t brightness) override;
    void SetColor(uint8_t r, uint8_t g, uint8_t b) override { }

    void OnBlinkTimer();

 private:
    const gpio_num_t led_pin_;
    const bool pwm_ = false;
    const bool output_invert_ = false;
    
    std::mutex mutex_;
    uint8_t brightness_ = 255;
    int blink_counter_ = 0;
    int blink_interval_ms_ = 0;
    Timer* timer_ = nullptr;

    void StartBlinkTask(int times, int interval_ms);
    void StartContinuousBlink(int interval_ms);

};

#endif  // _GPIO_LED_H_

#endif //CONFIG_USE_LED_GPIO