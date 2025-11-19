/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <string>
#include <chrono>
#include <string>
#include "config.h"
#include "src/sys/log.h"

class Display {
public:
    Display() { };
    virtual ~Display() { };

    virtual void Init() = 0;

    virtual void ShowNotification(const char* notification, int duration_ms = 3000) { };
    virtual void ShowNotification(const std::string &notification, int duration_ms = 3000) { };
    virtual void SetStatus(const std::string& status) { };
    virtual void SetText(const std::string& text) { };
    virtual void UpdateStatusBar(bool update_all = false) { };
    virtual void Sleep() { };

    inline int width() const { return width_; }
    inline int height() const { return height_; }

protected:
    int width_ = 0;
    int height_ = 0;
    
    friend class DisplayLockGuard;
    virtual bool Lock(int timeout_ms = 0) = 0;
    virtual void Unlock() = 0;

private:

};

class DisplayLockGuard {
public:
    DisplayLockGuard(Display *display) : display_(display) {
        if (!display_->Lock(30000)) {
            Log::Error("Display", "Failed to lock display");
        }
    }
    ~DisplayLockGuard() {
        display_->Unlock();
    }

private:
    Display *display_;
};

class NoDisplay : public Display {
private:
    virtual bool Lock(int timeout_ms = 0) override {
        return true;
    }
    virtual void Unlock() override {}
    virtual void Init() override {}
};

#endif
