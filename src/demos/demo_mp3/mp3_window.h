/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_MP3==1

#include <lvgl.h>
#include <string>
#include "src/framework/display/lvgl_style.h"
#include "src/framework/display/lvgl_window.h"

class Mp3Window : public LvglWindow {
public:
    Mp3Window();
    virtual ~Mp3Window();

    void SetupUI(lv_obj_t* container, const ThemeColors& theme, const DisplayFonts& fonts) override;
    void SetTheme(const ThemeColors& theme) override;
    
    void SetTitle(const std::string& title);
    void SetContent(const std::string& content);

protected:

    // 内容区
    lv_obj_t* content_ = nullptr;
    lv_obj_t* title_label_ = nullptr;
    lv_obj_t* content_label_ = nullptr;

    std::string title_;
    std::string content_;
    
};

#endif //APP_DEMO_MP3