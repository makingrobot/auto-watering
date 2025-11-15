/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "adc_battery_monitor.h"
#include "src/sys/log.h"
#include <Arduino.h>

#define TAG "AdcBatteryMonitor"

static const battery_point_t battery_point_table[]={
            { 4.2 ,  100},
            { 4.06 ,  80},
            { 3.82 ,  60},
            { 3.58 ,  40},
            { 3.34 ,  20},
            { 3.1 ,  0},
            { 3.0 ,  -10}
        };

void TimerCallback(AdcBatteryMonitor *arg) {
    arg->CheckBatteryStatus();
}

AdcBatteryMonitor::AdcBatteryMonitor(gpio_num_t charging_pin, adc_unit_t adc_unit, adc_channel_t adc_channel, float upper_resistor, float lower_resistor)
    : AdcBatteryMonitor(charging_pin, adc_unit, adc_channel, upper_resistor, lower_resistor, battery_point_table) {

}

AdcBatteryMonitor::AdcBatteryMonitor(gpio_num_t charging_pin, adc_unit_t adc_unit, adc_channel_t adc_channel, float upper_resistor, float lower_resistor, const battery_point_t *point_table)
    : charging_pin_(charging_pin) {
    
    // Initialize charging pin
    gpio_config_t gpio_cfg = {
        .pin_bit_mask = 1ULL << charging_pin,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&gpio_cfg));

    adc_battery_estimation_t adc_cfg = {
        .internal = {
            .adc_unit = adc_unit,
            .adc_bitwidth = ADC_BITWIDTH_12, 
            .adc_atten = ADC_ATTEN_DB_11, 
        },
        .adc_channel = adc_channel,
        .upper_resistor = upper_resistor,
        .lower_resistor = lower_resistor,
        //.battery_points = point_table,
        //.battery_points_count = sizeof(point_table) / sizeof(battery_point_t)
    };
    adc_cfg.charging_detect_cb = [](void *user_data) -> bool {
        AdcBatteryMonitor *self = (AdcBatteryMonitor *)user_data;
        return gpio_get_level(self->charging_pin_) == 1;
    };
    adc_cfg.charging_detect_user_data = this;
    adc_battery_estimation_handle_ = adc_battery_estimation_create(&adc_cfg);

    monitor_timer_ = new Ticker();
    monitor_timer_->attach(20, TimerCallback, this);
    Log::Info(TAG, "monitor ticker started");
    Serial.println("");
}

AdcBatteryMonitor::~AdcBatteryMonitor() {
    if (monitor_timer_!=nullptr) {
        monitor_timer_->detach();
        monitor_timer_ = nullptr;
    }
    if (adc_battery_estimation_handle_) {
        adc_battery_estimation_destroy(adc_battery_estimation_handle_);
    }
}

bool AdcBatteryMonitor::IsCharging() {
    bool is_charging = false;
    ESP_ERROR_CHECK(adc_battery_estimation_get_charging_state(adc_battery_estimation_handle_, &is_charging));
    return is_charging;
}

bool AdcBatteryMonitor::IsDischarging() {
    // 没有区分充电和放电，所以直接返回相反状态
    return !is_charging_;
}

int AdcBatteryMonitor::GetBatteryLevel() {
    float capacity = 0;
    int adc_value = 0;
    ESP_ERROR_CHECK(adc_battery_estimation_get_capacity(adc_battery_estimation_handle_, &adc_value, &capacity));
    return capacity;
}

void AdcBatteryMonitor::OnChargingStatusChanged(std::function<void(bool)> callback) {
    on_charging_status_changed_ = callback;
}

void AdcBatteryMonitor::CheckBatteryStatus() {
    float capacity = 0;
    int adc_value = 0;
    ESP_ERROR_CHECK(adc_battery_estimation_get_capacity(adc_battery_estimation_handle_, &adc_value, &capacity));
    Log::Debug(TAG, "Battery adc value: %d, capacity: %.1f%%", adc_value, capacity);

    bool new_charging_status = IsCharging();
    if (new_charging_status != is_charging_) {
        is_charging_ = new_charging_status;
        if (on_charging_status_changed_) {
            on_charging_status_changed_(is_charging_);
        }
    }
}
