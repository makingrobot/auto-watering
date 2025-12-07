/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（billy_zh@126.com）
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
#include "l9110_driver.h"
#include "wifi_configuration_ex.h"

#define TAG "XPSTEM_WATERING_SUIT"

void* create_board() { 
    return new XPSTEM_WATERING_SUIT();
}

XPSTEM_WATERING_SUIT::XPSTEM_WATERING_SUIT() : WifiBoard() {

    Log::Info(TAG, "===== Create Board ...... =====");

    led_ = new GpioLed(BUILTIN_LED_PIN);

    InitializeDisplay();

    InitializeButtons();

    InitializePeripherals();

    wifi_conf_ = new WifiConfigurationEx();
    
    Log::Info( TAG, "===== Board config completed. =====");
}

void XPSTEM_WATERING_SUIT::InitializeDisplay() {

    Log::Info( TAG, "Init ssd1306 display ......" );
    U8G2 *u8g2 = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(U8G2_R0, 
        /* i2c clk */ I2C_SCL_PIN,
        /* i2c data */ I2C_SDA_PIN,
        /* reset=*/ U8X8_PIN_NONE
    );
    u8g2->setI2CAddress(OLED_I2C_ADDRESS << 1);

    //u8g2_font_unifont_t_chinese2
    display_ = new U8g2Display(u8g2, DISPLAY_WIDTH, DISPLAY_HEIGHT, u8g2_font_wqy14_t_gb2312);

}

void buttonTickTask(void *pvParam) {
    Log::Info(TAG, "ButtonTickTask running on core %d", xPortGetCoreID());
    OneButton* button = static_cast<OneButton *>(pvParam);
    while (1) {
        button->tick();
        delay(5); //2ms
    }
}

uint32_t __press_start_time = 0;

void longPressStart(void* pvParam) {
    Log::Info(TAG, "Button longpress start.");
    __press_start_time = ((OneButton*)pvParam)->getPressedMs();
}

void longPressStop(void* pvParam) {
    Log::Info(TAG, "Button longpress stop.");
    uint32_t press_stop_time = ((OneButton*)pvParam)->getPressedMs();
    uint32_t duration = press_stop_time - __press_start_time;
    if (duration > 8000) {  // 8s
        Board& board = Board::GetInstance();
        board.OnPhysicalButtonEvent(kManualButton, ButtonAction::LongPress);
    }
}

void XPSTEM_WATERING_SUIT::InitializeButtons() {
    Log::Info( TAG, "Init buttons ......");

    //pinMode(MANUAL_BUTTON_PIN, INPUT);
    manual_button_ = new OneButton(MANUAL_BUTTON_PIN, false);
    manual_button_->attachDoubleClick([]() {
        Log::Info(TAG, "Manual button doubleclick.");
        Board& board = Board::GetInstance();
        board.OnPhysicalButtonEvent(kManualButton, ButtonAction::DoubleClick);
    });
    manual_button_->attachLongPressStart(longPressStart, manual_button_);
    manual_button_->attachLongPressStop(longPressStop, manual_button_);

    xTaskCreate(buttonTickTask, "ButtonTick_Task", 2048, manual_button_, 1, &button_taskhandle_);
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
