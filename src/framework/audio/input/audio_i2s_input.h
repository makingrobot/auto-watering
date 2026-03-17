/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#ifndef _AUDIO_I2S_INPUT_H
#define _AUDIO_I2S_INPUT_H

#include "../audio_codec.h"
#include "../audio_input.h"

/**
 * I2s设备输入
 */
class AudioI2sInput : public AudioInput {
public:
    AudioI2sInput(AudioCodec *codec) : codec_(codec) {  }
    virtual ~AudioI2sInput() {  }

    bool Init() override;
    bool Handle() override;
    bool Close() override;
    bool isEOF() override;
    int16_t* GetSamples() override;

    const char* Tag() override { return "I2sInput"; };

private:
    AudioCodec *codec_;
    int16_t samples_[2];
    
};

#endif // _AUDIO_I2S_INPUT_H

#endif //CONFIG_USE_AUDIO