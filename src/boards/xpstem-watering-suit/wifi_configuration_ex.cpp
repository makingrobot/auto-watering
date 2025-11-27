#include "wifi_configuration_ex.h"

#include <cJSON.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "src/sys/log.h"
#include "src/sys/settings.h"
#include "src/wifi/ssid_manager.h"
#include "wifi_configuration_res.h"

#define TAG "WifiConfigurationEx"

void WifiConfigurationEx::BindSsidRoute() {

    // GET / 
    web_server_->on("/", HTTP_GET, [this](AsyncWebServerRequest *request){ 
        request->send(200, "text/html", index_html); 
    });

    // GET / 
    web_server_->on("/mini.css", HTTP_GET, [this](AsyncWebServerRequest *request){ 
        request->send(200, "text/css", mini_css); 
    });

    // GET / 
    web_server_->on("/zepto.js", HTTP_GET, [this](AsyncWebServerRequest *request){ 
        request->send(200, "text/javascript", zepto_js); 
    });

    // POST /submit --json
    web_server_->on("/submit", HTTP_POST, [this](AsyncWebServerRequest *request){ 
        
            Log::Debug(TAG, "request body: %s", this->request_data_);
            
            cJSON *json = cJSON_Parse(this->request_data_);

            std::string ssid = cJSON_GetObjectItem(json, "ssid")->valuestring;
            std::string password = cJSON_GetObjectItem(json, "password")->valuestring;
            std::string serialno = cJSON_GetObjectItem(json, "serialno")->valuestring;
            int workmode = cJSON_GetObjectItem(json, "workmode")->valueint;

            Log::Info(TAG, "ssid: %s, password: %s", ssid.c_str(), password.c_str());
            if (!this->ConnectToWifi(ssid, password)) {
                request->send(200, "application/json", "{\"success\":false,\"error\":\"无法连接到 WiFi\"}");
                return;
            }

            bool read_config = this->ReadProductConfig(serialno, workmode);
            WiFi.disconnect();

            if (!read_config) {
                request->send(200, "application/json", "{\"success\":false,\"error\":\"获取产品配置失败！\"}");
                return;
            }

            Log::Info(TAG, "Save SSID %s %d", ssid.c_str(), ssid.length());
            SsidManager::GetInstance().AddSsid(ssid, password);

            request->send(200, "application/json", "{\"success\":true}"); 

        }, NULL, [this](AsyncWebServerRequest *request, uint8_t *bodyData, size_t bodyLen, size_t index, size_t total){ 
            this->request_data_ = reinterpret_cast<char*>(bodyData);
        });

    Log::Info(TAG, "bind ssid route.");
}

void WifiConfigurationEx::BindAdvancedRoute() {

}

bool WifiConfigurationEx::ReadProductConfig(const std::string& serialno, int workmode) {
    
    Log::Info(TAG, "read product config, serialno: %s", serialno);
    
    // 获取项目配置信息
    String config_url = String("https://www.xpstem.com/app/iot/project/productconfig?serialno=") + String(serialno.c_str());
    HTTPClient http;
    http.begin(config_url);
    int status_code = http.GET();
    if (status_code != 200) {
        return false;
    }

    const char *body = http.getString().c_str();
    http.end();

    // 解析
    cJSON *root_node = cJSON_Parse(body);
    if (!root_node) {
        const char *error = cJSON_GetErrorPtr();
        Log::Error(TAG, "解析配置错误，%s", error);
        return false;
    }

    Settings settings("config");
    settings.SetInt("workmode", workmode);
    settings.SetString("serialno", serialno);
    settings.SetString("configdata", std::string(body));

    return true;
}