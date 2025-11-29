#ifndef _WATERING_CONFIG_H
#define _WATERING_CONFIG_H

#include <string>
#include <cJSON.h>
#include "src/framework/sys/settings.h"

class WateringConfig {
public:
    static WateringConfig& GetInstance() {
        static WateringConfig instance;
        return instance;
    }

    const std::string& project_id() const { return project_id_; }
    const std::string& mqtt_server() const { return mqtt_server_; }
    const int mqtt_port() const { return mqtt_port_; }
    const std::string& mqtt_username() const { return mqtt_username_; }
    const std::string& mqtt_password() const { return mqtt_password_; }
    const std::string& pump_control_topic() const { return pump_control_topic_; }
    const std::string& soil_moisture_topic() const { return soil_moisture_topic_; }
    const std::string& soil_moisture_dataname() const { return soil_moisture_dataname_; }

private:
    WateringConfig() {
        Settings settings("config");
        std::string data = settings.GetString("configdata", "");
        if (data == "") {
            return;
        }

        cJSON *root_node = cJSON_Parse(data.c_str());
        cJSON *data_node = cJSON_GetObjectItem(root_node, "data");

        int version = cJSON_GetObjectItem(data_node, "version")->valueint;
        project_id_ = cJSON_GetObjectItem(data_node, "projectId")->valuestring;

        if (version==1) {
            // peripherals
            cJSON *peri_node = cJSON_GetObjectItem(data_node, "peripherals");

            cJSON *soil_moisture_node = cJSON_GetObjectItem(peri_node, "soilMoistureSensor");
            std::string soil_moisture_id = cJSON_GetObjectItem(soil_moisture_node, "id")->valuestring;
            soil_moisture_topic_ = std::string("user/") + soil_moisture_id + std::string("/data");

            cJSON *soil_moisture_config_node = cJSON_GetObjectItem(soil_moisture_node, "config");
            soil_moisture_dataname_ = cJSON_GetObjectItem(soil_moisture_config_node, "dataname")->valuestring;

            cJSON *pump_control_node = cJSON_GetObjectItem(peri_node, "pumpControlRelay");
            std::string pump_control_id = cJSON_GetObjectItem(pump_control_node, "id")->valuestring;
            pump_control_topic_ = std::string("user/") + pump_control_id + std::string("/ctrl");
        }

        // iot
        cJSON *iot_node = cJSON_GetObjectItem(data_node, "iot");
        mqtt_server_ = cJSON_GetObjectItem(iot_node, "mqttServer")->valuestring;
        mqtt_port_ = cJSON_GetObjectItem(iot_node, "mqttPort")->valueint;
        mqtt_username_ = cJSON_GetObjectItem(iot_node, "mqttUsername")->valuestring;
        mqtt_password_ = cJSON_GetObjectItem(iot_node, "mqttPassword")->valuestring;
    }

    std::string project_id_;
    std::string mqtt_server_ = "iot.xpstem.com";
    int mqtt_port_ = 1883;
    std::string mqtt_username_;
    std::string mqtt_password_;
    std::string pump_control_topic_;
    std::string soil_moisture_topic_;
    std::string soil_moisture_dataname_;

};

#endif //_WATERING_CONFIG_H