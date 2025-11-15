/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <Arduino.h>

#include <string>
#include <deque>
#include <mutex>
#include <Ticker.h>

#include "config.h"
#include "src/ota/ota.h"
#include "device_state.h"
#include "event_handler.h"
#include "types.h"

void* create_application();

class Application {
public:
    static Application& GetInstance() {
        static Application* instance = static_cast<Application*>(create_application());
        return *instance;
    }

    Application();
    virtual ~Application();

    void Schedule(callback_function_t callback);
    DeviceState* GetDeviceState() { return device_state_; }
    void Alert(const char* status, const char* message, const char* emotion);
    void Reboot();
    void EventLoop();

    virtual void Init();
    virtual void Start();
    virtual void SetDeviceState(const DeviceState* state);
    virtual void DismissAlert();
    virtual void ToggleWorkState();
    virtual void ShowWifiConfigHit(std::string ssid, std::string config_url, std::string mac_address);
    virtual bool CanEnterSleepMode();

    virtual bool OnPhysicalButtonEvent(const std::string& button_name, const ButtonAction action);
    virtual bool OnDisplayTouchEvent(const TouchPoint_t& point);
    virtual bool OnSensorData(const std::string& sensor_name, int value) { }
    virtual bool OnSensorData(const std::string& sensor_name, const std::string& sensor_value) { }

    const std::string& GetLastErrorMessage() const { return last_error_message_; }

    virtual const std::string& GetAppName() const = 0;
    virtual const std::string& GetAppVersion() const = 0;

#if CONFIG_CLOCK_ENABLE==1
    virtual void OnClockTimer();
#endif

protected:
    virtual void OnStateChanged();

    DeviceState* device_state_ = const_cast<DeviceState*>(kDeviceStateUnknown);
    std::string last_error_message_;
    EventGroupHandle_t event_group_;
    EventHandler* event_handler_ = nullptr;
    std::mutex mutex_;
    int clock_ticks_ = 0;

private:
    Application(const Application&) = delete; // 禁用拷贝构造函数
    Application& operator=(const Application&) = delete; // 禁用赋值操作

    std::deque<callback_function_t> app_tasks_;
    bool has_server_time_ = false;

#if CONFIG_CLOCK_ENABLE==1
    Ticker* clock_ticker_;
#endif

#if CONFIG_OTA_ENABLE==1
    void CheckNewVersion(Ota& ota);
#endif

};

#endif //_APPLICATION_H
