/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_LCD==1

#include "lcd_application.h"
#include "src/sys/log.h"
#include "src/boards/board.h"
#include "src/boards/wifi_board.h"
#include "src/lang/lang_zh_cn.h"
#include "src/display/lvgl_display.h"
#include "src/display/lvgl_text_window.h"

#define TAG "LcdApplication"

void* create_application() {
    return new LcdApplication();
}

LcdApplication::LcdApplication() : Application() { 

    window_ = new LvglTextWindow();
    LvglDisplay* disp = static_cast<LvglDisplay*>(Board::GetInstance().GetDisplay());
    disp->SetWindow(window_);
}

LcdApplication::~LcdApplication() {
   
}

void LcdApplication::OnInit() {

    window_->SetText("Hello world!");
}

#endif //APP_DEMO_LCD