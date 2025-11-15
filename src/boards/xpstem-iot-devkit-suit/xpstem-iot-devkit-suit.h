#include "config.h"
#if BOARD_XPSTEM_IOT_DEVKIT_SUIT == 1

#ifndef _XPSTEM_IOT_DEVKIT_SUIT_H
#define _XPSTEM_IOT_DEVKIT_SUIT_H

#include <esp_sleep.h>
#include <driver/i2c_master.h>
#include <driver/spi_common.h>
#include <driver/gpio.h>

#include "src/sys/log.h"
#include "src/boards/button.h"
#include "src/app/application.h"
#include "src/boards/wifi_board.h"
#include "src/display/display.h"
#include "src/led/led.h"
#include "src/power/power_save_timer.h"
#include "src/sys/time.h"

#if CONFIG_USE_LCD_PANEL==1
#include "src/display/backlight.h"
#include "src/display/disp_driver.h"
#include "src/display/lvgl_display.h"
#endif

#include "board_config.h"

class XPSTEM_IOT_DEVKIT_SUIT : public WifiBoard {
private:
    i2c_master_bus_handle_t i2c_bus_;
    Button* boot_button_ = nullptr;
    PowerSaveTimer* power_save_timer_ = nullptr;
    Display* display_ = nullptr;
    Led* led_ = nullptr;
#if CONFIG_USE_LCD_PANEL==1
    DispDriver* disp_driver_ = nullptr;
    Backlight* backlight_ = nullptr;
#endif

    void InitializePowerSaveTimer();
    void InitializeDisplay();
    void InitializeButtons();

public:
    XPSTEM_IOT_DEVKIT_SUIT();
    virtual ~XPSTEM_IOT_DEVKIT_SUIT();

    void SetPowerSaveMode(bool enabled) override;

    Display* GetDisplay() override { return display_; }
    Led* GetLed() override { return led_; }

#if CONFIG_USE_LCD_PANEL==1
    DispDriver* GetDispDriver() override { return disp_driver_; }

    Backlight* GetBacklight() override { return backlight_; }
    
    void SetDisplay(Display* display) {
        display_ = display;
        LvglDisplay *disp = static_cast<LvglDisplay*>(display);
        disp->SetupUI();
    }
#endif
};

#endif //_XPSTEM_IOT_DEVKIT_SUIT_H

#endif //BOARD_XPSTEM_IOT_DEVKIT_SUIT