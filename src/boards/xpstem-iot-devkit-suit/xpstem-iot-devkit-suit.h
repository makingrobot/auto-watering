#include "config.h"
#if BOARD_XPSTEM_IOT_DEVKIT_SUIT == 1

#ifndef _XPSTEM_IOT_DEVKIT_SUIT_H
#define _XPSTEM_IOT_DEVKIT_SUIT_H

#include <esp_sleep.h>
#include <driver/i2c_master.h>
#include <driver/spi_common.h>
#include <driver/gpio.h>
#include <OneButton.h>

#include "src/framework/sys/log.h"
#include "src/framework/app/application.h"
#include "src/framework/board/wifi_board.h"
#include "src/framework/display/display.h"
#include "src/framework/led/led.h"
#include "src/framework/power/power_save_timer.h"
#include "src/framework/sys/time.h"

#if CONFIG_USE_LCD_PANEL==1
#include "src/framework/display/backlight.h"
#include "src/framework/display/disp_driver.h"
#include "src/framework/display/lvgl_display.h"
#endif

#if CONFIG_USE_U8G2==1
#include <Arduino.h>
#include <U8g2lib.h>
#endif

#if CONFIG_USE_TFT_ESPI==1
#include <Arduino.h>
#include <TFT_eSPI.h>
#endif

#include "board_config.h"

class XPSTEM_IOT_DEVKIT_SUIT : public WifiBoard {
private:
    i2c_master_bus_handle_t i2c_bus_;
    OneButton* boot_button_ = nullptr;
    PowerSaveTimer* power_save_timer_ = nullptr;
    Display* display_ = nullptr;
    Led* led_ = nullptr;

#if CONFIG_USE_LCD_PANEL==1
    DispDriver* disp_driver_ = nullptr;
    Backlight* backlight_ = nullptr;
#endif

#if CONFIG_USE_U8G2==1
    U8G2 *u8g2_ = nullptr;
#endif

#if CONFIG_USE_TFT_ESPI==1
    TFT_eSPI *tft_espi_ = nullptr;
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
    
    void SetDisplay(Display* display) { display_ = display; }
#endif
};

#endif //_XPSTEM_IOT_DEVKIT_SUIT_H

#endif //BOARD_XPSTEM_IOT_DEVKIT_SUIT