/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_DISPLAY_SSD1306==1

#ifndef _SSD1306_DRIVER_H
#define _SSD1306_DRIVER_H

#include <driver/i2c_master.h>
#include "../../lcd_driver.h"

class SSD1306Driver : public LcdDriver {
public:
    SSD1306Driver(int width, int height, bool mirror_x, bool mirror_y, bool swap_xy) 
        : LcdDriver(width, height, mirror_x, mirror_y, swap_xy) { }

    virtual ~SSD1306Driver() {  }

    void InitI2c(i2c_master_bus_handle_t i2c_bus, uint8_t address) override;

};

#endif //_SSD1306_DRIVER_H

#endif //CONFIG_USE_DISPLAY_SSD1306