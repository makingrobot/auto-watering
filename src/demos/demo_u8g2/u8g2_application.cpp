/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_U8G2==1

#include "u8g2_application.h"
#include "src/framework/sys/log.h"
#include "src/framework/board/board.h"
#include "src/framework/lang/lang_zh_cn.h"
#include "src/framework/display/u8g2_display.h"
#include "test_window.h"

#define TAG "U8g2Application"

void* create_application() {
    return new U8g2Application();
}

U8g2Application::U8g2Application() : Application() { 

    TestWindow* window = new TestWindow();
    U8g2Display* disp = (U8g2Display*)Board::GetInstance().GetDisplay();
    disp->SetWindow(window);

}

U8g2Application::~U8g2Application() {
   
}

void U8g2Application::OnInit() {
    // do your init.
    
    Board& board = Board::GetInstance();
    // board.GetLed()->Blink(-1, 1000);
    // board.GetDisplay()->SetStatus("工作中");
    // board.GetDisplay()->SetText("你好,世界!");

    U8g2Display* disp = (U8g2Display*)board.GetDisplay();
    TestWindow* window = (TestWindow*)disp->GetWindow();
    window->Start();

}

#endif //APP_DEMO_U8G2