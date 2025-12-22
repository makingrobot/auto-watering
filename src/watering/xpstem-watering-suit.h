/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（vx: billyzh）
 */
#ifndef _XPSTEM_WATERING_SUIT_H
#define _XPSTEM_WATERING_SUIT_H

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "src/framework/sys/log.h"
#include "src/framework/app/application.h"
#include "src/framework/board/wifi_board.h"
#include "src/framework/board/button.h"
#include "src/framework/display/display.h"
#include "src/framework/led/led.h"
#include "src/framework/wifi/wifi_configuration.h"

#include "board_config.h"

static const std::string kManualButton      = "manual_button";
static const std::string kPumpControlName   = "pump_control";
static const std::string kSoilMositureName  = "soil_mositure";

class XPSTEM_WATERING_SUIT : public WifiBoard {
private:
    Button *boot_button_ = nullptr;
    Button *manual_button_ = nullptr;
    Display *display_ = nullptr;
    TaskHandle_t button_taskhandle_;
    WifiConfiguration *wifi_conf_;

    void InitializeDisplay();
    void InitializeButtons();
    void InitializePeripherals();

public:
    XPSTEM_WATERING_SUIT();

    Display* GetDisplay() override { return display_; }

    WifiConfiguration* GetWifiConfiguration() override {return wifi_conf_; }

    void ButtonTick();
};

#endif //_XPSTEM_WATERING_SUIT_H
