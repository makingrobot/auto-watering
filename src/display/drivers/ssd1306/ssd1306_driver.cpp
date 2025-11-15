/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_DISPLAY_SSD1306==1

#include "ssd1306_driver.h"

#include <esp_lcd_panel_vendor.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>

#include "src/sys/log.h"

#define TAG "SSD1306Driver"

void SSD1306Driver::InitI2c(i2c_master_bus_handle_t i2c_bus, uint8_t address) {

    drive_mode_ = kDriveModeI2c;
    
    // SSD1306 config
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = address,
        .on_color_trans_done = nullptr,
        .user_ctx = nullptr,
        .control_phase_bytes = 1,
        .dc_bit_offset = 6,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .flags = {
            .dc_low_on_data = 0,
            .disable_control_phase = 0,
        },
        .scl_speed_hz = 400 * 1000,
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c_v2(i2c_bus, &io_config, &panel_io_));

    Log::Info(TAG, "Install SSD1306 driver");
    esp_lcd_panel_dev_config_t panel_config = {};
    panel_config.reset_gpio_num = -1;
    panel_config.bits_per_pixel = 1;

    esp_lcd_panel_ssd1306_config_t ssd1306_config = {
        .height = static_cast<uint8_t>(height_),
    };
    panel_config.vendor_config = &ssd1306_config;

    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(panel_io_, &panel_config, &panel_));
    Log::Info(TAG, "SSD1306 driver installed");

    // Reset the display
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_));
    if (esp_lcd_panel_init(panel_) != ESP_OK) {
        return;
    }

    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_, swap_xy_));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_, mirror_x_, mirror_y_));
    
    // Set the display to on
    LogInfo(TAG, "Turning display on");
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_, true));

}

#endif //CONFIG_USE_DISPLAY_SSD1306