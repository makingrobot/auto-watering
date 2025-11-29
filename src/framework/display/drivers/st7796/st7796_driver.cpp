/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if CONFIG_USE_DISPLAY_ST7796==1

#include "st7796_driver.h"

#include <esp_lcd_panel_vendor.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <driver/spi_common.h>
#include "src/framework/sys/log.h"
#include "src/libs/esp_lcd_st7796/esp_lcd_st7796.h"

#define TAG "ST7796Driver"

void ST7796Driver::InitSpi(spi_host_device_t spi_host, int spi_mode, 
        gpio_num_t cs_num, gpio_num_t dc_num, gpio_num_t rst_num, 
        gpio_num_t mosi_num, gpio_num_t miso_num, gpio_num_t clk_num, 
        lcd_rgb_element_order_t rgb_color_order, bool invert_color ) {

    Log::Info(TAG, "Init St7796 driver");

    drive_mode_ = kDriveModeSpi;
    
    Log::Info( TAG, "Init SPI for lcd driver ......" );
    spi_bus_config_t buscfg = {};
    buscfg.mosi_io_num = mosi_num;
    buscfg.miso_io_num = miso_num;
    buscfg.sclk_io_num = clk_num;
    buscfg.quadwp_io_num = GPIO_NUM_NC;
    buscfg.quadhd_io_num = GPIO_NUM_NC;
    buscfg.max_transfer_sz = width_ * height_ * sizeof(uint16_t); // for lcd.
    ESP_ERROR_CHECK(spi_bus_initialize(spi_host, &buscfg, SPI_DMA_CH_AUTO));

    Log::Debug(TAG, "Install panel IO");
    esp_lcd_panel_io_spi_config_t io_config = {};
    io_config.cs_gpio_num = cs_num;
    io_config.dc_gpio_num = dc_num;
    io_config.spi_mode = spi_mode;
    io_config.pclk_hz = 40 * 1000 * 1000;
    io_config.trans_queue_depth = 10;
    io_config.lcd_cmd_bits = 8;
    io_config.lcd_param_bits = 8;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(spi_host, &io_config, &panel_io_));

    Log::Debug(TAG, "Install LCD driver");
    esp_lcd_panel_dev_config_t panel_config = {};
    panel_config.reset_gpio_num = rst_num;
    panel_config.rgb_ele_order = rgb_color_order;
    panel_config.bits_per_pixel = 16;
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7796(panel_io_, &panel_config, &panel_));
    
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_, invert_color));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_, swap_xy_));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_, mirror_x_, mirror_y_));
    //ESP_ERROR_CHECK(esp_lcd_panel_set_gap(panel_, 49, 0));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_, true));

}

#endif //CONFIG_USE_DISPLAY_ST7796