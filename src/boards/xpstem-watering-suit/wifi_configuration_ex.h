/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _WIFI_CONFIGURATION_EX_H
#define _WIFI_CONFIGURATION_EX_H

#include "../wifi_configuration_impl.h"

class WifiConfigurationEx : public WifiConfigurationImpl {
public:
    WifiConfigurationEx() { }

protected:
    void BindSsidRoute() override;
    void BindAdvancedRoute() override;

private:
    bool ReadProductConfig(const std::string& serialno, int workmode);

};

#endif //_WIFI_CONFIGURATION_EX_H
