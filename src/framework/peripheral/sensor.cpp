/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "sensor.h"
#include <Arduino.h>
#include "../sys/log.h"
#include "../sys/sw_timer.h"

#define TAG "Sensor"

Sensor::Sensor(gpio_num_t sensor_pin) {
    pinMode(sensor_pin_, INPUT);
    timer_ = new SwTimer("Sensor");
}

Sensor::~Sensor() {
    if (timer_ != nullptr) {
        timer_->Stop();
    }
}

/**
 * 启动传感器，
 * interval_ms: 数据采集间隔，单位毫秒
 */
void Sensor::Start(uint32_t interval_ms) {
    if (timer_ != nullptr) {
        timer_->Stop();
    }
    
    timer_->Start(interval_ms, [this](){ReadData();});
}

void Sensor::Stop() {
    if (timer_ != nullptr) {
        timer_->Stop();
    }
}

/**
 * 读取传感器数据
 */
void Sensor::ReadData() {
    sensor_val_ = new SensorValue();
    ReadValue(sensor_val_);

    if (on_newdata_callback_) {
        on_newdata_callback_(*sensor_val_);
    }
}
