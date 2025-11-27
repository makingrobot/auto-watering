/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "analog_sensor.h"
#include <Arduino.h>
#include "src/sys/log.h"
#include "src/sys/sw_timer.h"

#define TAG "AnalogSensor"

AnalogSensor::AnalogSensor(gpio_num_t sensor_pin) : sensor_pin_(sensor_pin) {
    pinMode(sensor_pin_, INPUT);
    timer_ = new SwTimer("Analog");
}

AnalogSensor::~AnalogSensor() {
    if (timer_ != nullptr) {
        timer_->Stop();
    }
}

/**
 * 启动传感器，
 * interval_ms: 数据采集间隔，单位毫秒
 */
void AnalogSensor::Start(uint32_t interval_ms) {
    if (timer_ != nullptr) {
        timer_->Stop();
    }
    
    timer_->Start(interval_ms, [this](){ReadData();});
}

void AnalogSensor::Stop() {
    if (timer_ != nullptr) {
        timer_->Stop();
    }
}

/**
 * 读取传感器数据
 */
void AnalogSensor::ReadData() {
    sensor_val_ = new SensorValue();
    sensor_val_->setIntValue(analogRead(sensor_pin_));

    if (on_newdata_callback_) {
        on_newdata_callback_(*sensor_val_);
    }
}