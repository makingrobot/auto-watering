#include "config.h"
#if BOARD_XPSTEM_S3_ELECTRONIC_SUIT == 1

#ifndef _XPSTEM_S3_ELECTRONIC_SUIT_H
#define _XPSTEM_S3_ELECTRONIC_SUIT_H

#include <esp_sleep.h>
#include <driver/i2c_master.h>
#include <driver/spi_common.h>
#include <driver/gpio.h>
#include <OneButton.h>

#include "src/framework/sys/log.h"
#include "src/framework/app/application.h"
#include "src/framework/board/wifi_board.h"
#include "src/framework/display/display.h"
#include "src/framework/display/disp_driver.h"
#include "src/framework/display/backlight.h"
#include "src/framework/led/led.h"
#include "src/framework/power/adc_battery_monitor.h"
#include "src/framework/power/power_save_timer.h"
#include "src/framework/display/drivers/ft6336/ft6336.h"
#include "src/framework/audio/audio_codec.h"
#include "src/framework/file/file_system.h"
#include "src/framework/sys/time.h"

class XPSTEM_S3_ELECTRONIC_SUIT : public WifiBoard {
private:
    i2c_master_bus_handle_t i2c_bus_;
    OneButton* boot_button_ = nullptr;
    PowerSaveTimer* power_save_timer_ = nullptr;
    DispDriver* disp_driver_ = nullptr;
    Display* display_ = nullptr;
    Ft6336* ft6336_ = nullptr;
    Backlight* backlight_ = nullptr;
    AudioCodec* audio_codec_ = nullptr;
    FileSystem* file_system_ = nullptr;
    Time* time_ = nullptr;

    void InitializeI2c();
    void InitializePowerSaveTimer();
    void InitializeDisplay();
    void InitializeButtons();
    void InitializeFt6336TouchPad();
    void InitializeFileSystem();

public:
    XPSTEM_S3_ELECTRONIC_SUIT();
    virtual ~XPSTEM_S3_ELECTRONIC_SUIT();

    bool GetBatteryLevel(int &level, bool &charging, bool &discharging) override;
    void SetPowerSaveMode(bool enabled) override;

    void SetDisplay(Display* display) override { display_ = display; }
    Display* GetDisplay() override { return display_; }

    DispDriver* GetDispDriver() override { return disp_driver_; }
    Backlight* GetBacklight() override { return backlight_; }
    AudioCodec* GetAudioCodec() override { return audio_codec_; }
    FileSystem* GetFileSystem() override { return file_system_; }
    Time* GetTime() override { return time_; }

};

#endif //_XPSTEM_S3_LCD_2_8_H

#endif //BOARD_XPSTEM_S3_ELECTRONIC_SUIT