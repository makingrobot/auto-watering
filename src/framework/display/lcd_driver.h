/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_LCD_PANEL == 1

#ifndef _LCD_DRIVER_H
#define _LCD_DRIVER_H

#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <driver/gpio.h>
#include "disp_driver.h"
#include <lvgl.h>

class LcdDriver : public DispDriver {
public:
    LcdDriver(int width, int height, 
            bool mirror_x, bool mirror_y, bool swap_xy, int offset_x = 0, int offset_y = 0) 
        : DispDriver(width, height), 
            mirror_x_(mirror_x), mirror_y_(mirror_y), swap_xy_(swap_xy), offset_x_(offset_x), offset_y_(offset_y) { }
    
    virtual ~LcdDriver() { 
        if (panel_ != nullptr) {
            esp_lcd_panel_del(panel_);
        }
        if (panel_io_ != nullptr) {
            esp_lcd_panel_io_del(panel_io_);
        }
        if (display_ != nullptr) {
            lv_display_delete(display_);
        }
    }

    /**
     * 初始为SPI模式
     * 
     * spi_host: SPI主机
     * spi_mode: SPI模式
     * cs_pin: CS引脚
     * dc_pin: DC引脚
     * rst_pin: RESET引脚
     * rgb_color_order: RGB色彩顺式
     * invert_color: 颜色反转
     */
    virtual void InitSpi(spi_host_device_t spi_host, int spi_mode, gpio_num_t cs_num, gpio_num_t dc_num, gpio_num_t rst_num, 
        gpio_num_t mosi_num, gpio_num_t miso_num, gpio_num_t clk_num, lcd_rgb_element_order_t rgb_color_order, bool invert_color) { }

    void Init() override;

protected:

    esp_lcd_panel_io_handle_t panel_io_ = nullptr;
    esp_lcd_panel_handle_t panel_ = nullptr;
    lv_display_t* display_ = nullptr;

    int offset_x_;
    int offset_y_;
    bool mirror_x_;
    bool mirror_y_;
    bool swap_xy_;

private:

};

#endif //_LCD_DRIVER_H

#endif //CONFIG_USE_LCD_PANEL