/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "xpstem-watering-suit.h"

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

#include "src/display/u8g2_display.h"
#include "src/sys/system_reset.h"
#include "src/boards/board.h"
#include "src/boards/i2c_device.h"
#include "src/led/gpio_led.h"
#include "src/peripheral/analog_sensor.h"
#include "src/peripheral/digital_actuator.h"

#define TAG "XPSTEM_WATERING_SUIT"

void* create_board() { 
    return new XPSTEM_WATERING_SUIT();
}

XPSTEM_WATERING_SUIT::XPSTEM_WATERING_SUIT() : WifiBoard() {

    Log::Info(TAG, "===== Create Board ...... =====");

    led_ = new GpioLed(BUILTIN_LED_PIN);

    InitializeDisplay();

    InitializePeripherals();

    Log::Info( TAG, "===== Board config completed. =====");
}

void XPSTEM_WATERING_SUIT::InitializeDisplay() {

    Log::Info( TAG, "Init ssd1306 display ......" );
    U8G2 *u8g2 = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(U8G2_R0, 
        /* i2c clk */ I2C_SCL_PIN,
        /* i2c data */ I2C_SDA_PIN,
        /* reset=*/ U8X8_PIN_NONE
    );

    //u8g2_font_unifont_t_chinese2
    display_ = new U8g2Display(u8g2, DISPLAY_WIDTH, DISPLAY_HEIGHT, u8g2_font_wqy14_t_gb2312);

}

void XPSTEM_WATERING_SUIT::InitializeButtons() {
    Log::Info( TAG, "Init buttons ......");

    boot_button_ = new Button(kBootButton, BOOT_BUTTON_PIN);
    boot_button_->OnClick([this]() {
        OnPhysicalButtonEvent(kBootButton, ButtonAction::Click);
    });

    boot_button_->OnDoubleClick([this]() {
        OnPhysicalButtonEvent(kBootButton, ButtonAction::DoubleClick);
    });

    manual_button_ = new Button(kManualButton, MANUAL_BUTTON_PIN);
    boot_button_->OnClick([this]() {
        OnPhysicalButtonEvent(kManualButton, ButtonAction::Click);
    });
}

void XPSTEM_WATERING_SUIT::InitializePeripherals() {
    
    Log::Info( TAG, "Init peripherals ......");

    AnalogSensor* soil_moisture = new AnalogSensor(SOIL_MOISTURE_PIN);
    soil_moisture->OnNewData([](int val) {
        auto& app = Application::GetInstance();
        app.OnSensorData(kSoilMositureName, val);
    });
    AddSensor(kSoilMositureName, soil_moisture);

    DigitalActuator* pump_control = new DigitalActuator(PUMP_CONTROL_PIN, PUMP_CONTROL_OUPUT_INVERT);
    AddActuator(kPumpControlName, pump_control);

}
