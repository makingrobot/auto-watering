/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#include <vector>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include "audio_file_output.h"
#include "../../sys/log.h"

#define TAG "AudioFileOutput"

bool AudioFileOutput::Init() 
{
    return false;
}

uint32_t AudioFileOutput::WriteSamples(const int16_t *data, uint32_t samples)
{
    return 0;
}

bool AudioFileOutput::Stop() 
{
    return false;
}

#endif //CONFIG_USE_AUDIO