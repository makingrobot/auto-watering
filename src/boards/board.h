/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _BOARD_H
#define _BOARD_H

#include <string>
#include <map>
#include "config.h"
#include "src/led/led.h"
#include "src/display/display.h"
#include "src/display/disp_driver.h"
#include "src/display/backlight.h"
#include "src/audio/audio_codec.h"
#include "src/camera/camera.h"
#include "src/file/file_system.h"
#include "src/sys/time.h"
#include "src/app/types.h"
#include "src/peripheral/actuator.h"
#include "src/peripheral/sensor.h"

void* create_board();

static const char* kBootButton = "boot_button";

class Board {
private:
    Board(const Board&) = delete; // 禁用拷贝构造函数
    Board& operator=(const Board&) = delete; // 禁用赋值操作

    std::map<std::string, Actuator*> actuator_map_; //执行器外设列列表
    std::map<std::string, Sensor*> sensor_map_; //传感器外设列表

protected:
    Board();
    std::string GenerateUuid();

    // 软件生成的设备唯一标识
    std::string uuid_;

    virtual void EnterSleepMode();
    virtual void ExitSleepMode();
    virtual void Shutdown();

    virtual void AddActuator(const std::string& name, Actuator* actuator) {
        actuator_map_[name] = actuator;
    }
    virtual void AddSensor(const std::string& name, Sensor* sensor) {
        sensor_map_[name] = sensor;
    }
   
public:
    static Board& GetInstance() {
        static Board* instance = static_cast<Board*>(create_board());
        return *instance;
    }

    // virtual methods
    virtual ~Board() = default;

    virtual std::string GetUuid() { return uuid_; }
    virtual std::string GetJson();
    virtual std::string GetBoardType() = 0;
    virtual std::string GetBoardJson() = 0;
    virtual std::string GetDeviceStatusJson() = 0;
    virtual void SetPowerSaveMode(bool enabled) = 0; 
    virtual bool GetTemperature(float& temp) { return false; }
    virtual bool GetBatteryLevel(int &level, bool& charging, bool& discharging) { return false; }

    virtual const char* GetNetworkStateIconName() = 0;

    virtual bool OnPhysicalButtonEvent(const std::string& button_name, const ButtonAction action);
    virtual bool OnDisplayTouchEvent(const TouchPoint_t& point);

    virtual Led* GetLed();

    virtual DispDriver* GetDispDriver() { return nullptr; }
    virtual Backlight* GetBacklight() { return nullptr; }
    virtual Time* GetTime() { return nullptr; }

    // 查找执行器外设
    virtual Actuator* GetActuator(const std::string& name) {
        auto it = actuator_map_.find(name);
        if (it != actuator_map_.end()) {
            return it->second;
        }
        return nullptr;
    }

    // 查找传感器外设
    virtual Sensor* GetSensor(const std::string& name) {
        auto it = sensor_map_.find(name);
        if (it != sensor_map_.end()) {
            return it->second;
        }
        return nullptr;
    }

    virtual Display* GetDisplay();
#if CONFIG_USE_LVGL==1
    virtual void SetDisplay(Display *display) = 0;
#endif

#if CONFIG_USE_AUDIO==1
    virtual AudioCodec* GetAudioCodec() { return nullptr; }
#endif

#if CONFIG_USE_FS==1
    virtual FileSystem* GetFileSystem() { return nullptr; }
#endif

#if CONFIG_USE_CAMERA==1
    virtual Camera* GetCamera() { return nullptr; }
#endif
};

#endif  //_BOARD_H