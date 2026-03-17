
/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#ifndef _AUDIO_ENCODER_OUTPUT_H
#define _AUDIO_ENCODER_OUTPUT_H

#include <string>
#include "../audio_output.h"
#include "../audio_encoder.h"

/**
 * 编码输出
 */
class AudioEncoderOutput : public AudioOutput {
public:
    AudioEncoderOutput(AudioOutput *output, const std::string& out_format);
    virtual ~AudioEncoderOutput();

    bool Init() override;
    uint32_t WriteSamples(const int16_t *data, uint32_t samples) override;
    const char* Tag() override { return "EncoderOutput"; };

private:
    AudioOutput *output_;
    AudioEncoder *encoder_;
    const std::string out_format_;

};

#endif // _AUDIO_I2S_OUTPUT_H

#endif