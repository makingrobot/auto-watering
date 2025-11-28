/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_LED_WS2812==1

#include "ws2812_led.h"
#include "src/sys/log.h"
#include "src/sys/sw_timer.h"

#define TAG "Ws2812Led"

Ws2812Led::Ws2812Led(gpio_num_t pin, uint8_t num_pixels) : pin_(pin), num_pixels_(num_pixels) {
    pixels_ = new Adafruit_NeoPixel(num_pixels, pin, NEO_GRB + NEO_KHZ800);
    pixels_->begin();

    timer_ = new SwTimer("Ws2812_Led");
}

Ws2812Led::~Ws2812Led() {
    Stop();

    pixels_->clear();
}

void Ws2812Led::Stop() {
    if (timer_ != nullptr) {
        timer_->Stop();
    }
}

void Ws2812Led::SetColor(uint8_t r, uint8_t g, uint8_t b) {
    Log::Info(TAG, "Set color r:%d, g:%d, b:%d", r, g, b);
    r_ = r;
    g_ = g;
    b_ = b;
}

void Ws2812Led::TurnOn() {
    Log::Debug(TAG, "turn on");
    if (pixels_ == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    Stop();

    pixels_->setPixelColor(0, pixels_->Color(r_, g_, b_));
    pixels_->show();
}

void Ws2812Led::TurnOff() {
    Log::Debug(TAG, "turn off");
    if (pixels_ == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    Stop();

    pixels_->clear();
}

void Ws2812Led::BlinkOnce() {
    Log::Debug(TAG, "blink");
    Blink(1, 100);
}

void Ws2812Led::Blink(int times, int interval_ms) {
    Log::Debug(TAG, "blink %d times", times);
    StartBlinkTask(times, interval_ms);
}

void Ws2812Led::StartContinuousBlink(int interval_ms) {
    StartBlinkTask(BLINK_INFINITE, interval_ms);
}

void Ws2812Led::StartBlinkTask(int times, int interval_ms) {
    if (pixels_ == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    Stop();
    
    blink_counter_ = times * 2;
    blink_interval_ms_ = interval_ms;
    
    timer_->Start(interval_ms, [this](){ OnBlinkTimer(); });
}

void Ws2812Led::OnBlinkTimer() {
    std::lock_guard<std::mutex> lock(mutex_);
    blink_counter_--;
    if (blink_counter_ & 1) {
        pixels_->setPixelColor(0, pixels_->Color(r_, g_, b_));
        pixels_->show();
    } else {
        pixels_->clear();

        if (blink_counter_ == 0) {
            Stop();
        }
    }
}

#endif //CONFIG_USE_LED_WS2812