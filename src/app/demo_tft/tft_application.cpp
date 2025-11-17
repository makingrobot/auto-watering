/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_TFT==1

#include "tft_application.h"
#include "src/sys/log.h"
#include "src/boards/board.h"
#include "src/lang/lang_zh_cn.h"
#include "src/display/tft_display.h"

#define TAG "TftApplication"

void* create_application() {
    return new TftApplication();
}

TftApplication::TftApplication() : Application() { 

}

TftApplication::~TftApplication() {
   
}

void TftApplication::Init() {
    Application::Init();
    
    // do your init.
}

void TftApplication::Start() {
    Application::Start();

    Board& board = Board::GetInstance();
    board.GetLed()->Blink(-1, 1000);
    board.GetDisplay()->SetStatus("工作中");
    board.GetDisplay()->SetText("Hello world!");
}

#endif //APP_DEMO_TFT