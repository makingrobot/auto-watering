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

#define TAG "Ws2812Led"

Ws2812Led::Ws2812Led(gpio_num_t gpio) {
    // If the gpio is not connected, you should use NoLed class
    assert(gpio != GPIO_NUM_NC);

    led_strip_config_t strip_config = {};
    strip_config.strip_gpio_num = gpio;
    strip_config.max_leds = 1;
    strip_config.led_pixel_format = LED_PIXEL_FORMAT_GRB;
    strip_config.led_model = LED_MODEL_WS2812;

    led_strip_rmt_config_t rmt_config = {};
    rmt_config.resolution_hz = 10 * 1000 * 1000; // 10MHz

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip_));
    led_strip_clear(led_strip_);

    blink_ticker_ = new Ticker();
}

Ws2812Led::~Ws2812Led() {
    Stop();

    if (led_strip_ != nullptr) {
        led_strip_del(led_strip_);
    }
}

void Ws2812Led::Stop() {
    if( blink_ticker_!=nullptr ) {
        blink_ticker_->detach();
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
    if (led_strip_ == nullptr) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    Stop();

    led_strip_set_pixel(led_strip_, 0, r_, g_, b_);
    led_strip_refresh(led_strip_);
}

void Ws2812Led::TurnOff() {
    Log::Debug(TAG, "turn off");
    if (led_strip_ == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    Stop();

    led_strip_clear(led_strip_);
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

void TickerCallback(Ws2812Led *arg) {
    arg->OnBlinkTimer();
}

void Ws2812Led::StartBlinkTask(int times, int interval_ms) {
    if (led_strip_ == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    Stop();
    
    blink_counter_ = times * 2;
    blink_interval_ms_ = interval_ms;
    
    blink_ticker_->attach_ms(interval_ms, TickerCallback, this);
    Log::Info( TAG, "blink timer started" );
}

void Ws2812Led::OnBlinkTimer() {
    std::lock_guard<std::mutex> lock(mutex_);
    blink_counter_--;
    if (blink_counter_ & 1) {
        led_strip_set_pixel(led_strip_, 0, r_, g_, b_);
        led_strip_refresh(led_strip_);
    } else {
        led_strip_clear(led_strip_);

        if (blink_counter_ == 0) {
            Stop();
        }
    }
}

#endif //CONFIG_USE_LED_WS2812