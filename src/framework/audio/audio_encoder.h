/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#ifndef _AUDIO_ENCODER_H
#define _AUDIO_ENCODER_H

#include <driver/gpio.h>

/**
 * 音频编码器基类
 */
class AudioEncoder {
public:
    virtual bool Init() { return false; }
    virtual bool Encode(const uint16_t *data, uint32_t samples) = 0;
    
};

#endif // _AUDIO_ENCODER_H

#endif //CONFIG_USE_AUDIO