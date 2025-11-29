/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_GFX_LIBRARY == 1 && CONFIG_USE_LVGL == 1

#include "gfx_lvgl_driver.h"
#include "../sys/log.h"

#define TAG "GfxLvglDriver"

/* LVGL calls it when a rendered image needs to copied to the display*/
void GfxLvglDriver::FlushCallback(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
#ifndef DIRECT_RENDER_MODE
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);

    gfx_->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);
#endif // #ifndef DIRECT_RENDER_MODE

    /*Call it to tell LVGL you are ready*/
    lv_disp_flush_ready(disp);
}

void GfxLvglDriver::Init() {
    // Init Display
    if (!gfx_->begin()) {
        Log::Error(TAG, "gfx->begin() failed!");
        return;
    }
    gfx_->fillScreen(RGB565_BLACK);

    lv_init();

    int screen_width = gfx_->width();
    int screen_height = gfx_->height();
    int buf_size = 0;

#ifdef DIRECT_RENDER_MODE
    buf_size = screen_width * screen_height;
#else
    buf_size = screen_width * 40;
#endif

#if defined(DIRECT_RENDER_MODE) && (defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL))
    disp_buf_ = (lv_color_t *)gfx->getFramebuffer();
#else  // !(defined(DIRECT_RENDER_MODE) && (defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL)))
    disp_buf_ = (lv_color_t *)heap_caps_malloc(buf_size * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if (!disp_buf_) {
        // remove MALLOC_CAP_INTERNAL flag try again
        disp_buf_ = (lv_color_t *)heap_caps_malloc(buf_size * 2, MALLOC_CAP_8BIT);
    }
#endif // !(defined(DIRECT_RENDER_MODE) && (defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL)))

    if (!disp_buf_) {
        Log::Error(TAG, "LVGL disp_draw_buf allocate failed!");
        return;
    }
   
    display_ = lv_display_create(screen_width, screen_height);
    
    auto bind_func = std::bind(&GfxDriver::FlushCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    std::function<void(lv_display_t*, const lv_area_t*, uint8_t*)> flush_func = bind_func;  //擦除类型
    lv_display_set_flush_cb(display_, flush_func.target<void(lv_display_t*, const lv_area_t*, uint8_t*)>());

#ifdef DIRECT_RENDER_MODE
    lv_display_set_buffers(display_, disp_buf_, NULL, buf_size * 2, LV_DISPLAY_RENDER_MODE_DIRECT);
#else
    lv_display_set_buffers(display_, disp_buf_, NULL, buf_size * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif

    /*Initialize the (dummy) input device driver*/
//    lv_indev_t *indev = lv_indev_create();
//    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
//    lv_indev_set_read_cb(indev, my_touchpad_read);

}

#endif //CONFIG_USE_GFX_LIBRARY