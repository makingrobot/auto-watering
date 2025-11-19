/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "analog_sensor.h"
#include <Arduino.h>
#include "src/sys/log.h"

#define TAG "AnalogSensor"

AnalogSensor::AnalogSensor(gpio_num_t sensor_pin) : sensor_pin_(sensor_pin) {

}

AnalogSensor::~AnalogSensor() {
    if (timer_handle_ != nullptr) {
        xTimerDelete(timer_handle_, 0);
    }
}

void timerCallback(TimerHandle_t param) {
    AnalogSensor* sensor = (AnalogSensor*)pvTimerGetTimerID(param);
    sensor->ReadData();
}

void AnalogSensor::Start(uint32_t interval) {
    if (timer_handle_ != nullptr) {
        xTimerDelete(timer_handle_, 0);
    }
    
    timer_handle_ = xTimerCreate("Analog_Timer", pdMS_TO_TICKS(interval * 1000), pdTRUE, this, timerCallback);
    if (xTimerStart(timer_handle_, pdMS_TO_TICKS(1000)) == pdPASS) {
        Log::Info(TAG, "sensor timer started.");
    } else {
        Log::Warn(TAG, "sensor timer start failure.");
    }
}

void AnalogSensor::Stop() {
    if (timer_handle_ != nullptr) {
        xTimerDelete(timer_handle_, 0);
    }
}

void AnalogSensor::ReadData() {
    int count = 3;
    int total_val = 0;
    for (int i=0; i<count; i++) {
        total_val += analogRead(sensor_pin_);
        vTaskDelay(pdMS_TO_TICKS(1)); // 10ms
    }

    // 取均值
    sensor_val_ = total_val / count;

    if (on_newdata_callback_) {
        on_newdata_callback_(sensor_val_);
    }
}