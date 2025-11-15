/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "watering_application.h"
#include <cJSON.h>
#include "src/sys/log.h"
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
    mqtt_service_ = new MqttService();
    mqtt_service_->SubscribeTopic("pump_control_topic_ctrl", [this](const std::string& payload) {
        OnIotMessage("pump_control_topic_ctrl", payload);
    });
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

     if (strcmp(button_name.c_str(), kManualButton)==0) {

        if (action == ButtonAction::Click) {
            DoWatering(5);
            return true;
        }
    }

    return Application::OnPhysicalButtonEvent(button_name, action);

}

bool WateringApplication::OnSensorData(const std::string& sensor_name, int value) {
    
    // 接收到传感器数据、上传IOT
    char buffer[128] = { 0 };
    snprintf(buffer, 127, "{\"data\":{\"temp\":%d}}", value);

    // 连接WiFi

    // 连接MQTT

    // 上传数据
    mqtt_service_->PublishData("soil_moilture_data", std::string(buffer));

    // 等待30秒
    vTaskDelay(pdMS_TO_TICKS(30000));

    // 关闭连接（省电）

    collect_count_++;
    // 已经采集了10次，进入深度睡眠23h
    if (collect_count_ > 10) {
        //Serial.printf("进入深度休眠状态(%d%s)。\n", 
        //    sleepSeconds<3600 ? sleepSeconds/60 : sleepSeconds/3600,
        //    sleepSeconds<3600 ? "分钟" : "小时");
        ESP.deepSleep(23 * 3600 * 1e6);
    }
    return true;
}

void WateringApplication::OnIotMessage(const std::string& topic, const std::string& payload) {

    if (strcmp("pump_control_topic_ctrl", topic.c_str()) == 0) {
        // 浇水动作。
        cJSON *json = cJSON_Parse(payload.c_str());
        cJSON *data = cJSON_GetObjectItem(json, "data");
        if (data) {
            int state = cJSON_GetObjectItem(data, "state")->valueint;
            int keep_seconds = cJSON_GetObjectItem(data, "keepSeconds")->valueint;
            if (state==1) {
                DoWatering(keep_seconds);
            }
        } else {
            Log::Error(TAG, "Payload invalid.");
        }
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
