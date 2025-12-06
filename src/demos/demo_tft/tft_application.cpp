/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_TFT==1

#include "tft_application.h"
#include "src/framework/sys/log.h"
#include "src/framework/board/board.h"
#include "src/framework/lang/lang_zh_cn.h"
#include "src/framework/display/tft_display.h"

#define TAG "TftApplication"

void* create_application() {
    return new TftApplication();
}

TftApplication::TftApplication() : Application() { 

    window_ = new TestWindow();
    TftDisplay* disp = static_cast<TftDisplay*>(Board::GetInstance().GetDisplay());
    disp->SetWindow(window_);
}

void TftApplication::OnInit() {
    
    // do your init.
    // Board& board = Board::GetInstance();
    // board.GetLed()->Blink(-1, 1000);
    // board.GetDisplay()->SetStatus("Work");
    // board.GetDisplay()->SetText("Hello world!");

    TftDisplay* disp = static_cast<TftDisplay*>(Board::GetInstance().GetDisplay());
    disp->Rotate(3);
    window_->Start();
}

#endif //APP_DEMO_TFT