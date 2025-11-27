/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _WIFI_CONFIGURATION_EX_H
#define _WIFI_CONFIGURATION_EX_H

#include <Arduino.h>
#include <WebServer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "src/wifi/wifi_configuration.h"

class WifiConfigurationEx : public WifiConfiguration {
public:
    WifiConfigurationEx() { }

protected:
    void StartWebServer() override;

private:
    bool ReadProductConfig(const std::string& serialno, int workmode);

    WebServer *web_server_;
    TaskHandle_t web_task_handler_;

};

#endif //_WIFI_CONFIGURATION_EX_H
