/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#ifndef _AUDIO_BUFFER_OUTPUT_H
#define _AUDIO_BUFFER_OUTPUT_H

#include "../audio_output.h"

/**
 * 缓存输出
 */
class AudioBufferOutput : public AudioOutput
{
public:
    AudioBufferOutput(AudioOutput *dest, int buf_size);
    virtual ~AudioBufferOutput();

    virtual uint32_t WriteSamples(const int16_t *data, uint32_t samples) override;
    virtual bool Stop() override;

    virtual void SetAudioConfig(const audio_config_t &config) override;
    
protected:
    AudioOutput *output;
    int buffSize;
    int16_t *leftSample;
    int16_t *rightSample;
    int writePtr;
    int readPtr;
    bool filled;
};

#endif //_AUDIO_BUFFER_OUTPUT_H

#endif