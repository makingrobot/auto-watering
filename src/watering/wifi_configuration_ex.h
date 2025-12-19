/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（vx: billyzh）
 */
#ifndef _WIFI_CONFIGURATION_EX_H
#define _WIFI_CONFIGURATION_EX_H

#include <Arduino.h>
#include <WebServer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "src/framework/wifi/wifi_configuration.h"

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
