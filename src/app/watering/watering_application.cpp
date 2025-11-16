/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "watering_application.h"
#include <cJSON.h>
#include "src/sys/log.h"
#include "src/boards/board.h"
#include "src/boards/wifi_board.h"
#include "src/lang/lang_zh_cn.h"
#include "src/display/u8g2_display.h"
#include "src/peripheral/digital_actuator.h"
#include "src/peripheral/analog_sensor.h"
#include "src/wifi/wifi_station.h"
#include "src/sys/settings.h"
#include "src/wifi/wifi_station.h"

#define TAG "WateringApplication"

// 传感器数据采集间隔
static const int kCollectInterval = 180;  //second

// 上传数据后等待多少秒（等待回调）
static const int kWaitSecondsAfterPublished = 30;  //second

// 每个周期采集多少次数据
static const int kTotalCountPerPeriod = 20;

// 深度睡眠时长（23小时）
static const long kDeepSleepSeconds = 23 * 3600 * 1e6;  //second

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
    Settings settings("watering", true);
    iot_broker_ = settings.GetString("iot_broker", "");
    iot_username_ = settings.GetString("iot_username", "");
    iot_password_ = settings.GetString("iot_password", "");
    pump_control_topic_ = settings.GetString("pump_control_topic", "");
    soil_moilture_topic_ = settings.GetString("soil_moilture_topic", "");

    mqtt_service_ = new MqttService();
    mqtt_service_->SubscribeTopic(pump_control_topic_, [this](const std::string& payload) {
        OnIotMessageEvent(pump_control_topic_, payload);
    });
}

void WateringApplication::Start() {
    Application::Start();

    Board& board = Board::GetInstance();
    board.GetLed()->Blink(-1, 1000);
    board.GetDisplay()->SetStatus("工作中");
    //board.GetDisplay()->SetText("你好,世界!");

    Sensor* sensor = board.GetSensor(kSoilMositureName);
    if (sensor != nullptr) {
        AnalogSensor* soil_mositure = static_cast<AnalogSensor*>(sensor);
        soil_mositure->Start(kCollectInterval);  //180s 
    }
}

/**
 * 物理按键事件处理
 */
bool WateringApplication::OnPhysicalButtonEvent(const std::string& button_name, const ButtonAction action) {

    Log::Info(TAG, "响应按钮：%s 的操作：%d", button_name, action);

    if (strcmp(button_name.c_str(), kManualButton)==0) {

        if (action == ButtonAction::DoubleClick) {
            DoWatering(5);
            return true;
        } else if (action == ButtonAction::LongPress) {
            // todo:
        }

    }

    return Application::OnPhysicalButtonEvent(button_name, action);

}

/**
 * 传感器数据事件处理
 */
bool WateringApplication::OnSensorDataEvent(const std::string& sensor_name, int value) {
    
    Log::Info(TAG, "接收到传感器：%s 的数据：%d", sensor_name.c_str(), value);

    // 接收到传感器数据、上传IOT
    char buffer[128] = { 0 };
    snprintf(buffer, 127, "{\"data\":{\"temp\":%d}}", value);

    // 连接WiFi
    WifiBoard* wifi_board = static_cast<WifiBoard*>(&Board::GetInstance());
    wifi_board->StartNetwork();

    // 连接MQTT
    if (!mqtt_service_->Connect(iot_broker_, iot_username_, iot_password_)) {
        // 连接失败
        //ESP.deepSleep(5 * 60); //5分钟
        return false;
    }

    // 上传数据
    mqtt_service_->PublishData(soil_moilture_topic_, std::string(buffer));

    // 等待30秒
    vTaskDelay(pdMS_TO_TICKS(kWaitSecondsAfterPublished * 1000));

    // 关闭连接（省电）
    mqtt_service_->Disconnect();
    WifiStation::GetInstance().Stop();

    collect_count_++;
    // 已经采集了10次，进入深度睡眠23h
    if (collect_count_ > kTotalCountPerPeriod) {
        //Serial.printf("进入深度休眠状态(%d%s)。\n", 
        //    sleepSeconds<3600 ? sleepSeconds/60 : sleepSeconds/3600,
        //    sleepSeconds<3600 ? "分钟" : "小时");
        ESP.deepSleep(kDeepSleepSeconds);
    }

    return true;
}

/**
 * 物联网消息事件处理
 */
void WateringApplication::OnIotMessageEvent(const std::string& topic, const std::string& payload) {

    Log::Info(TAG, "处理IoT消息，主题：%s", topic.c_str());

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
        Log::Info(TAG, "浇水 %d 秒", seconds);
        DigitalActuator* pump_control = static_cast<DigitalActuator*>(actuator);
        pump_control->On();
        vTaskDelay(pdMS_TO_TICKS(seconds * 1000));
        pump_control->Off();
    }
}


void WateringApplication::ShowSensorValue(int value) {
    
}

void WateringApplication::ShowStatus(const AppStatus status) {

}