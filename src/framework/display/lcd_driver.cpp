/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_LCD_PANEL == 1

#include <vector>
#include <esp_err.h>

#include "lcd_driver.h"
#include "../sys/log.h"
#include "src/libs/esp_lvgl_port/esp_lvgl_port.h"

#define TAG "LcdDriver"

void LcdDriver::Init() {
    Log::Info(TAG, "drive mode: %d", drive_mode_);
    
    // draw white
    if (drive_mode_ == kDriveModeSpi ||
            drive_mode_ == kDriveModeRgb) {
        std::vector<uint16_t> buffer(width_, 0xFFFF);
        for (int y = 0; y < height_; y++) {
            esp_lcd_panel_draw_bitmap(panel_, 0, y, width_, y + 1, buffer.data());
        }
    }
        
    // Set the display to on
    if (drive_mode_ == kDriveModeSpi ||
            drive_mode_ == kDriveModeMipi) {
        Log::Info(TAG, "Turning display on");
        ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_, true));
    }

    Log::Info(TAG, "Initialize LVGL library" );
    lv_init();

    if (drive_mode_ == kDriveModeSpi) {
        Log::Info(TAG, "Initialize LVGL port");
        lvgl_port_cfg_t port_cfg = ESP_LVGL_PORT_INIT_CONFIG();
        port_cfg.task_priority = 1;
        port_cfg.timer_period_ms = 50;
        lvgl_port_init(&port_cfg);

        Log::Info(TAG, "Adding LCD display");
        const lvgl_port_display_cfg_t display_cfg = {
            .io_handle = panel_io_,
            .panel_handle = panel_,
            .control_handle = nullptr,
            .buffer_size = static_cast<uint32_t>(width_ * 20),
            .double_buffer = false,
            .trans_size = 0,
            .hres = static_cast<uint32_t>(width_),
            .vres = static_cast<uint32_t>(height_),
            .monochrome = false,
            .rotation = {
                .swap_xy = swap_xy_,
                .mirror_x = mirror_x_,
                .mirror_y = mirror_y_,
            },
            .color_format = LV_COLOR_FORMAT_RGB565,
            .flags = {
                .buff_dma = 1,
                .buff_spiram = 0,
                .sw_rotate = 0,
                .swap_bytes = 1,
                .full_refresh = 0,
                .direct_mode = 0,
            },
        };

        display_ = lvgl_port_add_disp(&display_cfg);

    } else if (drive_mode_ == kDriveModeRgb) {
        Log::Info( TAG, "Initialize LVGL port" );
        lvgl_port_cfg_t port_cfg = ESP_LVGL_PORT_INIT_CONFIG();
        port_cfg.task_priority = 1;
        port_cfg.timer_period_ms = 50;
        lvgl_port_init(&port_cfg);

        Log::Info( TAG, "Adding LCD driver" );
        const lvgl_port_display_cfg_t display_cfg = {
            .io_handle = panel_io_,
            .panel_handle = panel_,
            .buffer_size = static_cast<uint32_t>(width_ * 20),
            .double_buffer = true,
            .hres = static_cast<uint32_t>(width_),
            .vres = static_cast<uint32_t>(height_),
            .rotation = {
                .swap_xy = swap_xy_,
                .mirror_x = mirror_x_,
                .mirror_y = mirror_y_,
            },
            .flags = {
                .buff_dma = 1,
                .swap_bytes = 0,
                .full_refresh = 1,
                .direct_mode = 1,
            },
        };

        const lvgl_port_display_rgb_cfg_t rgb_cfg = {
            .flags = {
                .bb_mode = true,
                .avoid_tearing = true,
            }
        };
        
        display_ = lvgl_port_add_disp_rgb(&display_cfg, &rgb_cfg);

    } else if (drive_mode_ == kDriveModeMipi) {

        Log::Info( TAG, "Initialize LVGL port");
        lvgl_port_cfg_t port_cfg = ESP_LVGL_PORT_INIT_CONFIG();
        lvgl_port_init(&port_cfg);

        Log::Info( TAG, "Adding LCD display");
        const lvgl_port_display_cfg_t disp_cfg = {
            .io_handle = panel_io_,
            .panel_handle = panel_,
            .control_handle = nullptr,
            .buffer_size = static_cast<uint32_t>(width_ * 50),
            .double_buffer = false,
            .hres = static_cast<uint32_t>(width_),
            .vres = static_cast<uint32_t>(height_),
            .monochrome = false,
            /* Rotation values must be same as used in esp_lcd for initial settings of the screen */
            .rotation = {
                .swap_xy = swap_xy_,
                .mirror_x = mirror_x_,
                .mirror_y = mirror_y_,
            },
            .flags = {
                .buff_dma = true,
                .buff_spiram =false,
                .sw_rotate = false,
            },
        };

        const lvgl_port_display_dsi_cfg_t dpi_cfg = {
            .flags = {
                .avoid_tearing = false,
            }
        };
        display_ = lvgl_port_add_disp_dsi(&disp_cfg, &dpi_cfg);

    }

    if (display_ == nullptr) {
        Log::Warn( TAG, "Failed to add RGB display");
        return;
    }

    if (offset_x_ != 0 || offset_y_ != 0) {
        lv_display_set_offset(display_, offset_x_, offset_y_);
    }
}

#endif //CONFIG_USE_LCD_PANEL