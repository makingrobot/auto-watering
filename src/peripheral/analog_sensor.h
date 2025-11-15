/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _ANALOG_SENSOR_H
#define _ANALOG_SENSOR_H

#include <driver/gpio.h>
#include <functional>
#include <Ticker.h>

#include "sensor.h"

/**
 * 模拟量传感器
 * 使用 Arduino-ESP32 的 AnalogRead 函数取值。
 */
class AnalogSensor : public Sensor {
public:
    AnalogSensor(gpio_num_t pin);
    virtual ~AnalogSensor();

    void OnNewData(std::function<void(int)> callback) { 
        on_newdata_callback_ = callback; 
    }
    void Start(uint32_t interval);
    void Stop();

    void ReadData();

private:
    gpio_num_t sensor_pin_;
    int sensor_val_;
    Ticker* sensor_ticker_;
    std::function<void(int)> on_newdata_callback_;

};

#endif //_ANALOG_SENSOR_H