/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_LVGL==1

#include "lvgl_application.h"
#include "src/framework/sys/log.h"
#include "src/framework/board/board.h"
#include "src/framework/board/wifi_board.h"
#include "src/framework/lang/lang_zh_cn.h"
#include "src/framework/display/lvgl_display.h"

#define TAG "LvglApplication"

void* create_application() {
    return new LvglApplication();
}

void LvglApplication::OnInit() {

    LvglDisplay* disp = static_cast<LvglDisplay*>(Board::GetInstance().GetDisplay());
    disp->GetWindow()->SetText("Hello world2!");
}

#endif //APP_DEMO_LVGL