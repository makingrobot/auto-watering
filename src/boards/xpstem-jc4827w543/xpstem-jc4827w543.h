#include "config.h"
#if BOARD_XPSTEM_JC4827W543 == 1

#ifndef _XPSTEM_JC4827W543_H
#define _XPSTEM_JC4827W543_H

#include <esp_sleep.h>
#include <driver/i2c_master.h>
#include <driver/spi_common.h>
#include <driver/gpio.h>
#include <OneButton.h>

#include "src/sys/log.h"
#include "src/boards/wifi_board.h"
#include "src/display/display.h"
#include "src/display/disp_driver.h"
#include "src/display/backlight.h"
#include "src/power/adc_battery_monitor.h"
#include "src/power/power_save_timer.h"

#include "board_config.h"

class XPSTEM_JC4827W543 : public WifiBoard {
private:
    i2c_master_bus_handle_t i2c_bus_;
    OneButton* boot_button_ = nullptr;
    PowerSaveTimer* power_save_timer_ = nullptr;
    DispDriver* disp_driver_ = nullptr;
    Display* display_ = nullptr;
    Backlight* backlight_ = nullptr;

    void InitializeI2c();
    void I2cDetect();
    void InitializePowerSaveTimer();
    void InitializeDisplay();
    void InitializeButtons();
    void InitializeTouchPad();
    void InitializeFileSystem();

public:
    XPSTEM_JC4827W543();
    virtual ~XPSTEM_JC4827W543();

#if CONFIG_USE_LVGL==1
    void SetDisplay(Display* display) override { display_ = display; }
#endif
    Display* GetDisplay() override { return display_; }

    //Led* GetLed() override { return led_; }
    DispDriver* GetDispDriver() override { return disp_driver_; }
    Backlight* GetBacklight() override { return backlight_; }

};

#endif //_XPSTEM_JC4827W543_H

#endif //BOARD_XPSTEM_JC4827W543