/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */

#ifndef _WATERING_APPLICATION_H
#define _WATERING_APPLICATION_H

#include "../application.h"
#include "mqtt_service.h"
#include <string>

class WateringApplication : public Application {
public:
    WateringApplication();
    ~WateringApplication();
    
    void Init() override;
    void Start() override;

    bool OnPhysicalButtonEvent(const std::string& button_name, const ButtonAction action) override;
    bool OnSensorDataEvent(const std::string& sensor_name, int value) override;
    void ShowWifiConfigHit(const std::string& ssid, const std::string& config_url, const std::string& mac_address) override;

    const std::string& GetAppName() const override { return "AutoWatering"; }
    const std::string& GetAppVersion() const override { return "1.0.0"; }

private:
    void OnIotMessageEvent(const std::string& topic, const std::string& payload);
    void DoWatering(uint8_t seconds);
    void ShowMessage(const std::string& message);

    MqttService *mqtt_service_ = nullptr;
    int collect_count_ = 0;
    bool started_ = false;
    int soil_moilture_value_ = 0;
    std::string last_message_;

    std::string iot_broker_;
    std::string iot_username_;
    std::string iot_password_;
    std::string pump_control_topic_;
    std::string soil_moilture_topic_;
};

#endif //_WATERING_APPLICATION_H
