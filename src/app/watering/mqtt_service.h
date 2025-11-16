#ifndef _MQTT_SERVICE_H
#define _MQTT_SERVICE_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <functional>
#include <map>
#include <string>

class MqttService {
public:
    MqttService();
    bool Connect(const std::string& broker_server, const std::string& username, const std::string& password);
    void PublishData(const std::string& topic, const std::string& payload);
    void SubscribeTopic(const std::string& topic, std::function<void(const std::string&)> callback);
    void Disconnect();

private:
    void OnMessage(char *mqtt_topic, byte *payload, unsigned int length);

    WiFiClient wifi_client_;
    PubSubClient *mqtt_client_;
    TaskHandle_t mqtt_loop_handle_;
    std::map<std::string, std::function<void(const std::string&)>> subscribe_callback_;

};

#endif //_MQTT_SERVICE_H