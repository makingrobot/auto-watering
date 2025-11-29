/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_MP3==1

#ifndef _MY_APPLICATION_H
#define _MY_APPLICATION_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Audio.h>

#include "src/framework/app/application.h"
#include "src/framework/app/device_state.h"
#include "mp3_window.h"

class Mp3Application : public Application {
public:
    Mp3Application();
    ~Mp3Application();
    
    void OnInit() override;

    void SetDeviceState(const DeviceState* state) override;

    const std::string& GetAppName() const override { return "DEMO_MP3"; }
    const std::string& GetAppVersion() const override { return "1.0.0"; }

private:
    Audio* audio_ = nullptr;
    uint8_t mp3_index_ = 0;
    uint8_t mp3_total_ = 0;
    TaskHandle_t audio_task_handle_ = nullptr;

    void AudioInfo(Audio::msg_t m);
    void AudioPlayEnd();

    Mp3Window* window_ = nullptr;

};

#endif //_MY_APPLICATION_H

#endif //APP_DEMO_MP3