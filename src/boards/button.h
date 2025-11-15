/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef BUTTON_H_
#define BUTTON_H_

#include <functional>
#include <string>
#include <driver/gpio.h>

#include "src/libs/button/button_types.h"
#include "src/libs/button/button_adc.h"
#include "src/libs/button/button_gpio.h"

class Button {
public:
    Button(const std::string& name, button_handle_t button_handle);
    Button(const std::string& name, gpio_num_t gpio_num, bool active_high = false, uint16_t long_press_time = 0, uint16_t short_press_time = 0, bool enable_power_save = false);
    ~Button();

    const std::string& name() const { return name_; }

    void OnPressDown(std::function<void()> callback);
    void OnPressUp(std::function<void()> callback);
    void OnLongPress(std::function<void()> callback);
    void OnClick(std::function<void()> callback);
    void OnDoubleClick(std::function<void()> callback);
    void OnMultipleClick(std::function<void()> callback, uint8_t click_count = 3);

protected:
    std::string name_;
    gpio_num_t gpio_num_;
    button_handle_t button_handle_ = nullptr;

    std::function<void()> on_press_down_;
    std::function<void()> on_press_up_;
    std::function<void()> on_long_press_;
    std::function<void()> on_click_;
    std::function<void()> on_double_click_;
    std::function<void()> on_multiple_click_;
};

class AdcButton : public Button {
public:
    AdcButton(const std::string& name, const button_adc_config_t& adc_config);
};

class PowerSaveButton : public Button {
public:
    PowerSaveButton(const std::string& name, gpio_num_t gpio_num) : Button(name_, gpio_num, false, 0, 0, true) {
    }
};

#endif // BUTTON_H_
