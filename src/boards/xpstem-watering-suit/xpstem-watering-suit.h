/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _XPSTEM_WATERING_SUIT_H
#define _XPSTEM_WATERING_SUIT_H

#include <driver/gpio.h>

#include "src/sys/log.h"
#include "src/boards/button.h"
#include "src/app/application.h"
#include "src/boards/wifi_board.h"
#include "src/display/display.h"
#include "src/led/led.h"

#include "board_config.h"

class XPSTEM_WATERING_SUIT : public WifiBoard {
private:
    Button* boot_button_ = nullptr;
    Button* manual_button_ = nullptr;
    Display* display_ = nullptr;
    Led* led_ = nullptr;

    void InitializeDisplay();
    void InitializeButtons();
    void InitializePeripherals();

public:
    XPSTEM_WATERING_SUIT();

    Display* GetDisplay() override { return display_; }

    Led* GetLed() override { return led_; }

};

#endif //_XPSTEM_WATERING_SUIT_H
