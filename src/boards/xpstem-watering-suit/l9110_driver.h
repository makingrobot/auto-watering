
#include "src/peripheral/switch_actuator.h"

class L9110Driver : public SwitchActuator {
public:
    L9110Driver(gpio_num_t pin_in_a, gpio_num_t pin_in_b, bool output_invert) 
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
    }

    void On(uint8_t power) {
        analogWrite(pin_in_a_, power);
        analogWrite(pin_in_b_, 0);
    }

    void On() override {
        analogWrite(pin_in_a_, 128);  //128-255 太低会带不动电机。
        analogWrite(pin_in_b_, 0);
    }

    void Off() override {
        analogWrite(pin_in_a_, 0);
        analogWrite(pin_in_b_, 0);
    }

private:
    gpio_num_t pin_in_a_;
    gpio_num_t pin_in_b_;
    bool output_invert_ = false;

};