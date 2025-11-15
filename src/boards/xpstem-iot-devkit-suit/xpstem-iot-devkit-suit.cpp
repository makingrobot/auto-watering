#include "config.h"
#if BOARD_XPSTEM_IOT_DEVKIT_SUIT == 1

#include "xpstem-iot-devkit-suit.h"

#if CONFIG_USE_U8G2==1
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "src/display/u8g2_display.h"
#endif
#if CONFIG_USE_TFT_ESPI==1
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "src/display/tft_display.h"
#endif

#include "src/sys/system_reset.h"
#include "src/boards/board.h"
#include "src/boards/i2c_device.h"
#include "src/led/gpio_led.h"

#if CONFIG_USE_LVGL==1
#include "src/display/lcd_driver.h"
#include "src/display/drivers/ili9341/ili9341_driver.h"
#endif

#define TAG "BOARD_IOT_DEVKIT_SUIT"

void* create_board() { 
    return new XPSTEM_IOT_DEVKIT_SUIT();
}

void XPSTEM_IOT_DEVKIT_SUIT::InitializePowerSaveTimer() {
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

void XPSTEM_IOT_DEVKIT_SUIT::InitializeDisplay() {

#if CONFIG_USE_U8G2==1
    Log::Info( TAG, "Init ssd1306 display ......" );
    U8G2 *u8g2 = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(U8G2_R0, 
        /* i2c clk */ I2C_SCL_PIN,
        /* i2c data */ I2C_SDA_PIN,
        /* reset=*/ U8X8_PIN_NONE
    );

    //u8g2_font_unifont_t_chinese2
    display_ = new U8g2Display(u8g2, 128, 64, u8g2_font_wqy14_t_gb2312);
#endif

#if CONFIG_USE_TFT_ESPI==1
    Log::Info( TAG, "Init ssd1306 display ......" );
    /**
     * 注意！！！
     * 请在TFT_eSPI库包内的User_Setup.h中配置引脚
     */
    TFT_eSPI *tft = new TFT_eSPI(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    tft->init();
    tft->setRotation(3);
    tft->fillScreen(TFT_BLACK);
    //tft->invertDisplay(DISPLAY_INVERT_COLOR);
    
    //u8g2_font_unifont_t_chinese2
    display_ = new TftDisplay(tft, DISPLAY_WIDTH, DISPLAY_HEIGHT);
#endif

#if CONFIG_USE_LVGL==1
    Log::Info( TAG, "Create ili9341 driver." );
    driver = new ILI9341Driver(DISPLAY_WIDTH, DISPLAY_HEIGHT,
                                    DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y, DISPLAY_SWAP_XY);
                                    
    Log::Info( TAG, "Init st7796 on spi mode." );
    driver->InitSpi(SPI3_HOST, DISPLAY_SPI_MODE, DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN, 
        DISPLAY_MOSI_PIN, DISPLAY_MISO_PIN, DISPLAY_SCK_PIN, DISPLAY_RGB_ORDER, DISPLAY_INVERT_COLOR);

    disp_driver_ = driver;
    display_ = new LvglDisplay(disp_driver_,  {
                                    .text_font = &font_puhui_20_4,
                                    .icon_font = &font_awesome_16_4,
                                    .emoji_font = font_emoji_32_init(),
                                });
#endif

}

void XPSTEM_IOT_DEVKIT_SUIT::InitializeButtons() {
    
}

XPSTEM_IOT_DEVKIT_SUIT::XPSTEM_IOT_DEVKIT_SUIT() : WifiBoard() {

    Log::Info(TAG, "===== Create Board ...... =====");

    led_ = new GpioLed(BUILTIN_LED_PIN, false); // no pwm

    // InitializePowerSaveTimer();

    InitializeButtons();

    InitializeDisplay();

#if CONFIG_USE_LCD_PANEL==1
    Log::Info( TAG, "Init backlight ......" );
    backlight_ = new PwmBacklight(DISPLAY_LED_PIN, DISPLAY_BACKLIGHT_OUTPUT_INVERT);
    backlight_->RestoreBrightness();
#endif

    Log::Info( TAG, "===== Board config completed. =====");
}

XPSTEM_IOT_DEVKIT_SUIT::~XPSTEM_IOT_DEVKIT_SUIT() {
    
}

void XPSTEM_IOT_DEVKIT_SUIT::SetPowerSaveMode(bool enabled) {

    // call base
    WifiBoard::SetPowerSaveMode(enabled);
}

#endif //BOARD_XPSTEM_IOT_DEVKIT_SUIT