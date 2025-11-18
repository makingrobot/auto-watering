#include "config.h"
#if BOARD_XPSTEM_S3_ELECTRONIC_SUIT == 1

#include "xpstem-s3-electronic-suit.h"

#include "src/sys/system_reset.h"
#include "src/boards/board.h"
#include "src/boards/i2c_device.h"
#include "src/display/lvgl_display.h"

#if CONFIG_USE_LCD_PANEL==1
#include "src/display/drivers/st7796/st7796_driver.h"
#include "src/display/lcd_driver.h"
#endif
#if CONFIG_USE_GFX_LIBRARY==1
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "src/display/gfx_driver.h"
#endif

#include "src/audio/codecs/no_audio_codec.h"

#include "src/sys/time/ntp_time.h"
#include "SD.h"
#include "SPI.h"

#include "board_config.h"

#define TAG "XPSTEM_S3_ELECTRONIC_SUIT"

void* create_board() { 
    return new XPSTEM_S3_ELECTRONIC_SUIT();
}

void XPSTEM_S3_ELECTRONIC_SUIT::InitializeI2c() {
    Log::Info( TAG, "Init I2C ......" );
    // Initialize I2C peripheral
    i2c_master_bus_config_t i2c_bus_cfg = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
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

void XPSTEM_S3_ELECTRONIC_SUIT::InitializePowerSaveTimer() {
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

void XPSTEM_S3_ELECTRONIC_SUIT::InitializeDisplay() {
    Log::Info( TAG, "Init lcd display ......" );

#if CONFIG_USE_LCD_PANEL==1
    Log::Info( TAG, "Create st7796 driver." );
    LcdDriver* driver = new ST7796Driver(DISPLAY_WIDTH, DISPLAY_HEIGHT,
                                    DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y, DISPLAY_SWAP_XY, 
                                    DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y);
                                    
    Log::Info( TAG, "Init st7796 on spi mode." );
    driver->InitSpi(SPI3_HOST, DISPLAY_SPI_MODE, DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN, 
        DISPLAY_MOSI_PIN, DISPLAY_MISO_PIN, DISPLAY_SCK_PIN, DISPLAY_RGB_ORDER, DISPLAY_INVERT_COLOR);

    disp_driver_ = driver;
#endif

#if CONFIG_USE_GFX_LIBRARY==1
    Arduino_DataBus *bus = new Arduino_ESP32SPI(DISPLAY_DC_PIN, DISPLAY_CS_PIN, DISPLAY_SCK_PIN,  
        DISPLAY_MOSI_PIN, DISPLAY_MISO_PIN, SPI3_HOST);

    Arduino_GFX *gfx = new Arduino_ST7796(bus, DISPLAY_RST_PIN, 0 /* rotation */, false /* IPS */);

    GfxDriver *driver = new GfxDriver(gfx, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    disp_driver_ = driver;
#endif

    display_ = new LvglDisplay(disp_driver_,  {
                                    .text_font = &font_puhui_20_4,
                                    .icon_font = &font_awesome_16_4,
                                    .emoji_font = font_emoji_32_init(),
                                });
}

void XPSTEM_S3_ELECTRONIC_SUIT::I2cDetect() {
    uint8_t address;
    printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");
    for (int i = 0; i < 128; i += 16) {
        printf("%02x: ", i);
        for (int j = 0; j < 16; j++) {
            fflush(stdout);
            address = i + j;
            esp_err_t ret = i2c_master_probe(i2c_bus_, address, pdMS_TO_TICKS(200));
            if (ret == ESP_OK) {
                printf("%02x ", address);
            } else if (ret == ESP_ERR_TIMEOUT) {
                printf("UU ");
            } else {
                printf("-- ");
            }
        }
        printf("\r\n");
    }
}

void buttonTickTask(void *pvParam) {
    OneButton* button = static_cast<OneButton *>(pvParam);
    while (1) {
        button->tick();
        vTaskDelay(pdMS_TO_TICKS(2)); //2ms
    }
}

void XPSTEM_S3_ELECTRONIC_SUIT::InitializeButtons() {
    Log::Info( TAG, "Init button ......" );

    boot_button_ = new Button(BUTTON_0_PIN);
    boot_button_->attachClick([]() {
        Board& board = Board::GetInstance();
        board.OnPhysicalButtonEvent(kBootButton, ButtonAction::Click);
    });

    boot_button_->attachDoubleClick([]() {
        Board& board = Board::GetInstance();
        board.OnPhysicalButtonEvent(kBootButton, ButtonAction::DoubleClick);
    });

    xTaskCreate(buttonTickTask, "ButtonTick_Task", 2048, boot_button_, 1, NULL);
}

void XPSTEM_S3_ELECTRONIC_SUIT::InitializeFt6336TouchPad() {
    Log::Info( TAG, "Init FT6336 touch ......" );
    ft6336_ = new Ft6336(i2c_bus_, TOUCH_FT6336_ADDR);
    ft6336_->OnTouched([this](const TouchPoint_t& tp) {
        OnDisplayTouchEvent(tp);
    });
    ft6336_->Start();
}

void XPSTEM_S3_ELECTRONIC_SUIT::InitializeFileSystem() {
    // Log::Info( TAG, "Init SD MMC ......" );

    // SPIClass *spi = new SPIClass(3); //SPI3_HOST
    // if (!SD.begin(2, *spi)) {
    //     Log::Warn(TAG, "SD Card mount failed!");
    //     return;
    // }

    // uint8_t cardType = SD.cardType();
    // if (cardType == CARD_NONE) {
    //     Log::Warn(TAG, "No SD_MMC card attached");
    //     return;
    // }

    // uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    // Log::Info(TAG, "SD_MMC Card Size: %lluMB\n", cardSize);

    // file_system_ = new FileSystem(SD);
}

XPSTEM_S3_ELECTRONIC_SUIT::XPSTEM_S3_ELECTRONIC_SUIT() : WifiBoard() {

    Log::Info(TAG, "===== Create Board ...... =====");

    InitializeI2c();
    //I2cDetect();

    InitializePowerSaveTimer();

    InitializeButtons();

    InitializeDisplay();

    //InitializeFt6336TouchPad();

    InitializeFileSystem();

    //time_ = new NTPTime();

    Log::Info( TAG, "Init backlight ......" );
    backlight_ = new PwmBacklight(DISPLAY_LED_PIN, DISPLAY_BACKLIGHT_OUTPUT_INVERT);
    backlight_->RestoreBrightness();

    Log::Info( TAG, "Init audio codec ......" );
    /* 使用ES8311 驱动 */
    audio_codec_ = new NoAudioCodecSimplex(
        AUDIO_INPUT_SAMPLE_RATE, 
        AUDIO_OUTPUT_SAMPLE_RATE,
        AUDIO_SPK_BCLK_PIN, 
        AUDIO_SPK_LRC_PIN, 
        AUDIO_SPK_DAT_PIN, 
        AUDIO_MIC_SCK_PIN, 
        AUDIO_MIC_WS_PIN, 
        AUDIO_MIC_DAT_PIN);

    Log::Info( TAG, "===== Board config completed. =====");
}

XPSTEM_S3_ELECTRONIC_SUIT::~XPSTEM_S3_ELECTRONIC_SUIT() {
    
}

bool XPSTEM_S3_ELECTRONIC_SUIT::GetBatteryLevel(int &level, bool &charging, bool &discharging) {
    // charging = battery_monitor_->IsCharging();
    // discharging = !charging;
    // level = battery_monitor_->GetBatteryLevel();
    return false;
}

void XPSTEM_S3_ELECTRONIC_SUIT::SetPowerSaveMode(bool enabled) {
    if (!enabled) {
        power_save_timer_->WakeUp();
    }
}

#endif //BOARD_XPSTEM_S3_ELECTRONIC_SUIT