/**
 * 物联网自动浇花应用
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */

#ifndef _WATERING_APPLICATION_H
#define _WATERING_APPLICATION_H

#include "../application.h"

class WateringApplication : public Application {
public:
    WateringApplication();
    ~WateringApplication();
    
    void Init() override;
    void Start() override;

    bool OnPhysicalButtonEvent(const std::string& button_name, const ButtonAction action) override;
    bool OnSensorData(const std::string& sensor_name, int value) override;

    const std::string& GetAppName() const override { return "AutoWatering"; }
    const std::string& GetAppVersion() const override { return "1.0.0"; }

private:
    void DoWatering(uint8_t seconds);
    
};

#endif //_WATERING_APPLICATION_H
