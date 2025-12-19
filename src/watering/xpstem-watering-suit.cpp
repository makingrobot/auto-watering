/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（vx: billyzh）
 */
#include "xpstem-watering-suit.h"

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <esp_system.h>

#include "src/framework/display/u8g2_display.h"
#include "src/framework/sys/system_reset.h"
#include "src/framework/board/i2c_device.h"
#include "src/framework/led/gpio_led.h"
#include "src/framework/peripheral/sensor.h"
#include "src/framework/peripheral/sensor_value.h"
#include "src/framework/wifi/wifi_station.h"
#include "l9110_driver.h"
#include "wifi_configuration_ex.h"

#define TAG "XPSTEM_WATERING_SUIT"

void* create_board() { 
    return new XPSTEM_WATERING_SUIT();
}

XPSTEM_WATERING_SUIT::XPSTEM_WATERING_SUIT() : WifiBoard() {

    Log::Info(TAG, "===== Create Board ...... =====");

    InitializeDisplay();

    InitializeButtons();

    InitializePeripherals();

    wifi_conf_ = new WifiConfigurationEx();
    
    Log::Info( TAG, "===== Board config completed. =====");
}

void XPSTEM_WATERING_SUIT::InitializeDisplay() {

    Log::Info( TAG, "Init ssd1306 display ......" );
    U8G2 *u8g2 = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(
        /* rotation */ U8G2_R2, 
        /* i2c clk */ I2C_SCL_PIN,
        /* i2c data */ I2C_SDA_PIN,
        /* reset=*/ U8X8_PIN_NONE
    );
    u8g2->setI2CAddress(OLED_I2C_ADDRESS << 1);

    //u8g2_font_unifont_t_chinese2
    display_ = new U8g2Display(u8g2, DISPLAY_WIDTH, DISPLAY_HEIGHT, u8g2_font_wqy14_t_gb2312);

}

void XPSTEM_WATERING_SUIT::ButtonTick() {
    boot_button_->tick();
    manual_button_->tick();
}

long _long_press_start = 0;

void XPSTEM_WATERING_SUIT::InitializeButtons() {
    Log::Info( TAG, "Init buttons ......");

    boot_button_ = new OneButton(BOOT_BUTTON_PIN);
    boot_button_->setLongPressIntervalMs(1000);
    boot_button_->attachLongPressStart([]() {
        Log::Info(TAG, "Boot button longpress start.");
        _long_press_start = millis();
    });
    boot_button_->attachLongPressStop([](void* parameter) {
        Log::Info(TAG, "Boot button longpress stop.");
        long duration = millis() - _long_press_start;
        if (duration > 5000) {
            // 长按5秒以上，则重置WiFi配置
            XPSTEM_WATERING_SUIT *_this = (XPSTEM_WATERING_SUIT*)parameter;
            _this->ResetWifiConfiguration();
        }
    }, this);

    manual_button_ = new OneButton(MANUAL_BUTTON_PIN);
    manual_button_->attachClick([](void* parameter) {
        Log::Info(TAG, "Manual button doubleclick.");
        XPSTEM_WATERING_SUIT *_this = (XPSTEM_WATERING_SUIT*)parameter;
        _this->OnPhysicalButtonEvent(kManualButton, ButtonAction::DoubleClick);
    }, this);

    xTaskCreate([](void *pvParam) {
            Log::Info(TAG, "ButtonTickTask running on core %d", xPortGetCoreID());
            XPSTEM_WATERING_SUIT* _this = (XPSTEM_WATERING_SUIT*)pvParam;
            while (1) {
                _this->ButtonTick();
                delay(2); //2ms
            }
        }, "ButtonTick_Task", 8192, this, 1, &button_taskhandle_);
}

void XPSTEM_WATERING_SUIT::InitializePeripherals() {
    
    Log::Info( TAG, "Init peripherals ......");

    std::shared_ptr<AnalogSensor> soil_moisture_ptr = std::make_shared<AnalogSensor>(SOIL_MOISTURE_PIN);
    soil_moisture_ptr->OnNewData([](const SensorValue& val) {
        auto& app = Application::GetInstance();
        app.OnSensorDataEvent(kSoilMositureName, val);
    });
    AddSensor(kSoilMositureName, soil_moisture_ptr);

    std::shared_ptr<L9110Driver> pump_control_ptr = std::make_shared<L9110Driver>(L9110_PIN_A, L9110_PIN_B, L9110_OUPUT_INVERT);
    AddActuator(kPumpControlName, pump_control_ptr);

}
