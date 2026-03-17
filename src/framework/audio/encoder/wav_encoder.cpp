/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_AUDIO_CODER_WAV==1

#include <Arduino.h>
#include "wav_encoder.h"

WavEncoder::WavEncoder()
{
    buffSize = 128;
    buff = NULL;
    buffPtr = 0;
    buffLen = 0;
}

WavEncoder::~WavEncoder()
{
    free(buff);
    buff = NULL;
}

bool WavEncoder::Init()
{
    return true;
}

bool WavEncoder::Encode(const uint16_t *data, uint32_t samples)
{
    return false;
}

#endif