/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _SENSOR_H
#define _SENSOR_H

#include <functional>
#include <driver/gpio.h>

#include "sensor_value.h"
#include "../sys/timer.h"

/**
 * 传感器类基类，如温湿度、人体感应、陀螺仪等
 */
class Sensor {
public:
    Sensor();
    virtual ~Sensor();

    /**
     * 启动传感器，
     * interval_ms: 数据采集间隔，单位毫秒
     */
    void Start(uint32_t interval_ms);
    void Stop();

    void OnNewData(std::function<void(const SensorValue&)> callback) { 
        on_newdata_callback_ = callback; 
    }
    
    virtual void ReadData();

protected:
    /**
     * 读取传感器数据，由派生类实现。
     */
    virtual void ReadValue(SensorValue *value) = 0;

private:
    std::function<void(const SensorValue&)> on_newdata_callback_;
    SensorValue *sensor_val_;
    Timer* timer_ = nullptr;

};

/**
 * 一般模拟量传感器
 */
class AnalogSensor : public Sensor {
public:
    AnalogSensor(gpio_num_t pin);

protected:
    void ReadValue(SensorValue *value) override;

private:
    const gpio_num_t sensor_pin_;

};

/**
 * 一般数字量传感器
 */
class DigitalSensor : public Sensor {
public:
    DigitalSensor(gpio_num_t pin);

protected:
    void ReadValue(SensorValue *value) override;

private:
    const gpio_num_t sensor_pin_;

};

#endif //_SENSOR_H