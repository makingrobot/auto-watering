/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_AUDIO==1

#include <string>
#include "audio_application.h"
#include "audio_state.h"
#include "src/framework/sys/log.h"
#include "src/framework/board/wifi_board.h"
#include "src/framework/audio/audio_codec.h"
#include "src/framework/lang/lang_zh_cn.h"

#define TAG "AudioApplication"

// 开发板
#if BOARD_XPSTEM_S3_LCD_2_80 == 1
#include "src/boards/xpstem-s3-lcd-2.8/board_config.h"
#define I2S_BCLK    AUDIO_I2S_BCLK_PIN
#define I2S_LRC     AUDIO_I2S_WS_PIN
#define I2S_DOUT    AUDIO_I2S_DOUT_PIN
#define I2S_MCLK    AUDIO_I2S_MCLK_PIN
#elif BOARD_XPSTEM_S3_ELECTRONIC_SUIT == 1
#include "src/boards/xpstem-s3-electronic-suit/board_config.h"
#define I2S_BCLK    AUDIO_SPK_BCLK_PIN
#define I2S_LRC     AUDIO_SPK_LRC_PIN
#define I2S_DOUT    AUDIO_SPK_DAT_PIN   
#elif BOARD_XPSTEM_JC4827W543 == 1
#include "src/boards/xpstem-jc4827w543/board_config.h"
#define I2S_BCLK    SPECK_BCLK_PIN
#define I2S_LRC     SPECK_LRCLK_PIN
#define I2S_DOUT    SPECK_DIN_PIN   
#endif

// 图形化库
#if CONFIG_USE_LVGL==1
#include "src/framework/display/lvgl_display.h"
#elif CONFIG_USE_GFX_LIBRARY==1
#include "src/framework/display/gfx_display.h"
#include "src/framework/display/gfx_window.h"
#endif

typedef struct {
    const char* name;                                      
    const char* url;                                       
} mp3_info_t;

static const mp3_info_t mp3_list[] {
    {"鸡你太美", "http://music.163.com/song/media/outer/url?id=1980818176.mp3"},
};

void* create_application() {
    return new AudioApplication();
}

AudioApplication::AudioApplication() : Application() { 
#if CONFIG_USE_LVGL==1
    window_ = new LvglAudioWindow();
    LvglDisplay* disp = static_cast<LvglDisplay*>(Board::GetInstance().GetDisplay());
    disp->SetWindow(window_);
#endif
}

AudioApplication::~AudioApplication() {
   
    if (audio_task_handle_ != nullptr) {
        vTaskDelete(audio_task_handle_);
    }

}

void AudioApplication::OnInit() {
    
    audio_ = new Audio(I2S_NUM_1);
    audio_->setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio_->setVolume(8);  //0-21
    Audio::audio_info_callback = [this](Audio::msg_t m){ this->AudioInfo(m); };

    Board& board = Board::GetInstance();
    // 启动音频编解码
    AudioCodec* codec = board.GetAudioCodec();
    codec->Start();
    codec->SetOutputVolume(50);

    mp3_total_ = sizeof(mp3_list) / sizeof(mp3_info_t);

    // 创建音频后台任务
    xTaskCreate([](void* pvParam){
        Audio* _audio = static_cast<Audio*>(pvParam);
        while(1) {
            _audio->loop();
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }, "AUDIO_TASK", 8192, audio_, 1, &audio_task_handle_);

    mp3_info_t info = mp3_list[mp3_index_];
    Log::Info(TAG, "play %s", info.url);
    audio_->connecttohost(info.url);

    std::string str = std::to_string(mp3_index_+1);
    str +=  " / ";
    str += std::to_string(mp3_total_);

#if CONFIG_USE_LVGL==1
    window_->SetTitle(info.name);
    window_->SetContent(str.c_str());
#else CONFIG_USE_GFX_LIBRARY==1
    GfxWindow* window = ((GfxDisplay*)board.GetDisplay())->GetWindow();
    window->SetText(info.name);
#endif

    SetDeviceState(kDeviceStatePlaying);
}

void AudioApplication::AudioInfo(Audio::msg_t m) {

    Log::Debug(TAG, "%s: %s", m.s, m.msg);
    switch (m.e) {
        case Audio::evt_eof:
            AudioPlayEnd();
            break;
        case Audio::evt_id3data:
            break;
        case Audio::evt_image:
            break;
        case Audio::evt_lyrics:
            break;
    } 

}

void AudioApplication::AudioPlayEnd() {

    mp3_index_ ++;
    if (mp3_index_ == mp3_total_) {
        mp3_index_ = 0;
    }

    mp3_info_t info = mp3_list[mp3_index_];
    Log::Info(TAG, "play %s", info.url);
    audio_->connecttohost(info.url);

    std::string str = std::to_string(mp3_index_+1);
    str +=  " / ";
    str += std::to_string(mp3_total_);

#if CONFIG_USE_LVGL==1
    window_->SetTitle(info.name);
    window_->SetContent(str.c_str());
#else CONFIG_USE_GFX_LIBRARY==1
    Board& board = Board::GetInstance();
    GfxWindow* window = ((GfxDisplay*)board.GetDisplay())->GetWindow();
    window->SetText(info.name);
#endif
}

void AudioApplication::SetDeviceState(const DeviceState* state) {
 
    Application::SetDeviceState(state);

    auto& board = Board::GetInstance();
    auto display = board.GetDisplay();

    if (state == kDeviceStatePlaying) {
        display->SetStatus(Lang::Strings::PLAYING);
    }

}

#endif 