#include "config.h"
#if BOARD_XINGZHI_MATRIXBIT_V3 == 1

#ifndef _XINGZHI_MATRIXBIT_V3_H
#define _XINGZHI_MATRIXBIT_V3_H

#include <esp_sleep.h>
#include <driver/i2c_master.h>
#include <driver/spi_common.h>
#include <driver/gpio.h>
#include <OneButton.h>

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
#include "src/framework/audio/audio_codec.h"
#endif

#include "board_config.h"

class XINGZHI_MATRIXBIT_V3 : public WifiBoard {
private:
    i2c_master_bus_handle_t i2c_bus_;
    OneButton* boot_button_ = nullptr;
    PowerSaveTimer* power_save_timer_ = nullptr;
    Display* display_ = nullptr;
    AudioCodec* audio_codec_ = nullptr;
#if CONFIG_USE_LCD_PANEL==1
    DispDriver* disp_driver_ = nullptr;
    Backlight* backlight_ = nullptr;
#endif

    void InitializeI2c();
    void InitializePowerSaveTimer();
    void InitializeDisplay();
    void InitializeButtons();
    void InitializeRP2040();

public:
    XINGZHI_MATRIXBIT_V3();
    virtual ~XINGZHI_MATRIXBIT_V3();

    void SetPowerSaveMode(bool enabled) override;

    Display* GetDisplay() override { return display_; }
    AudioCodec* GetAudioCodec() { return audio_codec_; };

#if CONFIG_USE_LCD_PANEL==1
    DispDriver* GetDispDriver() override { return disp_driver_; }

    Backlight* GetBacklight() override { return backlight_; }
    
    void SetDisplay(Display* display) { display_ = display; }
#endif
};

#endif //_XINGZHI_MATRIXBIT_V3_H

#endif //BOARD_XINGZHI_MATRIXBIT_V3