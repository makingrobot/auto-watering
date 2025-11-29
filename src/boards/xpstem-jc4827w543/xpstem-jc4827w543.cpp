#include "config.h"
#if BOARD_XPSTEM_JC4827W543 == 1

#include "xpstem-jc4827w543.h"

#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#include "src/framework/sys/system_reset.h"
#include "src/framework/board/board.h"
#include "src/framework/board/i2c_device.h"
#include "src/framework/display/lvgl_display.h"
#include "src/framework/display/gfx_display.h"
#include <SD_MMC.h>

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
    Log::Info( TAG, "Init lcd display ......" );

    // Log::Info( TAG, "Create GFX driver." );
    Arduino_DataBus *bus = new Arduino_ESP32QSPI(
        DISPLAY_CS_PIN /* cs */, 
        DISPLAY_CLK_PIN /* sck */, 
        DISPLAY_D0_PIN /* d0 */, 
        DISPLAY_D1_PIN /* d1 */, 
        DISPLAY_D2_PIN /* d2 */, 
        DISPLAY_D3_PIN /* d3 */);

    Arduino_GFX *g = new Arduino_NV3041A(bus, 
        GFX_NOT_DEFINED /* RST */, 
        DISPLAY_ROTATION /* rotation */, 
        true /* IPS */);

    Arduino_GFX *gfx = new Arduino_Canvas(
        DISPLAY_WIDTH /* width */, 
        DISPLAY_HEIGHT /* height */, 
        g);

    display_ = new GfxDisplay(gfx, DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

void XPSTEM_JC4827W543::I2cDetect() {
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

    xTaskCreate(buttonTickTask, "ButtonTick_Task", 2048, boot_button_, 1, NULL);
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

    InitializeI2c();
    //I2cDetect();

    InitializeButtons();

    InitializeDisplay();

    Log::Info( TAG, "Init backlight ......" );
    backlight_ = new PwmBacklight(DISPLAY_BACKLIGHT_PIN, DISPLAY_BACKLIGHT_OUTPUT_INVERT);
    backlight_->RestoreBrightness();

    InitializeTouchPad();

    //InitializeFileSystem();

    InitializePowerSaveTimer();

    Log::Info( TAG, "===== Board config completed. =====");
}

XPSTEM_JC4827W543::~XPSTEM_JC4827W543() {
    
}

#endif //BOARD_XPSTEM_JC4827W543