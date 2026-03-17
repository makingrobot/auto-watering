#include "config.h"
#if BOARD_XPSTEM_P4_LCD_7_0 == 1

#include "xpstem-p4-lcd-7.0.h"

#define TAG "XPSTEM_P4_LCD_7_0"

void* create_board() { 
    return new XPSTEM_P4_LCD_7_0();
}

XPSTEM_P4_LCD_7_0::XPSTEM_P4_LCD_7_0() : WifiBoard() {

    Log::Info(TAG, "===== Create Board ...... =====");

   

    Log::Info( TAG, "===== Board config completed. =====");
}


#endif //BOARD_XPSTEM_P4_LCD_7_0