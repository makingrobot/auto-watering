/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_AUDIO==1 && CONFIG_USE_LVGL==1

#include <lvgl.h>
#include <string>
#include "src/framework/display/lvgl_style.h"
#include "src/framework/display/lvgl_window.h"

class LvglAudioWindow : public LvglWindow {
public:
    LvglAudioWindow();
    virtual ~LvglAudioWindow();

    void SetupUI(lv_obj_t* container, const ThemeColors& theme, const DisplayFonts& fonts) override;
    void SetTheme(const ThemeColors& theme) override;
    
    void SetTitle(const std::string& title);
    void SetContent(const std::string& content);

protected:

    // 内容区
    lv_obj_t* lv_content_ = nullptr;
    lv_obj_t* title_label_ = nullptr;
    lv_obj_t* content_label_ = nullptr;

    std::string title_;
    std::string content_;
    
};

#endif 