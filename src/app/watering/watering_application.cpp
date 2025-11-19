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
#include "src/wifi/wifi_station.h"
#include "src/sys/settings.h"
#include "src/wifi/wifi_station.h"
#include "src/peripheral/analog_sensor.h"
#include "src/peripheral/switch_actuator.h"
#include "src/boards/xpstem-watering-suit/l9110_driver.h"

#define TAG "WateringApplication"

// 传感器数据采集间隔
static const int kCollectInterval = 120;  //second

// 上传数据后等待多少秒（等待回调）
static const int kWaitSecondsAfterPublished = 30;  //second

// 每个周期采集多少次数据
static const int kTotalCountPerPeriod = 20;

// 深度睡眠时长（23小时）
static const long kDeepSleepSeconds = 23 * 3600;  //second

static const std::string kSsid = "qwer_1234";
static const std::string kPassword = "abc123";

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
}

void WateringApplication::Start() {

    Board& board = Board::GetInstance();
    board.GetLed()->Blink(-1, 1000);

    // 启动传感器
    Sensor* sensor = board.GetSensor(kSoilMositureName);
    if (sensor != nullptr) {
        AnalogSensor* soil_mositure = static_cast<AnalogSensor*>(sensor);
        soil_mositure->Start(kCollectInterval);  //120s 
    }

    Application::Start();

    // 主题订阅
    mqtt_service_->SubscribeTopic(pump_control_topic_, [this](const std::string& payload) {
        OnIotMessageEvent(pump_control_topic_, payload);
    });

    started_ = true;
}

void WateringApplication::ShowWifiConfigHit(const std::string& ssid, const std::string& config_url, const std::string& mac_address) {
    // 显示 WiFi 配置 AP 的 SSID 和 Web 服务器 URL
    Board& board = Board::GetInstance();
    U8g2Display* display = static_cast<U8g2Display*>(board.GetDisplay());
    display->GetWindow()->SetText(1, "热点" + ssid);
    display->GetWindow()->SetText(2, "访问" + config_url);
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

    Log::Info(TAG, "接收到传感器:%s 的数据:%d", sensor_name.c_str(), value);

    if (value>0) {
        // 映射到 0-100
        soil_moilture_value_ = map(value, 1, 4095, 100, 1);
    }

    WifiBoard* wifi_board = static_cast<WifiBoard*>(&Board::GetInstance());
    U8g2Display* display = static_cast<U8g2Display*>(wifi_board->GetDisplay());

    if (!started_) {
        display->GetWindow()->SetText(3, "湿度: " + std::to_string(soil_moilture_value_) );
        return false;
    }

    display->GetWindow()->SetText(1, "湿度: " + std::to_string(soil_moilture_value_) );

    SetDeviceState(kDeviceStateConnecting);

#if CONFIG_WIFI_CONFIGURE_ENABLE==1
    wifi_board->StartNetwork();
#else
    // 连接WiFi
    if (!wifi_board->StartNetwork(kSsid, kPassword)) {
        Log::Info(TAG, "Wifi connect failure.");
        ShowMessage("WiFi连接失败。");
        SetDeviceState(kDeviceStateWarning);
        return false;
    }
#endif

    int retry_count = 5;
    // 连接MQTT
    while (!mqtt_service_->Connect(iot_broker_, iot_username_, iot_password_)) {
        // 连接失败
        SetDeviceState(kDeviceStateWarning);
        ShowMessage("MQTT连接失败。");

        Log::Info(TAG, "5秒后重试。");
        vTaskDelay(pdMS_TO_TICKS(5000));

        retry_count--;
        if (retry_count == 0) {
            Log::Info(TAG, "未连接到小鹏AIoT平台，进入睡眠状态。");
            wifi_board->GetDisplay()->Sleep();
            wifi_board->Sleep(300 * 1000); //300s
            return false;
        }
    }

    SetDeviceState(kDeviceStateWorking);

    // 上传IOT
    char buffer[128] = { 0 };
    snprintf(buffer, 127, "{\"data\":{\"temp\":%d}}", value);
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
        wifi_board->Sleep(kDeepSleepSeconds * 1000);
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
        L9110Driver* pump_control = static_cast<L9110Driver*>(actuator);
        pump_control->On();
        vTaskDelay(pdMS_TO_TICKS(seconds * 1000));
        pump_control->Off();
    }
}


void WateringApplication::ShowMessage(const std::string& message) {
    last_message_ = message;

    U8g2Display* display = static_cast<U8g2Display*>( Board::GetInstance().GetDisplay() );
    display->GetWindow()->SetText(3, last_message_);
}
