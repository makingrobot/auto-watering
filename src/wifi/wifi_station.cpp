/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "wifi_station.h"

#include <WiFi.h>
#include <esp_mac.h>
#include <esp_wifi.h>
#include "ssid_manager.h"
#include "src/sys/log.h"

#define TAG "WiFiStation"

void WifiStation::OnScanBegin(std::function<void()> callback) {
    on_scanbegin_handler_ = callback;
}

void WifiStation::OnConnect(std::function<void(std::string&)> callback) {
    on_connect_handler_ = callback;
}

void WifiStation::OnConnected(std::function<void(std::string&)> callback) {
    on_connected_handler_ = callback;
}

void WifiStation::Start() {

    if (on_scanbegin_handler_!=nullptr) {
        on_scanbegin_handler_();
    } 

    // step1: 取得已存热点
    const std::vector<SsidItem>& saved_list = SsidManager::GetInstance().GetSsidList();
    if (saved_list.size() == 0) {
        return;
    }

    // step2: 扫描可连热点
    int n = WiFi.scanNetworks();
    
    std::vector<std::string> ap_list;

    for (int i=0; i<n; ++i) {
        //Log::Debug(TAG, "SSID: %s, RSSI: %d, Authmode: %d",
        //    WiFi.SSID(i).c_str(), WiFi.RSSI(i), WiFi.encryptionType(i) );
        ap_list.push_back(std::string(WiFi.SSID(i).c_str()));
    }

    WiFi.scanDelete();

    // step3: 查找匹配的热点
    for (auto item : saved_list) {
        auto it = std::find(ap_list.begin(), ap_list.end(), item.ssid);
        if (it != ap_list.end()) {
            current_ssid_ = *it;
            break;
        }
    }

}

void WifiStation::Stop() {

    WiFi.disconnect();
}

bool WifiStation::WaitForConnected(uint32_t timeout_ms) {

    if (on_connect_handler_!=nullptr) {
        on_connect_handler_(current_ssid_);
    }
    
    const SsidItem* ssid = SsidManager::GetInstance().GetSsid(current_ssid_);
    if (ssid==nullptr) {
        return false;
    }

    // step3: 连接
    WiFi.begin(current_ssid_.c_str(), ssid->password.c_str());
    int n = 0;
    int step = 100;
    while (n < timeout_ms) {
        vTaskDelay(pdMS_TO_TICKS(step));
        if (WiFi.status() == WL_CONNECTED) {
            break; //已连接，中止等待
        }
        n += step;
    }

    if (WiFi.status() == WL_CONNECTED) {
        if (on_connected_handler_!=nullptr) {
            on_connected_handler_(current_ssid_);
        }
        is_connected_ = true;
        WiFi.setAutoReconnect(true);
        return true;
    }

    return false;
}

bool WifiStation::WaitForConnected(const std::string& ssid, const std::string& password, uint32_t timeout_ms) {

    if (on_connect_handler_!=nullptr) {
        on_connect_handler_(current_ssid_);
    }
    
    // step3: 连接
    WiFi.begin(ssid.c_str(), password.c_str());
    int n = 0;
    int step = 100;
    while (n < timeout_ms) {
        vTaskDelay(pdMS_TO_TICKS(step));
        if (WiFi.status() == WL_CONNECTED) {
            break; //已连接，中止等待
        }
        n += step;
    }

    if (WiFi.status() == WL_CONNECTED) {
        if (on_connected_handler_!=nullptr) {
            on_connected_handler_(current_ssid_);
        }
        is_connected_ = true;
        current_ssid_ = ssid;
        WiFi.setAutoReconnect(true);
        return true;
    }

    return false;
}

int8_t WifiStation::GetRssi() {
    return WiFi.RSSI();
}

uint8_t WifiStation::GetChannel() {
    return 0;
}

std::string WifiStation::GetIpAddress() {
    return std::string(WiFi.localIP().toString().c_str());
}

void WifiStation::SetPowerSaveMode(bool enabled) {
    ESP_ERROR_CHECK(esp_wifi_set_ps(enabled ? WIFI_PS_MIN_MODEM : WIFI_PS_NONE));
}
