/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（vx: billyzh）
 */
#ifndef _L9110_DRIVER_H
#define _L9110_DRIVER_H

#include "src/framework/peripheral/switch_actuator.h"

#define TAG "PumpDriver"

/**
 * 水泵驱动
 */
class PumpDriver : public SwitchActuator {
public:
    PumpDriver(gpio_num_t pin_in_a, gpio_num_t pin_in_b, bool output_invert=false) 
            : SwitchActuator(pin_in_a, output_invert), output_invert_(output_invert) { 

        if (output_invert) {
            pin_in_a_ = pin_in_b;
            pin_in_b_ = pin_in_a;
        } else {
            pin_in_a_ = pin_in_a;
            pin_in_b_ = pin_in_b;
        }

        pinMode(pin_in_a_, OUTPUT);
        pinMode(pin_in_b_, OUTPUT);

        analogWrite(pin_in_a_, 0);
        analogWrite(pin_in_b_, 0);
    }

    void On(uint8_t power) {
        Log::Info(TAG, " On.");

        analogWrite(pin_in_a_, power);
        analogWrite(pin_in_b_, 0);
    }

    void On() override {
        On(192);  //128-255 太低会带不动电机。
    }

    void Off() override {
        analogWrite(pin_in_a_, 0);
        analogWrite(pin_in_b_, 0);

        Log::Info(TAG, " Off.");
    }

private:
    gpio_num_t pin_in_a_;
    gpio_num_t pin_in_b_;
    bool output_invert_ = false;

};

#endif //_L9110_DRIVER_H