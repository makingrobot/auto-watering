/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#ifndef _AUDIO_PIPE_H
#define _AUDIO_PIPE_H

#include <freertos/FreeRTOS.h>

#include "audio_input.h"
#include "audio_output.h"
#include "audio_encoder.h"
#include "audio_decoder.h"
#include "audio_status.h"

/**
 * 音频管道
 */
class AudioPipe {
public:
    AudioPipe() { }

    virtual void Start(AudioInput *input,  AudioOutput *output);
    virtual void Stop();

    void MetadataCallback(const char *tag, const char *type, const char *text);
    void StatusCallback(const char *tag, int code, const char *text);
    
protected:
    void Execute();
    
    int16_t last_sample_[2];

private:
    volatile bool running_;

    AudioInput *input_ = nullptr;
    AudioOutput *output_ = nullptr;

    TaskHandle_t task_handle_;

};

#endif // _AUDIO_PIPE_H

#endif 