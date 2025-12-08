/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_LED_RGB==1

#include "rgb_led.h"
#include "../app/application.h"
#include "../sys/log.h"
#include <Arduino.h>

#define TAG "RgbLed"

#define BLINK_INFINITE          -1

// GPIO_LED
RgbLed::RgbLed(gpio_num_t r_pin, gpio_num_t g_pin, gpio_num_t b_pin, bool output_invert)
        : r_pin_(r_pin), g_pin_(g_pin), b_pin_(b_pin), output_invert_(output_invert) {

    assert(r_pin != GPIO_NUM_NC);
    assert(g_pin != GPIO_NUM_NC);
    assert(b_pin != GPIO_NUM_NC);

    pinMode(r_pin, OUTPUT);
    pinMode(g_pin, OUTPUT);
    pinMode(b_pin, OUTPUT);

    timer_ = new SwTimer("Rgb_Led");
}

RgbLed::~RgbLed() {
    if (timer_ != nullptr) {
        timer_->Stop();
    }
}

void RgbLed::SetColor(uint8_t r, uint8_t g, uint8_t b) {
    r_val_ = r;
    g_val_ = g;
    b_val_ = b;
}

void RgbLed::TurnOn() {
    std::lock_guard<std::mutex> lock(mutex_);
    blink_counter_ = 0;
    timer_->Stop();

    analogWrite(r_pin_, r_val_);
    analogWrite(g_pin_, g_val_);
    analogWrite(b_pin_, b_val_);
}

void RgbLed::TurnOff() {
    std::lock_guard<std::mutex> lock(mutex_);
    blink_counter_ = 0;
    timer_->Stop();

    analogWrite(r_pin_, 0);
    analogWrite(g_pin_, 0);
    analogWrite(b_pin_, 0);
}

void RgbLed::BlinkOnce() {
    Blink(1, 100);
}

void RgbLed::Blink(int times, int interval_ms) {
    StartBlinkTask(times, interval_ms);
}

void RgbLed::StartContinuousBlink(int interval_ms) {
    StartBlinkTask(BLINK_INFINITE, interval_ms);
}

void RgbLed::StartBlinkTask(int times, int interval_ms) {

    timer_->Stop();

    blink_counter_ = times * 2;
    blink_interval_ms_ = interval_ms;
    
    timer_->Start(interval_ms, [this](){ OnBlinkTimer(); });
}

void RgbLed::OnBlinkTimer() {
    
    std::lock_guard<std::mutex> lock(mutex_);
    blink_counter_--;
    if (blink_counter_ & 1) {
        analogWrite(r_pin_, r_val_);
        analogWrite(g_pin_, g_val_);
        analogWrite(b_pin_, b_val_);
    } else {
        analogWrite(r_pin_, 0);
        analogWrite(g_pin_, 0);
        analogWrite(b_pin_, 0);
    }

    if (blink_counter_ == 0) {
        timer_->Stop();
    }
}

#endif //CONFIG_USE_LED_RGB