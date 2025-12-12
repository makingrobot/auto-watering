#include "config.h"
#if CONFIG_USE_SW_TIMER==1

#include "sw_timer.h"
#include "../log.h"

#define TAG "SwTimer"

SwTimer::SwTimer(const std::string& name) : name_(name) {
}

SwTimer::~SwTimer() {
    Stop();
}

void _timerCallback(TimerHandle_t param) {
    SwTimer *_this = (SwTimer*)pvTimerGetTimerID(param);
    if (_this->callback_function_) {
        _this->callback_function_();
    }
}

bool SwTimer::Start(uint32_t interval_ms, std::function<void()> callback_function, bool once) {
    Stop();

    callback_function_ = std::move(callback_function);
    
    timer_handle_ = xTimerCreate(name_.c_str(), pdMS_TO_TICKS(interval_ms), once ? pdFALSE : pdTRUE, this, _timerCallback);
    if (timer_handle_==NULL) {
        Log::Info(TAG, "%s timer create failure.", name_.c_str());
        return false; 
    }

    bool success = (xTimerStart(timer_handle_, 0) == pdPASS);
    Log::Info(TAG, success ? "%s timer started." : "%s timer start failure.", name_.c_str());
    return success;
}

bool SwTimer::Stop() {
    if (timer_handle_ != NULL) {
        bool success = xTimerDelete(timer_handle_, 0);
        timer_handle_ = NULL;
        Log::Info(TAG, "%s timer stopped.", name_.c_str());
        return success;
    }
    return false;
}

#endif