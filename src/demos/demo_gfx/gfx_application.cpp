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
#include "src/framework/display/gfx_display.h"
#include "benchmark_window.h"

#define TAG "GfxApplication"

void* create_application() {
    return new GfxApplication();
}

GfxApplication::GfxApplication() : Application() { 

    // window_ = new BenchmarkWindow();

    // GfxDisplay* display = (GfxDisplay*)Board::GetInstance().GetDisplay();
    // display->SetWindow(window_);
}

GfxApplication::~GfxApplication() {
   
}

void GfxApplication::OnInit() {
    Log::Info(TAG, "Initial.");

    // window_->Start();
    
    Board& board = Board::GetInstance();
    board.GetDisplay()->SetStatus("Work");
    board.GetDisplay()->SetText("Hello world!");
    
}

#endif //APP_DEMO_TFT