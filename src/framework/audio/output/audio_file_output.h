/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#ifndef _AUDIO_FILE_OUTPUT_H
#define _AUDIO_FILE_OUTPUT_H

#include <FS.h>
#include "../audio_output.h"

/**
 * 文件输出
 */
class AudioFileOutput : public AudioOutput {
public:
    AudioFileOutput(fs::File &file) : file_(&file) {  }
    virtual ~AudioFileOutput() { if (file_) file_->close(); }

    bool Init() override;
    uint32_t WriteSamples(const int16_t *data, uint32_t samples) override;
    bool Stop() override;
    const char* Tag() override { return "FileOutput"; };

private:
    fs::File *file_;

};

#endif // _AUDIO_FILE_OUTPUT_H

#endif