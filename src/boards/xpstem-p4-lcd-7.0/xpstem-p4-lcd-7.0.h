#include "config.h"
#if BOARD_XPSTEM_P4_LCD_7_0 == 1

#ifndef _XPSTEM_P4_LCD_7_0_H
#define _XPSTEM_P4_LCD_7_0_H

#include <esp_sleep.h>
#include <driver/i2c_master.h>
#include <driver/spi_common.h>
#include <driver/gpio.h>

#include "src/framework/sys/log.h"
#include "src/framework/board/wifi_board.h"

#include "board_config.h"

class XPSTEM_P4_LCD_7_0 : public WifiBoard {
private:

public:
    XPSTEM_P4_LCD_7_0();


};

#endif //_XPSTEM_P4_LCD_7_0_H

#endif //BOARD_XPSTEM_P4_LCD_7_0