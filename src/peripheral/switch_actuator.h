/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _SWITCH_EXECUTOR_H
#define _SWITCH_EXECUTOR_H

#include <Arduino.h>
#include "actuator.h"

/**
 * 开关类执行器，如继电器，小电机驱动等
 */
class SwitchActuator : public Actuator {
public:
    SwitchActuator(gpio_num_t pin, bool output_invert) 
            : sensor_pin_(pin), output_invert_(output_invert) { 

        pinMode(sensor_pin_, OUTPUT);
        Off();
    }

    virtual ~SwitchActuator() { 
        Off(); 
    };

    virtual void On() {
        digitalWrite(sensor_pin_, output_invert_ ? 0 : 1);
        state_ = 1; //on
    }

    virtual void Off() {
        digitalWrite(sensor_pin_, output_invert_ ? 1 : 0);
        state_ = 0; //off
    }

private:
    gpio_num_t sensor_pin_;
    bool output_invert_ = false;
    uint8_t state_;

};

#endif //_SWITCH_EXECUTOR_H