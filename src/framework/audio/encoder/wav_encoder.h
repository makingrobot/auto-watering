/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_AUDIO_CODER_WAV==1

#ifndef _WAV_DECODER_H
#define _WAV_DECODER_H

#include "../audio_encoder.h"
#include "../audio_source.h"
#include "../audio_status.h"

class WavEncoder : public AudioEncoder
{
public:
    WavEncoder();
    virtual ~WavEncoder();
    bool Init() override;
    bool Encode(const uint16_t *data, uint32_t samples) override;
    void SetBufferSize(int sz) { buffSize = sz; }

private:
    
    uint32_t availBytes;

    // We need to buffer some data in-RAM to avoid doing 1000s of small reads
    uint32_t buffSize;
    uint8_t *buff;
    uint16_t buffPtr;
    uint16_t buffLen;
};

#endif //_WAV_ENCODER_H

#endif