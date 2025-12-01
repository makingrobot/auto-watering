#include "config.h"
#if BOARD_XPSTEM_JC4827W543 == 1

#include "xpstem-jc4827w543.h"

#include <SD_MMC.h>

#include "src/framework/sys/system_reset.h"
#include "src/framework/board/board.h"
#include "src/framework/board/i2c_device.h"
#include "src/framework/audio/codecs/no_audio_codec.h"

#if CONFIG_USE_GFX_LIBRARY==1
#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#if CONFIG_USE_LVGL==1
#include "src/framework/display/lvgl_display.h"
#include "src/framework/display/gfx_lvgl_driver.h"
#else
#include "src/framework/display/gfx_display.h"
#endif  //CONFIG_USE_LVGL

#endif //CONFIG_USE_GFX_LIBRARY

#define TAG "XPSTEM_JC4827W543"

void* create_board() { 
    return new XPSTEM_JC4827W543();
}

void XPSTEM_JC4827W543::InitializeI2c() {
    Log::Info( TAG, "Init I2C ......" );
    // Initialize I2C peripheral
    i2c_master_bus_config_t i2c_bus_cfg = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = TOUCH_SDA_PIN,
        .scl_io_num = TOUCH_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .intr_priority = 0,
        .trans_queue_depth = 0,
        .flags = {
            .enable_internal_pullup = 1,
        },
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_cfg, &i2c_bus_));
}

void XPSTEM_JC4827W543::InitializePowerSaveTimer() {
    Log::Info( TAG, "Init power save timer ......" );
    power_save_timer_ = new PowerSaveTimer(-1, 180, 900);
    power_save_timer_->OnEnterSleepMode([this]() {
        EnterSleepMode();
    });
    power_save_timer_->OnExitSleepMode([this]() {
        ExitSleepMode();
    });
    power_save_timer_->OnShutdownRequest([this]() {
        Shutdown();
    });

    power_save_timer_->SetEnabled(true);
}

void XPSTEM_JC4827W543::InitializeDisplay() {

#if CONFIG_USE_GFX_LIBRARY==1
    Log::Info( TAG, "Create GFX driver." );
    gfx_bus_ = new Arduino_ESP32QSPI(
        DISPLAY_CS_PIN /* cs */, 
        DISPLAY_CLK_PIN /* sck */, 
        DISPLAY_D0_PIN /* d0 */, 
        DISPLAY_D1_PIN /* d1 */, 
        DISPLAY_D2_PIN /* d2 */, 
        DISPLAY_D3_PIN /* d3 */);

    gfx_graphics_ = new Arduino_NV3041A(
        gfx_bus_, 
        GFX_NOT_DEFINED /* RST */, 
        DISPLAY_ROTATION /* rotation */, 
        true /* IPS */,
        DISPLAY_WIDTH /* width */,
        DISPLAY_HEIGHT /* height */
    );

#if CONFIG_USE_LVGL==1
    Log::Info( TAG, "Create Lvgl display." );
    GfxLvglDriver* driver = new GfxLvglDriver(gfx_canvas_, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    display_ = new LvglDisplay(driver, {
                                    .text_font = &font_puhui_20_4,
                                    .icon_font = &font_awesome_16_4,
                                    .emoji_font = font_emoji_32_init(),
                                });
#else
    Log::Info( TAG, "Create GFX display." );
    display_ = new GfxDisplay(gfx_graphics_, DISPLAY_WIDTH, DISPLAY_HEIGHT);
#endif // CONFIG_USE_LVGL

#endif // CONFIG_USE_GFX_LIBRARY

}

void XPSTEM_JC4827W543::InitializeButtons() {
    Log::Info( TAG, "Init button ......" );

    boot_button_ = new OneButton(BOOT_BUTTON_PIN);
    boot_button_->attachClick([]() {
        Board& board = Board::GetInstance();
        board.OnPhysicalButtonEvent(kBootButton, ButtonAction::Click);
    });

    boot_button_->attachDoubleClick([]() {
        Board& board = Board::GetInstance();
        board.OnPhysicalButtonEvent(kBootButton, ButtonAction::DoubleClick);
    });

    xTaskCreate([](void *pvParam) {
        OneButton* button = static_cast<OneButton *>(pvParam);
        while (1) {
            button->tick();
            vTaskDelay(pdMS_TO_TICKS(2)); //2ms
        }
    }, "ButtonTick_Task", 2048, boot_button_, 1, NULL);
}

void XPSTEM_JC4827W543::InitializeTouchPad() {
    Log::Info( TAG, "Init XPT2046 touch ......" );
    // ft6336_ = new Ft6336(i2c_bus_, TOUCH_FT6336_ADDR);
    // ft6336_->OnTouched([this](const TouchPoint_t& tp) {
    //     OnDisplayTouchEvent(tp);
    // });
    // ft6336_->Start();
}

void XPSTEM_JC4827W543::InitializeFileSystem() {
    Log::Info( TAG, "Init SD MMC ......" );
    // if (!SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D1, SD_MMC_D2, SD_MMC_D3, SD_MMC_D4)) {
    //     Log::Warn(TAG, "Set SD MMC pin change failed!");
    //     return;
    // }

    // if (!SD_MMC.begin()) {
    //     Log::Warn(TAG, "SD Card mount failed!");
    //     return;
    // }

    // uint8_t cardType = SD_MMC.cardType();
    // if (cardType == CARD_NONE) {
    //     Log::Warn(TAG, "No SD_MMC card attached");
    //     return;
    // }

    // uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    // Log::Info(TAG, "SD_MMC Card Size: %lluMB\n", cardSize);

    // file_system_ = new FileSystem(SD_MMC);
}

XPSTEM_JC4827W543::XPSTEM_JC4827W543() : WifiBoard() {

    Log::Info(TAG, "===== Create Board ...... =====");

    //Log::Info( TAG, "Init led ......" );
    //led_ = new Ws2812Led(BUILTIN_LED_PIN);

    //InitializeI2c();
    //I2cDetect();

    InitializeButtons();

    InitializeDisplay();

    Log::Info( TAG, "Init backlight ......" );
    backlight_ = new PwmBacklight(DISPLAY_BACKLIGHT_PIN, DISPLAY_BACKLIGHT_OUTPUT_INVERT);
    backlight_->RestoreBrightness();

    InitializeTouchPad();

    //InitializeFileSystem();

    InitializePowerSaveTimer();

      Log::Info( TAG, "Init audio codec ......" );
    /* 使用ES8311 驱动 */
    audio_codec_ = new NoAudioCodecSimplex(
        AUDIO_INPUT_SAMPLE_RATE, 
        AUDIO_OUTPUT_SAMPLE_RATE,
        SPECK_BCLK_PIN, 
        SPECK_LRCLK_PIN, 
        SPECK_DIN_PIN);

    Log::Info( TAG, "===== Board config completed. =====");
}

XPSTEM_JC4827W543::~XPSTEM_JC4827W543() {
    
}

#endif //BOARD_XPSTEM_JC4827W543