/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#include "config.h"
#if APP_DEMO_MP3==1

#ifndef _MP_STATE_H
#define _MP_STATE_H

#include "src/framework/app/device_state.h"

static const DeviceState* kDeviceStatePlaying            = DeviceState::Create(101, "playing");

#endif //_MY_STATE_H

#endif //APP_DEMO_MP3