/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "log.h"

#include "config.h"
#include <string>
#include <Arduino.h>

void Log::Info(const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[128] = { 0 };
    vsnprintf(buffer, 127, format, args);
    va_end(args);

#if CONFIG_USE_ESP_LOG==1
    log_i("%s %s", tag, buffer);
#else
    Serial.printf("%d [%s] %s\n", millis(), tag, buffer);
#endif
}

void Log::Warn(const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[128] = { 0 };
    vsnprintf(buffer, 127, format, args);
    va_end(args);

#if CONFIG_USE_ESP_LOG==1
    log_w("%s %s", tag, buffer);
#else
    Serial.printf("%d [%s] %s\n", millis(), tag, buffer);
#endif
}

void Log::Debug(const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[128] = { 0 };
    vsnprintf(buffer, 127, format, args);
    va_end(args);

#if CONFIG_USE_ESP_LOG==1
    log_d("%s %s", tag, buffer);
#else
    Serial.printf("%d [%s] %s\n", millis(), tag, buffer);
#endif
}

void Log::Error(const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[128] = { 0 };
    vsnprintf(buffer, 127, format, args);
    va_end(args);

#if CONFIG_USE_ESP_LOG==1
    log_e("%s %s", tag, buffer);
#else
    Serial.printf("%d [%s] %s\n", millis(), tag, buffer);
#endif
}