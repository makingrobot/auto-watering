/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_MP3==1

#include "mp3_window.h"
#include "src/sys/log.h"
#include "src/fonts/font_awesome_symbols.h"
#include "src/lang/lang_zh_cn.h"

#define TAG "Mp3Window"

Mp3Window::Mp3Window() {
            
    Log::Debug(TAG, "Mp3Window constructor.");

}

Mp3Window::~Mp3Window() {

    if (title_label_ != nullptr) {
        lv_obj_del(title_label_);
    }

    if (content_label_ != nullptr) {
        lv_obj_del(content_label_);
    }

    if (content_ != nullptr) {
        lv_obj_del(content_);
    }
   
}

void Mp3Window::SetupUI(lv_obj_t* container, const ThemeColors& theme, const DisplayFonts& fonts) {
    Log::Info(TAG, "SetupUI ......");

    /* Content */
    content_ = lv_obj_create(container);
    lv_obj_set_scrollbar_mode(content_, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_radius(content_, 0, 0);
    lv_obj_set_width(content_, LV_HOR_RES);
    lv_obj_set_flex_grow(content_, 1);
    lv_obj_set_style_pad_all(content_, 5, 0);
    lv_obj_set_style_bg_color(content_, theme.chat_background, 0);
    lv_obj_set_style_border_color(content_, theme.border, 0); // Border color for content

    lv_obj_set_flex_flow(content_, LV_FLEX_FLOW_COLUMN); // 垂直布局（从上到下）
    lv_obj_set_flex_align(content_, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_EVENLY); // 子对象居中对齐，等距分布

    title_label_ = lv_label_create(content_);
    lv_label_set_text(title_label_, "");
    lv_obj_set_width(title_label_, LV_HOR_RES * 0.9); // 限制宽度为屏幕宽度的 90%
    lv_label_set_long_mode(title_label_, LV_LABEL_LONG_WRAP); // 设置为自动换行模式
    lv_obj_set_style_text_align(title_label_, LV_TEXT_ALIGN_CENTER, 0); // 设置文本居中对齐
    lv_obj_set_style_text_color(title_label_, theme.text, 0);

    content_label_ = lv_label_create(content_);
    lv_label_set_text(content_label_, "");
    lv_obj_set_width(content_label_, LV_HOR_RES * 0.9); // 限制宽度为屏幕宽度的 90%
    lv_label_set_long_mode(content_label_, LV_LABEL_LONG_WRAP); // 设置为自动换行模式
    lv_obj_set_style_text_align(content_label_, LV_TEXT_ALIGN_CENTER, 0); // 设置文本居中对齐
    lv_obj_set_style_text_color(content_label_, theme.text, 0);

    Log::Info( TAG, "SetupUI completed." );
}

void Mp3Window::SetTheme(const ThemeColors& theme) {
    
    // Update content area colors
    if (content_ != nullptr) {
        lv_obj_set_style_bg_color(content_, theme.chat_background, 0);
        lv_obj_set_style_border_color(content_, theme.border, 0);
        
        lv_obj_set_style_text_color(title_label_, theme.text, 0);
        lv_obj_set_style_text_color(content_label_, theme.text, 0);
    }
    
}

void Mp3Window::SetTitle(const char* title) {
    if (title_label_ == nullptr) {
        Log::Warn(TAG, "title label not setup.");
        return;
    }
    lv_label_set_text(title_label_, title);
}

void Mp3Window::SetContent(const char* content) {
    if (content_label_ == nullptr) {
        Log::Warn(TAG, "content label not setup.");
        return;
    }
    lv_label_set_text(content_label_, content);
}

#endif //APP_DEMO_MP3
