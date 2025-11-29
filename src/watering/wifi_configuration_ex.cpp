#include "wifi_configuration_ex.h"

#include <cJSON.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "src/framework/sys/log.h"
#include "src/framework/sys/settings.h"
#include "src/framework/wifi/ssid_manager.h"
#include "wifi_configuration_res.h"

#define TAG "WifiConfigurationEx"

void _webServerTask(void *pvParam) {
    WebServer* server = (WebServer*)pvParam;
    while (1) {
        server->handleClient();
        delay(10);
    }
}

void _rebootTask(void* pvParam) {
    // 等待200ms确保HTTP响应完全发送
    vTaskDelay(pdMS_TO_TICKS(200));
    // 停止Web服务器
    WebServer* server = (WebServer*)pvParam;
    if (server!=nullptr) {
        server->stop();
    }
    // 再等待100ms确保所有连接都已关闭
    vTaskDelay(pdMS_TO_TICKS(100));
    // 执行重启
    esp_restart();
}

void WifiConfigurationEx::StartWebServer() {

    web_server_ = new WebServer(80);

    // GET / 
    web_server_->on("/", [this](){ 
        web_server_->send(200, "text/html", index_html); 
    });

    // GET / 
    web_server_->on("/mini.css", [this](){ 
        web_server_->send(200, "text/css", mini_css); 
    });

    // GET / 
    web_server_->on("/zepto.js", [this](){ 
        web_server_->send(200, "text/javascript", zepto_js); 
    });

    // POST /submit --json
    web_server_->on("/submit", [this](){ 
        
            // post参数，form表单
            std::string ssid = std::string(web_server_->arg("ssid").c_str());
            std::string password = std::string(web_server_->arg("password").c_str());
            std::string serialno = std::string(web_server_->arg("serialno").c_str());  //序列号
            int workmode = web_server_->arg("workmode").toInt();     //工作模式

            if (ssid=="" || password=="" || serialno=="") {
                web_server_->send(200, "application/json", "{\"success\":false,\"error\":\"参数不能为空\"}");
                return;
            }

            Log::Info(TAG, "ssid: %s, password: %s", ssid.c_str(), password.c_str());
            if (!this->ConnectToWifi(ssid, password)) {
                web_server_->send(200, "application/json", "{\"success\":false,\"error\":\"无法连接到 WiFi\"}");
                return;
            }

            bool read_config = ReadProductConfig(serialno, workmode);
            WiFi.disconnect();

            if (!read_config) {
                web_server_->send(200, "application/json", "{\"success\":false,\"error\":\"获取产品配置失败！\"}");
                return;
            }

            Log::Info(TAG, "Save SSID %s %d", ssid.c_str(), ssid.length());
            SsidManager::GetInstance().AddSsid(ssid, password);

            web_server_->send(200, "application/json", "{\"success\":true}"); 

        });

        
    // GET /done.html
    web_server_->on("/done.html", [this](){ 
        web_server_->send(200, "text/html", done_html); 
    });
    
    // POST /reboot
    web_server_->on("/reboot", [this](){ 
        web_server_->send(200, "application/json", "{\"success\":true}"); 

        // 创建一个延迟重启任务
        Log::Info(TAG, "Rebooting..." );
        xTaskCreate(_rebootTask, "reboot_task", 4096, web_server_, 5, NULL);
    });

    web_server_->begin();
    
    xTaskCreate(_webServerTask, "WebServer_Task", 8192, web_server_, 1, &web_task_handler_);

    Log::Info(TAG, "WebServer started.");
}

bool WifiConfigurationEx::ReadProductConfig(const std::string& serialno, int workmode) {
    
    Log::Info(TAG, "read product config, serialno: %s", serialno.c_str());
    
    // 获取项目配置信息
    String config_url = String("https://www.xpstem.com/app/iot/project/productconfig?serialno=") + String(serialno.c_str());
    HTTPClient http;
    http.begin(config_url);
    int status_code = http.GET();
    if (status_code != 200) {
        Log::Warn(TAG, "read product config failure, status code: %d", status_code);
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

    Settings settings("config", true);
    settings.SetInt("workmode", workmode);
    settings.SetString("serialno", serialno);
    settings.SetString("configdata", std::string(body));
    Log::Info(TAG, "write to ns:config successfully.");

    return true;
}