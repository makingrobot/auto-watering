#include "config.h"
#if BOARD_XPSTEM_JC4827W543 == 1

#ifndef _XPSTEM_JC4827W543_H
#define _XPSTEM_JC4827W543_H

#include <esp_sleep.h>
#include <driver/spi_common.h>
#include <driver/gpio.h>

#include <Arduino.h>
#include <OneButton.h>

#if CONFIG_USE_GFX_LIBRARY==1
#include <Arduino_GFX_Library.h>
#endif

#include "src/framework/sys/log.h"
#include "src/framework/board/wifi_board.h"
#include "src/framework/display/display.h"
#include "src/framework/display/disp_driver.h"
#include "src/framework/display/backlight.h"
#include "src/framework/audio/audio_codec.h"
#include "src/framework/power/adc_battery_monitor.h"
#include "src/framework/power/power_save_timer.h"

#include "board_config.h"

class XPSTEM_JC4827W543 : public WifiBoard {
private:
    i2c_master_bus_handle_t i2c_bus_;
    OneButton* boot_button_ = nullptr;
    PowerSaveTimer* power_save_timer_ = nullptr;
    DispDriver* disp_driver_ = nullptr;
    Display* display_ = nullptr;
    Backlight* backlight_ = nullptr;
    AudioCodec* audio_codec_ = nullptr;

#if CONFIG_USE_GFX_LIBRARY==1
    Arduino_DataBus* gfx_bus_ = nullptr;
    Arduino_GFX* gfx_graphics_ = nullptr;
#endif

    void InitializeI2c();
    void InitializePowerSaveTimer();
    void InitializeDisplay();
    void InitializeButtons();
    void InitializeTouchPad();
    void InitializeFileSystem();

public:
    XPSTEM_JC4827W543();

#if CONFIG_USE_LVGL==1
    void SetDisplay(Display* display) override { display_ = display; }
#endif
    Display* GetDisplay() override { return display_; }

    //Led* GetLed() override { return led_; }
    DispDriver* GetDispDriver() override { return disp_driver_; }
    Backlight* GetBacklight() override { return backlight_; }
    AudioCodec* GetAudioCodec() override { return audio_codec_; }

};

#endif //_XPSTEM_JC4827W543_H

#endif //BOARD_XPSTEM_JC4827W543