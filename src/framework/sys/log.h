/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _LOG_H
#define _LOG_H

class Log {
public:
    static void Info(const char* tag, const char* format, ...);
    static void Warn(const char* tag, const char* format, ...);
    static void Debug(const char* tag, const char* format, ...);
    static void Error(const char* tag, const char* format, ...);
    
};

#endif //_LOG_H