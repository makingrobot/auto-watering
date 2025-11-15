/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_MP3==1

#include <string>
#include "mp3_application.h"
#include "mp3_state.h"
#include "src/sys/log.h"
#include "src/boards/wifi_board.h"
#include "src/display/lvgl_display.h"
#include "src/audio/audio_codec.h"
#include "src/lang/lang_zh_cn.h"

#define TAG "Mp3Application"

#if BOARD_XPSTEM_S3_LCD_2_80 == 1
#include "src/boards/xpstem-s3-lcd-2.8/board_config.h"
#define I2S_BCLK    AUDIO_I2S_BCLK_PIN
#define I2S_LRC     AUDIO_I2S_WS_PIN
#define I2S_DOUT    AUDIO_I2S_DOUT_PIN
#define I2S_MCLK    AUDIO_I2S_MCLK_PIN
#endif

#if BOARD_XPSTEM_S3_ELECTRONIC_SUIT == 1
#include "src/boards/xpstem-s3-electronic-suit/board_config.h"
#define I2S_BCLK    AUDIO_SPK_BCLK_PIN
#define I2S_LRC     AUDIO_SPK_LRC_PIN
#define I2S_DOUT    AUDIO_SPK_DAT_PIN   
#endif

typedef struct {
    const char* name;                                      
    const char* url;                                       
} mp3_info_t;

static const mp3_info_t mp3_list[] {
    {"鸡你太美", "http://music.163.com/song/media/outer/url?id=1980818176.mp3"},
};

void* create_application() {
    return new Mp3Application();
}

Mp3Application::Mp3Application() : Application() { 

    window_ = new Mp3Window();
    LvglDisplay* disp = static_cast<LvglDisplay*>(Board::GetInstance().GetDisplay());
    disp->SetWindow(window_);

}

Mp3Application::~Mp3Application() {
   
    if (audio_task_handle_ != nullptr) {
        vTaskDelete(audio_task_handle_);
    }

}

void Mp3Application::Init() {
    Application::Init();
    
    Audio::audio_info_callback = [this](Audio::msg_t m){ this->AudioInfo(m); };
}


void AudioTask(void *param) {

    Audio* audio = static_cast<Audio*>(param);
    while(1) {
        audio->loop();
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}


void Mp3Application::Start() {
    Application::Start();

    // 启动Wifi
    WifiBoard* board = static_cast<WifiBoard*>(&Board::GetInstance());
    board->StartNetwork();

    // 设置音频编解码
    AudioCodec* codec = board->GetAudioCodec();
    codec->Start();
    codec->SetOutputVolume(50);

    audio_ = new Audio(I2S_NUM_1);

    audio_->setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio_->setVolume(8);  //0-21

    mp3_total_ = sizeof(mp3_list) / sizeof(mp3_info_t);

    // 创建音频后台任务
    xTaskCreate(AudioTask, "AUDIO_TASK", 8192, audio_, 1, &audio_task_handle_);

    mp3_info_t info = mp3_list[mp3_index_];
    Log::Info(TAG, "play %s", info.url);
    audio_->connecttohost(info.url);

    window_->SetTitle(info.name);

    std::string str = std::to_string(mp3_index_+1);
    str +=  " / ";
    str += std::to_string(mp3_total_);
    window_->SetContent(str.c_str());

    SetDeviceState(kDeviceStatePlaying);
}

void Mp3Application::AudioInfo(Audio::msg_t m) {

    Log::Debug(TAG, "%s: %s", m.s, m.msg);
    if (strcmp(m.s, "eof")==0) {
        AudioPlayEnd();
    }

}

void Mp3Application::AudioPlayEnd() {

    mp3_index_ ++;
    if (mp3_index_ == mp3_total_) {
        mp3_index_ = 0;
    }

    mp3_info_t info = mp3_list[mp3_index_];
    Log::Info(TAG, "play %s", info.url);
    audio_->connecttohost(info.url);

    window_->SetTitle(info.name);
    
    std::string str = std::to_string(mp3_index_+1);
    str +=  " / ";
    str += std::to_string(mp3_total_);
    window_->SetContent(str.c_str());
}

void Mp3Application::SetDeviceState(const DeviceState* state) {
 
    Application::SetDeviceState(state);

    auto& board = Board::GetInstance();
    auto display = board.GetDisplay();

    if (state == kDeviceStatePlaying) {
        display->SetStatus(Lang::Strings::PLAYING);
    }

}

#endif //APP_DEMO_MP3