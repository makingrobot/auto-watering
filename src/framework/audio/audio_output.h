/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#ifndef _AUDIO_OUTPUT_H
#define _AUDIO_OUTPUT_H

#include "audio_common.h"
#include "audio_status.h"

/**
 * 音频输出基类
 */
class AudioOutput {
public:
    virtual bool Init() { return false; }
    virtual uint32_t WriteSamples(const int16_t *data, uint32_t samples) = 0;
    virtual bool Stop() { return false; }
    virtual const char* Tag() = 0;

    virtual void SetAudioConfig(const audio_config_t &config) { config_ = config; }

    void SetMetadataCallback(AudioStatus::MetadataCallbackFn fn, void *data) {
        status.RegisterMetadataCallback(fn, Tag(), data);
    }

    void SetStatusCallback(AudioStatus::StatusCallbackFn fn, void *data) { 
        status.RegisterStatusCallback(fn, Tag(), data);
    }

protected:
    audio_config_t config_ = {
        .rate = SAMPLE_RATE_44K,
        .bits = SAMPLE_BITS_16,
        .channels = CHANNELS_2,
    };

    AudioStatus status;
};

#endif // _AUDIO_OUTPUT_H

#endif //CONFIG_USE_AUDIO