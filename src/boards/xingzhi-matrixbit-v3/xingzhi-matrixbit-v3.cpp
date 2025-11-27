#include "config.h"
#if BOARD_XINGZHI_MATRIXBIT_V3 == 1

#include "xingzhi-matrixbit-v3.h"

#if CONFIG_USE_TFT_ESPI==1
#include <Arduino.h>
#include <TFT_eSPI.h>
#endif

#include "src/sys/system_reset.h"
#include "src/boards/board.h"
#include "src/boards/i2c_device.h"
#include "src/audio/audio_codec.h"
#include "src/audio/codecs/es8311/es8311_audio_codec.h"
#if CONFIG_USE_TFT_ESPI==1
#include "src/display/tft_display.h"
#endif
#if CONFIG_USE_LVGL==1
#include "src/display/lcd_driver.h"
#include "src/display/drivers/st7789/st7789_driver.h"
#endif
#define TAG "XINGZHI_MATRIXBIT_V3"

void* create_board() { 
    return new XINGZHI_MATRIXBIT_V3();
}

void XINGZHI_MATRIXBIT_V3::InitializeI2c() {
    // Initialize I2C peripheral
    i2c_master_bus_config_t i2c_bus_cfg = {
        .i2c_port = (i2c_port_t)1,
        // .i2c_port = I2C_NUM_0,
        .sda_io_num = AUDIO_CODEC_I2C_SDA_PIN,
        .scl_io_num = AUDIO_CODEC_I2C_SCL_PIN,
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

void XINGZHI_MATRIXBIT_V3::InitializePowerSaveTimer() {
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

void XINGZHI_MATRIXBIT_V3::InitializeDisplay() {

#if CONFIG_USE_TFT_ESPI==1
    Log::Info( TAG, "Init ssd1306 display ......" );
    TFT_eSPI *tft = new TFT_eSPI();
    tft->init();
    tft->setRotation(1);
    //tft->invertDisplay(0);

    //u8g2_font_unifont_t_chinese2
    display_ = new TftDisplay(tft, 240, 240);
#endif

#if CONFIG_USE_LCD_PANEL==1
    Log::Info( TAG, "Create st7789 driver." );
    ST7789Driver *driver = new ST7789Driver(DISPLAY_WIDTH, DISPLAY_HEIGHT,
                                    DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y, DISPLAY_SWAP_XY);
                                    
    Log::Info( TAG, "Init st7796 on spi mode." );
    driver->InitSpi(SPI3_HOST, 0, DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN, 
        DISPLAY_MOSI_PIN, GPIO_NUM_NC, DISPLAY_SCK_PIN, DISPLAY_RGB_ORDER, DISPLAY_INVERT_COLOR);

    disp_driver_ = driver;
#endif

}

void XINGZHI_MATRIXBIT_V3::InitializeButtons() {
    
}

void XINGZHI_MATRIXBIT_V3::InitializeRP2040() {
    // RP2040负责外围设备（信号采集和出）
    // ESP32负责计算和逻辑处理，与RP2040通过I2C通讯，RP2040为I2C主设置
    // 通讯数据包采用JSON格式，格式为数据长度（2个字节）+内容{"name":"xxx", "data":"value"或{}}

}

XINGZHI_MATRIXBIT_V3::XINGZHI_MATRIXBIT_V3() : WifiBoard() {

    Log::Info(TAG, "===== Create Board ...... =====");

    InitializeI2c();

    // InitializePowerSaveTimer();

    InitializeButtons();

    InitializeDisplay();

#if CONFIG_USE_LCD_PANEL==1
    Log::Info( TAG, "Init backlight ......" );
    backlight_ = new PwmBacklight(DISPLAY_BACKLIGHT_PIN, DISPLAY_BACKLIGHT_OUTPUT_INVERT);
    backlight_->RestoreBrightness();
#endif

    audio_codec_ = new Es8311AudioCodec(
        i2c_bus_, 
        I2C_NUM_0,
        AUDIO_INPUT_SAMPLE_RATE, 
        AUDIO_OUTPUT_SAMPLE_RATE,
        AUDIO_I2S_MCLK_PIN, 
        AUDIO_I2S_BCLK_PIN, 
        AUDIO_I2S_WS_PIN, 
        AUDIO_I2S_DOUT_PIN, 
        AUDIO_I2S_DIN_PIN,
        AUDIO_CODEC_PA_PIN, 
        AUDIO_CODEC_ES8311_ADDR, 
        AUDIO_INPUT_REFERENCE);

    Log::Info( TAG, "===== Board config completed. =====");
}

XINGZHI_MATRIXBIT_V3::~XINGZHI_MATRIXBIT_V3() {
    
}

void XINGZHI_MATRIXBIT_V3::SetPowerSaveMode(bool enabled) {
    if (!enabled) {
        power_save_timer_->WakeUp();
    }
    WifiBoard::SetPowerSaveMode(enabled);
}

// void XINGZHI_MATRIXBIT_V3::InitializeCamera() {
//     camera_config_t config = {};
//     config.ledc_channel = LEDC_CHANNEL_2;  // LEDC通道选择  用于生成XCLK时钟 但是S3不用
//     config.ledc_timer = LEDC_TIMER_2; // LEDC timer选择  用于生成XCLK时钟 但是S3不用
//     config.pin_d0 = CAMERA_PIN_D0;
//     config.pin_d1 = CAMERA_PIN_D1;
//     config.pin_d2 = CAMERA_PIN_D2;
//     config.pin_d3 = CAMERA_PIN_D3;
//     config.pin_d4 = CAMERA_PIN_D4;
//     config.pin_d5 = CAMERA_PIN_D5;
//     config.pin_d6 = CAMERA_PIN_D6;
//     config.pin_d7 = CAMERA_PIN_D7;
//     config.pin_xclk = CAMERA_PIN_XCLK;
//     config.pin_pclk = CAMERA_PIN_PCLK;
//     config.pin_vsync = CAMERA_PIN_VSYNC;
//     config.pin_href = CAMERA_PIN_HREF;
//     config.pin_sccb_sda = -1;   // 这里写-1 表示使用已经初始化的I2C接口
//     config.pin_sccb_scl = CAMERA_PIN_SIOC;
//     config.sccb_i2c_port = 1;
//     config.pin_pwdn = CAMERA_PIN_PWDN;
//     config.pin_reset = CAMERA_PIN_RESET;
//     config.xclk_freq_hz = XCLK_FREQ_HZ;
//     config.pixel_format = PIXFORMAT_RGB565;
//     config.frame_size = FRAMESIZE_VGA;
//     config.jpeg_quality = 12;
//     config.fb_count = 1;
//     config.fb_location = CAMERA_FB_IN_PSRAM;
//     config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

//     camera_ = new Esp32Camera(config);
// }

#endif //BOARD_XINGZHI_MATRIXBIT_V3