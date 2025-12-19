/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（vx: billyzh）
 */
#ifndef _WATERING_CONFIG_H
#define _WATERING_CONFIG_H

#include <string>
#include <cJSON.h>
#include "src/framework/sys/settings.h"
#include "src/framework/sys/log.h"

#define TAG "WateringConfig"

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

    void Update(const std::string& serialno, int workmode, cJSON *data_node) {

        int version = cJSON_GetObjectItem(data_node, "version")->valueint;
        std::string project_id = cJSON_GetObjectItem(data_node, "projectId")->valuestring;

        //if (version==1) {
            // peripherals
            cJSON *peri_node = cJSON_GetObjectItem(data_node, "peripherals");

            cJSON *soil_moisture_node = cJSON_GetObjectItem(peri_node, "soilMoistureSensor");
            std::string soil_moisture_id = cJSON_GetObjectItem(soil_moisture_node, "id")->valuestring;
            std::string soil_moisture_topic = std::string("user/") + soil_moisture_id + std::string("/data");

            cJSON *soil_moisture_config_node = cJSON_GetObjectItem(soil_moisture_node, "config");
            std::string soil_moisture_dataname = cJSON_GetObjectItem(soil_moisture_config_node, "dataname")->valuestring;

            cJSON *pump_control_node = cJSON_GetObjectItem(peri_node, "pumpControlRelay");
            std::string pump_control_id = cJSON_GetObjectItem(pump_control_node, "id")->valuestring;
            std::string pump_control_topic = std::string("user/") + pump_control_id + std::string("/ctrl");
        //}

        // iot
        cJSON *iot_node = cJSON_GetObjectItem(data_node, "iot");
        std::string mqtt_server = cJSON_GetObjectItem(iot_node, "mqttServer")->valuestring;
        int mqtt_port = cJSON_GetObjectItem(iot_node, "mqttPort")->valueint;
        std::string mqtt_username = cJSON_GetObjectItem(iot_node, "mqttUsername")->valuestring;
        std::string mqtt_password = cJSON_GetObjectItem(iot_node, "mqttPassword")->valuestring;
        
        Settings settings("config", true);
        settings.SetString("serialno", serialno);
        settings.SetInt("workmode", workmode);
        settings.SetString("projectid", project_id);
        settings.SetString("sm_topic", soil_moisture_topic);
        settings.SetString("sm_dataname", soil_moisture_dataname);
        settings.SetString("pc_topic", pump_control_topic);
        settings.SetString("mq_server",mqtt_server);
        settings.SetInt("mq_port", mqtt_port);
        settings.SetString("mq_username", mqtt_username);
        settings.SetString("mq_password", mqtt_password);
    }
    
private:
    WateringConfig() {
        Settings settings("config");
        std::string serialno = settings.GetString("serialno", "");
        if (serialno == "") {
            Log::Warn(TAG, "no configdata");
            return;
        }

        project_id_             = settings.GetString("projectid" );
        soil_moisture_topic_    = settings.GetString("sm_topic" );
        soil_moisture_dataname_ = settings.GetString("sm_dataname" );
        pump_control_topic_     = settings.GetString("pc_topic" );
        mqtt_server_            = settings.GetString("mq_server", "iot.xpstem.com" );
        mqtt_port_              = settings.GetInt("mq_port", 1883 );
        mqtt_username_          = settings.GetString("mq_username" );
        mqtt_password_          = settings.GetString("mq_password" );

        Log::Debug(TAG, "project_id: %s", project_id_.c_str());
        Log::Debug(TAG, "soil_moisture_topic: %s", soil_moisture_topic_.c_str());
        Log::Debug(TAG, "soil_moisture_dataname: %s", soil_moisture_dataname_.c_str());
        Log::Debug(TAG, "pump_control_topic: %s", pump_control_topic_.c_str());
        Log::Debug(TAG, "mqtt_server: %s", mqtt_server_.c_str());
        Log::Debug(TAG, "mqtt_username: %s", mqtt_username_.c_str());
        Log::Debug(TAG, "mqtt_password: %s", mqtt_password_.c_str());
    }

    std::string project_id_;
    std::string mqtt_server_;
    int mqtt_port_;
    std::string mqtt_username_;
    std::string mqtt_password_;
    std::string pump_control_topic_;
    std::string soil_moisture_topic_;
    std::string soil_moisture_dataname_;

};

#endif //_WATERING_CONFIG_H