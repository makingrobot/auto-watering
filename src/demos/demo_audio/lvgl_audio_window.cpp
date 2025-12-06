/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_AUDIO==1 && CONFIG_USE_LVGL==1

#include "lvgl_audio_window.h"
#include "src/framework/sys/log.h"
#include "src/framework/fonts/font_awesome_symbols.h"
#include "src/framework/lang/lang_zh_cn.h"

#define TAG "AudioWindow"

LvglAudioWindow::LvglAudioWindow() {
            
    Log::Debug(TAG, "LvglAudioWindow constructor.");

}

LvglAudioWindow::~LvglAudioWindow() {

    if (title_label_ != nullptr) {
        lv_obj_del(title_label_);
    }

    if (content_label_ != nullptr) {
        lv_obj_del(content_label_);
    }

    if (lv_content_ != nullptr) {
        lv_obj_del(lv_content_);
    }
   
}

void LvglAudioWindow::SetupUI(lv_obj_t* container, const ThemeColors& theme, const DisplayFonts& fonts) {
    Log::Info(TAG, "SetupUI ......");

    /* Content */
    lv_content_ = lv_obj_create(container);
    lv_obj_set_scrollbar_mode(lv_content_, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_radius(lv_content_, 0, 0);
    lv_obj_set_width(lv_content_, LV_HOR_RES);
    lv_obj_set_flex_grow(lv_content_, 1);
    lv_obj_set_style_pad_all(lv_content_, 5, 0);
    lv_obj_set_style_bg_color(lv_content_, theme.chat_background, 0);
    lv_obj_set_style_border_color(lv_content_, theme.border, 0); // Border color for content

    lv_obj_set_flex_flow(lv_content_, LV_FLEX_FLOW_COLUMN); // 垂直布局（从上到下）
    lv_obj_set_flex_align(lv_content_, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_EVENLY); // 子对象居中对齐，等距分布

    title_label_ = lv_label_create(lv_content_);
    lv_label_set_text(title_label_, "");
    lv_obj_set_width(title_label_, LV_HOR_RES * 0.9); // 限制宽度为屏幕宽度的 90%
    lv_label_set_long_mode(title_label_, LV_LABEL_LONG_WRAP); // 设置为自动换行模式
    lv_obj_set_style_text_align(title_label_, LV_TEXT_ALIGN_CENTER, 0); // 设置文本居中对齐
    lv_obj_set_style_text_color(title_label_, theme.text, 0);

    content_label_ = lv_label_create(lv_content_);
    lv_label_set_text(content_label_, "");
    lv_obj_set_width(content_label_, LV_HOR_RES * 0.9); // 限制宽度为屏幕宽度的 90%
    lv_label_set_long_mode(content_label_, LV_LABEL_LONG_WRAP); // 设置为自动换行模式
    lv_obj_set_style_text_align(content_label_, LV_TEXT_ALIGN_CENTER, 0); // 设置文本居中对齐
    lv_obj_set_style_text_color(content_label_, theme.text, 0);

    Log::Info( TAG, "SetupUI completed." );
}

void LvglAudioWindow::SetTheme(const ThemeColors& theme) {
    
    // Update content area colors
    if (lv_content_ != nullptr) {
        lv_obj_set_style_bg_color(lv_content_, theme.chat_background, 0);
        lv_obj_set_style_border_color(lv_content_, theme.border, 0);
        
        lv_obj_set_style_text_color(title_label_, theme.text, 0);
        lv_obj_set_style_text_color(content_label_, theme.text, 0);
    }
    
}

void LvglAudioWindow::SetTitle(const std::string& title) {
    if (title_label_ == nullptr) {
        Log::Warn(TAG, "title label not setup.");
        return;
    }
    title_ = title;
    lv_label_set_text(title_label_, title_.c_str());
}

void LvglAudioWindow::SetContent(const std::string& content) {
    if (content_label_ == nullptr) {
        Log::Warn(TAG, "content label not setup.");
        return;
    }
    content_ = content;
    lv_label_set_text(content_label_, content_.c_str());
}

#endif 
