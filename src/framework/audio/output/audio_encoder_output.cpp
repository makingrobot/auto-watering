/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#include <Arduino.h>
#include "audio_encoder_output.h"
#include "../encoder/wav_encoder.h"
#include "../../sys/log.h"

#define TAG "EncoderOutput"

AudioEncoderOutput::AudioEncoderOutput(AudioOutput *output, const std::string& out_format) 
    : output_(output), out_format_(out_format) {

}
    
AudioEncoderOutput::~AudioEncoderOutput() {

}

bool AudioEncoderOutput::Init() {
    bool ret = output_->Init();
    if (!ret) {
        Log::Error(TAG, "output source init fail.");
        return false;
    }

    // 创建编码器
    if (out_format_ == "wav") {
#if CONFIG_AUDIO_CODER_WAV==1
        encoder_ = new WavEncoder();
#else
        #pragma message("Please set CONFIG_AUDIO_CODER_WAV=1 in config file when use WAV encode.")
#endif
    }
    
    if (!encoder_) {
        Log::Error(TAG, "encoder is null.");
        return false;
    }

    ret = encoder_->Init();
    if (!ret) {
        Log::Error(TAG, "encoder init fail.");
        return false;
    }

    return true;
}

uint32_t AudioEncoderOutput::WriteSamples(const int16_t *data, uint32_t samples) {
    // 编码处理
    return 0;
}

#endif