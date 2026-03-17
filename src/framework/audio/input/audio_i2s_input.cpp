/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#include "audio_i2s_input.h"
#include "../../sys/log.h"

#define TAG "I2sInput"

bool AudioI2sInput::Init() 
{
    codec_->Init(config_);
    return true;
}

bool AudioI2sInput::Handle() 
{
    uint32_t bytes_read = codec_->Read(samples_, 2);
    return true;
}

bool AudioI2sInput::Close() 
{
    return false;
}

bool AudioI2sInput::isEOF() 
{
    return false;
}

int16_t* AudioI2sInput::GetSamples() 
{
    return samples_;
}

#endif //CONFIG_USE_AUDIO