/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#include "audio_i2s_simplex.h"

#include <cmath>
#include <cstring>
#include <Arduino.h>

#include "../../sys/log.h"

#define TAG "AudioI2sSimplex"

bool AudioI2sSimplex::Init(const audio_config_t &config)
{
    Log::Info(TAG, "init...");
    AudioI2sCodec::Init(config);

    i2s_data_bit_width_t bps = GetDataBitWidth(config.bits);
    i2s_slot_mode_t ch = GetChannel(config.channels);

    if (i2s_driver_) 
    {
        bool ret = i2s_driver_->begin(I2S_MODE_STD, (int)config.rate, bps, ch);
        if (!ret) {
            Log::Error(TAG, "Failed to initialize I2S tx!");
            return false;
        }
    } 
    
    return true;
}

AudioI2sSimplex::~AudioI2sSimplex() 
{
    if (i2s_driver_) 
    {
        i2s_driver_->end();
    }
}

uint32_t AudioI2sSimplex::Write(const int16_t* data, uint32_t samples) 
{    
    if (!output_enabled_) return 0;
    
    // 根据音量调整值
    // TODO: 
    size_t bytes_write = i2s_driver_->write((const uint8_t*)data, samples*2);
    return bytes_write;
}

uint32_t AudioI2sSimplex::Read(int16_t* dest, uint32_t samples) 
{
    if (!input_enabled_) return 0;

    size_t bytes_read = i2s_driver_->readBytes((char *)dest, samples*2);
    return bytes_read;
}

#endif //CONFIG_USE_AUDIO