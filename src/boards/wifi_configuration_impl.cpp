/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_WIFI_CONFIGURE_ENABLE==1

#include "wifi_configuration_impl.h"
#include <WiFi.h>
#include <cJSON.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <esp_mac.h>
#include <esp_wifi.h>

#include "src/wifi/ssid_manager.h"
#include "src/sys/log.h"
#include "wifi_configuration_html.h"

#define TAG "WifiConfigurationImpl"

WifiConfigurationImpl::~WifiConfigurationImpl() {

    if (web_server_!=nullptr) {
        web_server_->end();
    }
}

void WifiConfigurationImpl::LoadAdvancedConfig() {
    // 加载高级配置
    nvs_handle_t nvs;
    esp_err_t err = nvs_open("wifi", NVS_READONLY, &nvs);
    if (err == ESP_OK) {
        // 读取OTA URL
        char ota_url[256] = {0};
        size_t ota_url_size = sizeof(ota_url);
        err = nvs_get_str(nvs, "ota_url", ota_url, &ota_url_size);
        if (err == ESP_OK) {
            ota_url_ = ota_url;
        }

        // 读取WiFi功率
        err = nvs_get_i8(nvs, "max_tx_power", &max_tx_power_);
        if (err == ESP_OK) {
            Log::Info(TAG, "WiFi max tx power from NVS: %d", max_tx_power_);
            ESP_ERROR_CHECK(esp_wifi_set_max_tx_power(max_tx_power_));
        } else {
            esp_wifi_get_max_tx_power(&max_tx_power_);
        }

        // 读取BSSID记忆设置
        uint8_t remember_bssid = 0;
        err = nvs_get_u8(nvs, "remember_bssid", &remember_bssid);
        if (err == ESP_OK) {
            remember_bssid_ = remember_bssid;
        } else {
            remember_bssid_ = true; // 默认值
        }

        nvs_close(nvs);
    }
}

void WifiConfigurationImpl::StartWebServer() {

    web_server_ = new AsyncWebServer(80);

    BindSsidRoute();

    BindAdvancedRoute();

    // POST /reboot
    web_server_->on("/reboot", HTTP_POST, [this](AsyncWebServerRequest *request){ 
        request->send(200, "application/json", "{\"success\":true}"); 

        // 创建一个延迟重启任务
        Log::Info(TAG, "Rebooting..." );
        xTaskCreate([](void *ctx) {
                // 等待200ms确保HTTP响应完全发送
                vTaskDelay(pdMS_TO_TICKS(200));
                // 停止Web服务器
                auto* self = static_cast<WifiConfigurationImpl*>(ctx);
                if (self->web_server_!=nullptr) {
                    self->web_server_->end();
                }
                // 再等待100ms确保所有连接都已关闭
                vTaskDelay(pdMS_TO_TICKS(100));
                // 执行重启
                esp_restart();
            }, "reboot_task", 4096, this, 5, NULL);
    });

    // GET /done.html
    web_server_->on("/done.html", [this](AsyncWebServerRequest *request){ 
        request->send(200, "text/html", done_html); 
    });
    
    web_server_->begin();
    
    Log::Info(TAG, "WebServer started");
}

void WifiConfigurationImpl::BindSsidRoute() {
    // GET / 
    web_server_->on("/", HTTP_GET, [this](AsyncWebServerRequest *request){ 
        request->send(200, "text/html", index_html); 
    });

    // POST /submit --json
    web_server_->on("/submit", HTTP_POST, [this](AsyncWebServerRequest *request){ 
        
            Log::Debug(TAG, "request body: %s", this->request_data_);
            
            cJSON *json = cJSON_Parse(this->request_data_);

            std::string ssid = cJSON_GetObjectItem(json, "ssid")->valuestring;
            std::string password = cJSON_GetObjectItem(json, "password")->valuestring;

            Log::Info(TAG, "ssid: %s, password: %s", ssid.c_str(), password.c_str());
            if (!this->ConnectToWifi(ssid, password)) {
                request->send(200, "application/json", "{\"success\":false,\"error\":\"无法连接到 WiFi\"}");
                return;
            }

            Log::Info(TAG, "Save SSID %s %d", ssid.c_str(), ssid.length());
            SsidManager::GetInstance().AddSsid(ssid, password);

            request->send(200, "application/json", "{\"success\":true}"); 
        }, NULL, [this](AsyncWebServerRequest *request, uint8_t *bodyData, size_t bodyLen, size_t index, size_t total){ 
            this->request_data_ = reinterpret_cast<char*>(bodyData);
        });

    // GET /scan  --json
    web_server_->on("/scan", HTTP_GET, [this](AsyncWebServerRequest *request){ 
        std::string json = this->GetAvailableAPList();
        request->send(200, "application/json", json.c_str()); 
    });

    // GET /saved/list  --json
    web_server_->on("/saved/list", HTTP_GET, [this](AsyncWebServerRequest *request){ 
        std::string json = this->GetSavedAPList();
        request->send(200, "application/json", json.c_str()); 
    });

    // GET /saved/set_default --json
    web_server_->on("/saved/set_default", HTTP_GET, [this](AsyncWebServerRequest *request){ 
        int index = request->getParam("index")->value().toInt();
        Log::Info(TAG, "Set default item %d", index);
        SsidManager::GetInstance().SetDefaultSsid(index);
        request->send(200, "application/json", "{}"); 
    });

    // GET /saved/delete  --json
    web_server_->on("/saved/delete", HTTP_GET, [this](AsyncWebServerRequest *request){ 
        int index = request->getParam("index")->value().toInt();
        Log::Info(TAG, "Delete saved list item %d", index);
        SsidManager::GetInstance().RemoveSsid(index);
        request->send(200, "application/json", "{}"); 
    });
}

void WifiConfigurationImpl::BindAdvancedRoute() {

    // GET /advanced/config --json
    web_server_->on("/advanced/config", HTTP_GET, [this](AsyncWebServerRequest *request){ 
        
        // 创建JSON对象
        cJSON *json = cJSON_CreateObject();
        if (!json) {
            request->send(500, "text/plain", "Failed to create JSON");
            return ESP_FAIL;
        }

        // 添加配置项到JSON
        if (!this->ota_url_.empty()) {
            cJSON_AddStringToObject(json, "ota_url", this->ota_url_.c_str());
        }
        cJSON_AddNumberToObject(json, "max_tx_power", this->max_tx_power_);
        cJSON_AddBoolToObject(json, "remember_bssid", this->remember_bssid_);

        // 发送JSON响应
        char *json_str = cJSON_PrintUnformatted(json);
        cJSON_Delete(json);
        if (!json_str) {
            request->send(500, "text/plain", "Failed to print JSON");
            return ESP_FAIL;
        }

        request->send(200, "application/json", json_str); 
    });

    // POST /advanced/submit --json
    web_server_->on("/advanced/submit", HTTP_POST,  [this](AsyncWebServerRequest *request){
        Log::Debug(TAG, "request body: %s", this->request_data_);

        cJSON *json = cJSON_Parse(this->request_data_);

        // 打开NVS
        nvs_handle_t nvs;
        esp_err_t err = nvs_open("wifi", NVS_READWRITE, &nvs);
        if (err != ESP_OK) {
            cJSON_Delete(json);
            request->send(200, "text/plain", "Failed to open NVS");
            return ESP_FAIL;
        }

        // 保存OTA URL
        cJSON *ota_url = cJSON_GetObjectItem(json, "ota_url");
        if (cJSON_IsString(ota_url) && ota_url->valuestring) {
            this->ota_url_ = ota_url->valuestring;
            err = nvs_set_str(nvs, "ota_url", this->ota_url_.c_str());
            if (err != ESP_OK) {
                Log::Error(TAG, "Failed to save OTA URL: %d", err);
            }
        }

        // 保存WiFi功率
        cJSON *max_tx_power = cJSON_GetObjectItem(json, "max_tx_power");
        if (cJSON_IsNumber(max_tx_power)) {
            this->max_tx_power_ = max_tx_power->valueint;
            err = esp_wifi_set_max_tx_power(this->max_tx_power_);
            if (err != ESP_OK) {
                Log::Error(TAG, "Failed to set WiFi power: %d", err);
                request->send(200, "text/plain", "Failed to set WiFi power");
                return ESP_FAIL;
            }
            err = nvs_set_i8(nvs, "max_tx_power", this->max_tx_power_);
            if (err != ESP_OK) {
                Log::Error(TAG, "Failed to save WiFi power: %d", err);
            }
        }

        // 保存BSSID记忆设置
        cJSON *remember_bssid = cJSON_GetObjectItem(json, "remember_bssid");
        if (cJSON_IsBool(remember_bssid)) {
            this->remember_bssid_ = cJSON_IsTrue(remember_bssid);
            err = nvs_set_u8(nvs, "remember_bssid", this->remember_bssid_);
            if (err != ESP_OK) {
                Log::Error(TAG, "Failed to save remember_bssid: %d", err);
            }
        }

        // 提交更改
        err = nvs_commit(nvs);
        nvs_close(nvs);
        cJSON_Delete(json);

        if (err != ESP_OK) {
            request->send(200, "text/plain", "Failed to save configuration");
            return ESP_FAIL;
        }

        request->send(200, "application/json", "{\"success\":true}"); 
    
    }, NULL, [this](AsyncWebServerRequest *request, uint8_t *bodyData, size_t bodyLen, size_t index, size_t total){ 
        this->request_data_ = reinterpret_cast<char*>(bodyData);
    });

    LoadAdvancedConfig();
}

#endif //CONFIG_WIFI_CONFIGURE_ENABLE