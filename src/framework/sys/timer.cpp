#include "timer.h"

#include "config.h"
#include "timer/sw_timer.h"
#include "timer/hw_timer.h"

Timer* TimerFactory::CreateTimer(const std::string& name) {
#if CONFIG_USE_SW_TIMER==1
    return new SwTimer(name);
#elif CONFIG_USE_HW_TIMER==1
    return new HwTimer(name);
#endif
}