/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _WIFI_BOARD_H
#define _WIFI_BOARD_H

#include "board.h"

class WifiBoard : public Board {
protected:
    bool wifi_config_mode_ = false;
#if CONFIG_WIFI_CONFIGURE_ENABLE==1
    void EnterWifiConfigMode();
#endif

public:
    WifiBoard();

    virtual std::string GetBoardType() override;
    virtual std::string GetBoardJson() override;
    virtual std::string GetDeviceStatusJson() override;
    virtual void SetPowerSaveMode(bool enabled) override;
    virtual const char* GetNetworkStateIconName() override;

    virtual void StartNetwork();
    virtual bool StartNetwork(const std::string& ssid, const std::string& password);

#if CONFIG_WIFI_CONFIGURE_ENABLE==1
    virtual void Configure();
    virtual void ResetWifiConfiguration();
#endif
};

#endif // _WIFI_BOARD_H
