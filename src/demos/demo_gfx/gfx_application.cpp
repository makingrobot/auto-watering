/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_GFX==1

#include "gfx_application.h"
#include "src/framework/sys/log.h"
#include "src/framework/board/board.h"

#if CONFIG_USE_LVGL==1
#include "src/framework/display/lvgl_display.h"
#include "src/framework/display/gfx_lvgl_driver.h"
#else
#include "src/framework/display/gfx_display.h"
#endif

#define TAG "GfxApplication"

void* create_application() {
    return new GfxApplication();
}

GfxApplication::GfxApplication() : Application() { 

#if CONFIG_USE_LVGL==1
    // lvgl
#else
    // window_ = new BenchmarkWindow();

    // GfxDisplay* display = (GfxDisplay*)Board::GetInstance().GetDisplay();
    // display->SetWindow(window_);
#endif
}

void GfxApplication::OnInit() {
    Log::Info(TAG, "Initial.");

#if CONFIG_USE_LVGL==1

    xTaskCreate([](void* pvParam){
        while(1) {
            GfxLvglDriver* driver = (GfxLvglDriver*)Board::GetInstance().GetDispDriver();
            driver->TaskHandler();
            vTaskDelay(pdMS_TO_TICKS(5));
        }
    }, "LVGL_UI_TASK", 8192, NULL, 1, &lvgl_task_handle_);
    
    Board& board = Board::GetInstance();
    board.GetDisplay()->SetStatus("Work");
    board.GetDisplay()->SetText("Hello world!");
#else
    Board& board = Board::GetInstance();
    board.GetDisplay()->SetStatus("Work");
    board.GetDisplay()->SetText("Hello world!");
    // GfxDisplay* display = (GfxDisplay*)Board::GetInstance().GetDisplay();
    // display->Rotate(1);

    // window_->Start();
#endif
    
}

#endif //APP_DEMO_TFT