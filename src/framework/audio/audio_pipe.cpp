/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 */
#include "config.h"
#if CONFIG_USE_AUDIO==1

#include "audio_pipe.h"
#include "../sys/log.h"

#define TAG "AudioPipe"

void AudioPipe::Start(AudioInput *input,  AudioOutput *output) {

    input_ = input;
    output_ = output;

    running_ = true;
    // 启动任务
    xTaskCreate(
        [](void *parameter) {
            Log::Info(TAG, "Pipe task running on core %d", xPortGetCoreID());
            
            AudioPipe* pipe = (AudioPipe *)parameter;
            pipe->Execute();

            vTaskDelete(NULL);
        },                      /* 任务函数 TaskFunction_t */
        "Pipe_Task",            /* 任务名称 const char* */
        8192,                   /* 堆栈字数 */
        this,                   /* 自定义参数指针 void* */
        tskIDLE_PRIORITY + 1,   /* 任务优先级 UBaseType_t */
        &task_handle_           /* 任务句柄指针 TaskHandle_t* */
    );
}

void _MetadataCallback(const char *tag, const char *type, const char *text, void *data) {
    AudioPipe *pipe = (AudioPipe *)data;
    pipe->MetadataCallback(tag, type, text);
}

void _StatusCallback(const char *tag, int code, const char *text, void *data) {
    AudioPipe *pipe = (AudioPipe *)data;
    pipe->StatusCallback(tag, code, text);
}

void AudioPipe::Execute() {

    // 初始化
    bool ret = input_->Init();
    if (!ret) {
        Log::Error(TAG, "audio input init fail.");
        return;
    }

    ret = output_->Init();
    if (!ret) {
        Log::Error(TAG, "audio output init fail.");
        return;
    }

    input_->SetMetadataCallback(_MetadataCallback, this);
    input_->SetStatusCallback(_StatusCallback, this);

    output_->SetStatusCallback(_StatusCallback, this);

    // 处理数据
    // AudioInput使用Handle方法拉取数据；
    // AudioOutput使用WriteXxx方法推送数据；
    while (running_ && !input_->isEOF()) {
        if (input_->Handle()) { 
            int16_t* samples = input_->GetSamples();

            // 输出
            output_->WriteSamples(samples, sizeof(samples) / sizeof(int16_t)); 
        }
    }

    Log::Info(TAG, "handle end.");
}

void AudioPipe::Stop() {
    output_->Stop();
    running_ = false;
}

void AudioPipe::MetadataCallback(const char *tag, const char *type, const char *text) {
    Log::Info(tag, "Metadata: %s = %s", type, text);
}

void AudioPipe::StatusCallback(const char *tag, int code, const char* text) {
    Log::Info(tag, "Status(%d): %s", code, text);
}

#endif