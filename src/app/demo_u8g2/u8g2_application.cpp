/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_U8G2==1

#include "u8g2_application.h"
#include "src/sys/log.h"
#include "src/boards/board.h"
#include "src/lang/lang_zh_cn.h"
#include "src/display/u8g2_display.h"

#define TAG "U8g2Application"

void* create_application() {
    return new U8g2Application();
}

U8g2Application::U8g2Application() : Application() { 

}

U8g2Application::~U8g2Application() {
   
}

void U8g2Application::Init() {
    Application::Init();
    
    // do your init.
}

void U8g2Application::Start() {
    Application::Start();

    Board& board = Board::GetInstance();
    board.GetLed()->Blink(-1, 1000);
    board.GetDisplay()->SetStatus("工作中");
    board.GetDisplay()->SetText("你好,世界!");
}

#endif //APP_DEMO_U8G2