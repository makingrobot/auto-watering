/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_LED_WS2812==1

#ifndef _WS2812_LED_H_
#define _WS2812_LED_H_

#include <driver/gpio.h>
#include <atomic>
#include <mutex>
#include <vector>

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "led.h"
#include "../sys/timer.h"

/**
 * Ws2812灯珠类
 * 使用idf组件 led-strip 驱动
 */
class Ws2812Led : public Led {
public:
    Ws2812Led(gpio_num_t gpio, uint8_t num_pixels);
    virtual ~Ws2812Led();

    void TurnOn() override;
    void TurnOff() override;
    void BlinkOnce() override;
    void Blink(int times, int interval_ms) override;
    void SetColor(uint8_t r, uint8_t g, uint8_t b) override;

    void SetBrightness(uint8_t brightness) override { }
    void OnBlinkTimer();

    uint8_t num_pixels() const { return num_pixels_; }
    
    /**
     * 设置要点亮灯珠的编号，编号范围：0-num_pixels
     */
    void SetLightNo(const std::vector<uint8_t>& light_set);
    
private:
    const gpio_num_t pin_;
    const uint8_t num_pixels_;
    std::vector<uint8_t> light_set_ = { 0 };  // 默认只使用第1个灯珠

    std::mutex mutex_;
    TaskHandle_t blink_task_ = nullptr;
    uint8_t r_ = 0, g_ = 0, b_ = 0;
    int blink_counter_ = 0;
    int blink_interval_ms_ = 0;
    Timer* timer_ = nullptr;
    Adafruit_NeoPixel *pixels_ = nullptr;

    void StartContinuousBlink(int interval_ms);
    void StartBlinkTask(int times, int interval_ms);

    void Stop();
    
};

#endif // _WS2812_LED_H_

#endif //CONFIG_USE_LED_WS2812
