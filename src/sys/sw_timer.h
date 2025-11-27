#ifndef _SW_TIMER_H
#define _SW_TIMER_H

#include <functional>
#include <string>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include "src/sys/timer.h"

class SwTimer : public Timer {
public:
    SwTimer(const std::string& name);
    virtual ~SwTimer();

    virtual bool Start(uint32_t interval_ms, std::function<void()> callback_function, bool once=false);
    virtual bool Stop();

    std::function<void()> callback_function_;

private:
    
    TimerHandle_t timer_handle_ = NULL;
    std::string name_;
    
};

#endif //_SW_TIMER_H