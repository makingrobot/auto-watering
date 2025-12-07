/**
 * Iot-Devkit-Examples
 * Iot开发套件示例，基于ESP32-Arduino-Framework应用开发框架。
 * 
 * 套件详情：https://www.xpstem.com/product/iot-devkit-suit
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if BOARD_XPSTEM_IOT_DEVKIT_SUIT==1

#ifndef _DHT11_H
#define _DHT11_H

#include <Arduino.h>
#include <DHT.h>
#include <vector>

#include "src/framework/peripheral/sensor.h"

class Dht11Sensor : public Sensor {
public:
    Dht11Sensor(gpio_num_t pin) : Sensor() {
        dht_ = new DHT(pin, DHT11);
        dht_->begin();
    }

    void ReadValue(SensorValue *value) override {
        float shidu = dht_->readHumidity();
        float wendu = dht_->readTemperature();

        std::vector<float> list;
        list.push_back(shidu);
        list.push_back(wendu);

        value->setFloatList(list);
    }

private:
    DHT* dht_;
    
};

#endif //_DHT11_H

#endif