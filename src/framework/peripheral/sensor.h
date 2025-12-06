/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _SENSOR_H
#define _SENSOR_H

/**
 * 传感器类基类，如温湿度、人体感应、陀螺仪等
 */
class Sensor {
public:
    Sensor(gpio_num_t pin);
    virtual ~Sensor();

    void OnNewData(std::function<void(const SensorValue&)> callback) { 
        on_newdata_callback_ = callback; 
    }
    void Start(uint32_t interval);
    void Stop();

    virtual void ReadData();

protected:
    virtual void ReadValue(SensorValue *value) = 0;

    std::function<void(const SensorValue&)> on_newdata_callback_;
    gpio_num_t sensor_pin_;

private:
    SensorValue *sensor_val_;
    Timer* timer_ = nullptr;

};

/**
 * 一般模拟量传感器
 */
class AnalogSensor : public Sensor {
public:
    AnalogSensor(gpio_num_t pin) : Sensor(pin) { }

protected:
    void ReadValue(SensorValue *value) override {
        value->setIntValue(analogRead(sensor_pin_));
    }

};

/**
 * 一般数字量传感器
 */
class DigitalSensor : public Sensor {
public:
    DigitalSensor(gpio_num_t pin) : Sensor(pin) { }

protected:
    void ReadValue(SensorValue *value) override {
        value->setIntValue(digitalRead(sensor_pin_));
    }

};

#endif //_SENSOR_H