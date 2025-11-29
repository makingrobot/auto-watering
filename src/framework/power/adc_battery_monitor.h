/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _ADC_BATTERY_MONITOR_H
#define _ADC_BATTERY_MONITOR_H

#include <functional>
#include <vector>
#include <driver/gpio.h>
#include "../sys/timer.h"
#include "src/libs/adc_battery_estimation/adc_battery_estimation.h"

/**
 * 锂电池电量检测
 * 注意：本类默认电量与电压对应表仅适用于3.7V锂电池
 * 若是其他类型充电电池，请传入自定义的电量与电压对应表（battery_point_t[]）
 */
class AdcBatteryMonitor {
public:
    AdcBatteryMonitor(gpio_num_t charging_pin, adc_unit_t adc_unit, adc_channel_t adc_channel, float upper_resistor, float lower_resistor);
    AdcBatteryMonitor(gpio_num_t charging_pin, adc_unit_t adc_unit, adc_channel_t adc_channel, float upper_resistor, float lower_resistor, const battery_point_t* points);
    ~AdcBatteryMonitor();

    bool IsCharging();
    bool IsDischarging();
    int GetBatteryLevel();
    void CheckBatteryStatus();

    void OnChargingStatusChanged(std::function<void(bool)> callback);

private:
    gpio_num_t charging_pin_;
    adc_battery_estimation_handle_t adc_battery_estimation_handle_ = nullptr;
    bool is_charging_ = false;
    std::function<void(bool)> on_charging_status_changed_;
    Timer* timer_ = nullptr;
    
};

#endif //_ADC_BATTERY_MONITOR_H
