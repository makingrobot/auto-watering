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
#include <Ticker.h>
#include <atomic>
#include <mutex>
#include <cstdint>

/**
 * 单色LED
 * 使用 Arduino-ESP32 的 ledc 封装函敏驱动
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
    std::mutex mutex_;
    gpio_num_t led_pin_;
    bool pwm_ = false;
    bool output_invert_ = false;
    uint8_t brightness_ = 255;
    int blink_counter_ = 0;
    int blink_interval_ms_ = 0;
    Ticker* blink_ticker_ = nullptr;

    void StartBlinkTask(int times, int interval_ms);

    void StartContinuousBlink(int interval_ms);
    void StartFadeTask();

};

#endif  // _GPIO_LED_H_

#endif //CONFIG_USE_LED_GPIO