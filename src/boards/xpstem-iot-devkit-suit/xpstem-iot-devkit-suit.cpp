#include "config.h"
#if BOARD_XPSTEM_IOT_DEVKIT_SUIT == 1

#include "xpstem-iot-devkit-suit.h"

#if CONFIG_USE_U8G2==1
#include <Arduino.h>
#include <Wire.h>
#include "src/framework/display/u8g2_display.h"
#endif

#if CONFIG_USE_TFT_ESPI==1
#include <Arduino.h>
#include <SPI.h>
#include "src/framework/display/tft_display.h"
#endif

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

#if CONFIG_USE_LCD_PANEL==1
#include "src/framework/display/lcd_driver.h"
#include "src/framework/display/drivers/ili9341/ili9341_driver.h"
#endif

#include "src/framework/sys/system_reset.h"
#include "src/framework/board/board.h"
#include "src/framework/board/i2c_device.h"
#include "src/framework/led/gpio_led.h"
#include "dht11_sensor.h"

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
    u8g2_ = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(U8G2_R0, 
        /* i2c clk */ I2C_SCL_PIN,
        /* i2c data */ I2C_SDA_PIN,
        /* reset=*/ U8X8_PIN_NONE
    );
    u8g2_->setI2CAddress(SSD1306_I2C_ADDR << 1);

    //u8g2_font_unifont_t_chinese2
    display_ = new U8g2Display(u8g2_, 128, 64, u8g2_font_wqy14_t_gb2312);
#endif

#if CONFIG_USE_TFT_ESPI==1
    Log::Info( TAG, "Init ili9341 display ......" );
    /**
     * 注意！！！
     * 请在TFT_eSPI库包内的User_Setup.h中配置引脚
     */
    tft_espi_ = new TFT_eSPI(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    //tft_espi_->invertDisplay(DISPLAY_INVERT_COLOR);
    
    //u8g2_font_unifont_t_chinese2
    display_ = new TftDisplay(tft_espi_, DISPLAY_WIDTH, DISPLAY_HEIGHT);
#endif

#if CONFIG_USE_GFX_LIBRARY==1
    Log::Info( TAG, "Create GFX driver." );
    gfx_bus_ = new Arduino_ESP32SPI(
        DISPLAY_DC_PIN /* DC */, 
        DISPLAY_CS_PIN /* CS*/, 
        DISPLAY_SCK_PIN /* SCK */, 
        DISPLAY_MOSI_PIN /* MOSI */, 
        DISPLAY_MISO_PIN /* MISO */, 
        VSPI /* spi_num */);

    gfx_graphics_ = new Arduino_ILI9341(
        gfx_bus_, 
        DISPLAY_RST_PIN, 
        0 /* rotation */, 
        false /* IPS */);

#if CONFIG_USE_LVGL==1
    Log::Info( TAG, "Create Lvgl display." );
    disp_driver_ = new GfxLvglDriver(gfx_graphics_, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    display_ = new LvglDisplay(disp_driver_, {
                                    .text_font = &font_puhui_20_4,
                                    .icon_font = &font_awesome_16_4,
                                    .emoji_font = font_emoji_32_init(),
                                });
#else
    Log::Info( TAG, "Create GFX display." );
    display_ = new GfxDisplay(gfx_graphics_, DISPLAY_WIDTH, DISPLAY_HEIGHT);
#endif // CONFIG_USE_LVGL

#endif // CONFIG_USE_GFX_LIBRARY

#if CONFIG_USE_LCD_PANEL==1
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

#if CONFIG_USE_LCD_PANEL==1 || CONFIG_USE_GFX_LIBRARY==1 || CONFIG_USE_TFT_ESPI==1
    Log::Info( TAG, "Init backlight ......" );
    backlight_ = new PwmBacklight(DISPLAY_LED_PIN, DISPLAY_BACKLIGHT_OUTPUT_INVERT);
    backlight_->RestoreBrightness();
#endif

    std::shared_ptr<Dht11Sensor> dht11_ptr = std::make_shared<Dht11Sensor>(DHT11_PIN);
    dht11_ptr->OnNewData([](const SensorValue& value){
        auto& app = Application::GetInstance();
        app.OnSensorDataEvent(kDht11, value);
    });
    AddSensor(kDht11, dht11_ptr);

    Log::Info( TAG, "===== Board config completed. =====");
}

XPSTEM_IOT_DEVKIT_SUIT::~XPSTEM_IOT_DEVKIT_SUIT() {
    
}

void XPSTEM_IOT_DEVKIT_SUIT::SetPowerSaveMode(bool enabled) {

    // call base
    WifiBoard::SetPowerSaveMode(enabled);
}

#endif //BOARD_XPSTEM_IOT_DEVKIT_SUIT