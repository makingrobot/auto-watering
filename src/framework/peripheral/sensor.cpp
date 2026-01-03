/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "sensor.h"
#include <Arduino.h>
#include "../sys/log.h"
#include "../sys/timer.h"
#include "../app/application.h"

#define TAG "Sensor"

Sensor::Sensor(const std::string& name) : name_(name) {
}

Sensor::~Sensor() {
    if (timer_ != nullptr) {
        timer_->Stop();
    }
}

/**
 * 启动传感器，
 * interval_ms: 数据采集间隔，单位毫秒
 */
void Sensor::Start(uint32_t interval_ms) {
    if (timer_ == nullptr) {
        timer_ = TimerFactory::CreateTimer("Sensor");
    } else {
        timer_->Stop();
    }
    
    timer_->Start(interval_ms, [this](){
        auto& app = Application::GetInstance();
        app.Schedule([this](){
            ReadData();
        });
    });
}

void Sensor::Stop() {
    if (timer_ != nullptr) {
        timer_->Stop();
    }
}

void Sensor::BindData() {
    on_newdata_callback_ = [this](const SensorValue &value) {
        auto& app = Application::GetInstance();
        app.OnSensorDataEvent(name_, value);
    };
}

/**
 * 读取传感器数据
 */
void Sensor::ReadData() {
    sensor_val_ = new SensorValue();
    ReadValue(sensor_val_);

    if (on_newdata_callback_) {
        on_newdata_callback_(*sensor_val_);
    }

    delete sensor_val_;
}

/*********** AnalogSensor **************/
AnalogSensor::AnalogSensor(const std::string& name, gpio_num_t pin) : Sensor(name),sensor_pin_(pin) {
    pinMode(sensor_pin_, INPUT);
}

void AnalogSensor::ReadValue(SensorValue *value) {
    Log::Debug("AnalogSensor", "Read value.");
    value->setIntValue(analogRead(sensor_pin_));
}

/*********** DigitalSensor **************/
DigitalSensor::DigitalSensor(const std::string& name, gpio_num_t pin) : Sensor(name),sensor_pin_(pin) {
    pinMode(sensor_pin_, INPUT);
}

void DigitalSensor::ReadValue(SensorValue *value) {
    Log::Debug("DigitalSensor", "Read value.");
    value->setIntValue(digitalRead(sensor_pin_));
}
