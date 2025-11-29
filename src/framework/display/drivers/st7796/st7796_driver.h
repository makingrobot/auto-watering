/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_DISPLAY_ST7796==1

#ifndef _ST7796_DRIVER_H
#define _ST7796_DRIVER_H

#include "../../lcd_driver.h"

class ST7796Driver : public LcdDriver {
public:
    ST7796Driver(int width, int height, bool mirror_x, bool mirror_y, bool swap_xy, int offset_x=0, int offset_y=0) 
        : LcdDriver(width, height, mirror_x, mirror_y, swap_xy, offset_x, offset_y) { }

    virtual ~ST7796Driver() {  }

    void InitSpi(spi_host_device_t spi_host, int spi_mode, gpio_num_t cs_num, gpio_num_t dc_num, gpio_num_t rst_num, 
        gpio_num_t mosi_num, gpio_num_t miso_num, gpio_num_t clk_num, lcd_rgb_element_order_t rgb_color_order, bool invert_color) override;

};

#endif //_ST7796_DRIVER_H

#endif //CONFIG_USE_DISPLAY_ST7796