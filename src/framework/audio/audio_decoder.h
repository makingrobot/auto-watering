/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#ifndef _AUDIO_DECODER_H
#define _AUDIO_DECODER_H

#include "audio_input.h"

/**
 * 音频解码器基类
 */
class AudioDecoder {
public:
    virtual bool Init() { return false; }
    virtual bool Decode() = 0;

    bool isEOF() { return eof_; }
    int16_t* samples() { return samples_; }
    uint16_t channels() const { return channels_; }
    uint32_t sampleRate() const { return sampleRate_; }
    uint16_t bitsPerSample() const { return bitsPerSample_; }

protected:
    bool eof_ = false;
    int16_t samples_[2];
    uint16_t channels_ = 1;
    uint32_t sampleRate_ = 16000;
    uint16_t bitsPerSample_ = 16;

};

#endif // _AUDIO_DECODER_H

#endif //CONFIG_USE_AUDIO