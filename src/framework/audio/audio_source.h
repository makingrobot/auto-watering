/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#ifndef _AUDIO_SOURCE_H
#define _AUDIO_SOURCE_H

#include "audio_status.h"

/**
 * 音频源基类
 */
class AudioSource {
public:
    virtual uint32_t Read(uint8_t *data, uint32_t len) = 0;
    virtual bool Seek(int32_t pos, int dir) = 0;
    virtual bool Close() = 0;
    virtual const char* Tag() = 0;

    virtual bool Init() { return true; }
    virtual size_t GetPosition() { return 0; }
    virtual size_t GetSize() { return 0; }

    AudioStatus* Status() { return &status; }

    virtual void SetMetadataCallback(AudioStatus::MetadataCallbackFn fn, void *data) {
        status.RegisterMetadataCallback(fn, Tag(), data);
    }

    virtual void SetStatusCallback(AudioStatus::StatusCallbackFn fn, void *data) { 
        status.RegisterStatusCallback(fn, Tag(), data);
    }

protected:
    AudioStatus status;

};

#endif // _AUDIO_SOURCE_H

#endif //CONFIG_USE_AUDIO