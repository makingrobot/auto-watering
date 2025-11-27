/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_LED_GPIO==1

#include "gpio_led.h"
#include "src/app/application.h"
#include "src/sys/log.h"
#include <Arduino.h>

#define TAG "GpioLed"

#define BLINK_INFINITE          -1

// GPIO_LED
GpioLed::GpioLed(gpio_num_t gpio, bool pwm, bool output_invert)
        : led_pin_(gpio), pwm_(pwm), output_invert_(output_invert) {

    assert(gpio != GPIO_NUM_NC);

    pinMode(gpio, OUTPUT);
    timer_ = new SwTimer("Gpio_Led");
}

GpioLed::~GpioLed() {
    if (timer_ != nullptr) {
        timer_->Stop();
    }
}

void GpioLed::SetBrightness(uint8_t brightness) {
    brightness_ = brightness;
    if (pwm_) {
        analogWrite(led_pin_, brightness);
    } else {
        digitalWrite(led_pin_, brightness > 127 ? HIGH : LOW);
    }
}

void GpioLed::TurnOn() {
    blink_counter_ = 0;
    timer_->Stop();

    if (pwm_) {
        analogWrite(led_pin_, output_invert_ ? 0 : brightness_);
    } else {
        digitalWrite(led_pin_, output_invert_ ? LOW : HIGH);
    }
}

void GpioLed::TurnOff() {
    blink_counter_ = 0;
    timer_->Stop();

    if (pwm_) {
        analogWrite(led_pin_, output_invert_ ? brightness_ : 0);
    } else {
        digitalWrite(led_pin_, output_invert_ ? HIGH : LOW);
    }
}

void GpioLed::BlinkOnce() {
    Blink(1, 100);
}

void GpioLed::Blink(int times, int interval_ms) {
    StartBlinkTask(times, interval_ms);
}

void GpioLed::StartContinuousBlink(int interval_ms) {
    StartBlinkTask(BLINK_INFINITE, interval_ms);
}

void GpioLed::StartBlinkTask(int times, int interval_ms) {

    timer_->Stop();

    blink_counter_ = times * 2;
    blink_interval_ms_ = interval_ms;
    
    timer_->Start(interval_ms, [this](){ OnBlinkTimer(); });
}

void GpioLed::OnBlinkTimer() {
    
    std::lock_guard<std::mutex> lock(mutex_);
    blink_counter_--;
    if (blink_counter_ & 1) {
        if (pwm_) {
            analogWrite(led_pin_, output_invert_ ? 0 : brightness_);
        } else {
            digitalWrite(led_pin_, output_invert_ ? LOW : HIGH);
        }
    } else {
        if (pwm_) {
            analogWrite(led_pin_, output_invert_ ? brightness_ : 0);
        } else {
            digitalWrite(led_pin_, output_invert_ ? HIGH : LOW);
        }
    }

    if (blink_counter_ == 0) {
        timer_->Stop();
    }
}

#endif //CONFIG_USE_LED_GPIO