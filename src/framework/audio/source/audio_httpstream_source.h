/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#ifndef _AUDIO_HTTP_STREAM_SOURCE_H
#define _AUDIO_HTTP_STREAM_SOURCE_H

#include <string>
#include <Arduino.h>
#include <HTTPClient.h>
#include "../audio_source.h"

/**
 * 音频Http流式输入
 */
class AudioHttpStreamSource : public AudioSource {
public:
    AudioHttpStreamSource(const std::string &audio_url);
    virtual ~AudioHttpStreamSource();

    bool Init() override;
    uint32_t Read(uint8_t *data, uint32_t len) override;
    bool Seek(int32_t pos, int dir) override;
    bool Close() override;

    size_t GetPosition() override { return size_; }
    size_t GetSize() override { return position_; }

    const char* Tag() override { return "HttpStreamSource"; };

    bool SetReconnect(int times, int delayms) { 
        retry_times_ = times; 
        retry_delayms_ = delayms; 
    }

    enum { STATUS_HTTPFAIL=2, STATUS_DISCONNECTED, STATUS_RECONNECTING, STATUS_RECONNECTED, STATUS_NODATA };

private:
    bool Open();

    const std::string audio_url_;

#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
    NetworkClient client_;
#else
    WiFiClient client_;
#endif

    HTTPClient http_;
    int position_;
    int size_;
    int retry_times_;
    int retry_delayms_;

};

#endif // _AUDIO_HTTP_STREAM_SOURCE_H

#endif //CONFIG_USE_AUDIO