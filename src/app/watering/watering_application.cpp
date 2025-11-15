/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "watering_application.h"
#include "esp_log.h"
#include "src/boards/board.h"
#include "src/lang/lang_zh_cn.h"
#include "src/display/u8g2_display.h"
#include "src/peripheral/digital_actuator.h"
#include "src/peripheral/analog_sensor.h"

#define TAG "WateringApplication"

void* create_application() {
    return new WateringApplication();
}

WateringApplication::WateringApplication() : Application() { 

}

WateringApplication::~WateringApplication() {
   
}

void WateringApplication::Init() {
    Application::Init();
    
    // do your init.
}

void WateringApplication::Start() {
    //Application::Start();

    Board& board = Board::GetInstance();
    board.GetLed()->Blink(-1, 1000);
    board.GetDisplay()->SetStatus("工作中");
    //board.GetDisplay()->SetText("你好,世界!");

    Sensor* sensor = board.GetSensor(kSoilMositureName);
    if (sensor != nullptr) {
        AnalogSensor* soil_mositure = static_cast<AnalogSensor*>(sensor);
        soil_mositure->Start(180);  //180s 
    } 
}

bool WateringApplication::OnPhysicalButtonEvent(const std::string& button_name, const ButtonAction action) {

}

bool WateringApplication::OnSensorData(const std::string& sensor_name, int value) {
    
    // 上传IOT
    if (value > 1000) {
        DoWatering(5);
    }
}

void WateringApplication::DoWatering(uint8_t seconds) {

    Board& board = Board::GetInstance();
    Actuator* actuator = board.GetActuator(kPumpControlName);
    if (actuator != nullptr) {
        // 打开
        DigitalActuator* pump_control = static_cast<DigitalActuator*>(actuator);
        pump_control->On();
        vTaskDelay(pdMS_TO_TICKS(seconds * 1000));
        pump_control->Off();
    }
}
