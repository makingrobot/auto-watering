
#include "mqtt_service.h"
#include "src/sys/log.h"
#include "src/boards/board.h"
#include "watering_application.h"

#define TAG "MqttService"

void mqttLoopTask(void *pvParam) {
    PubSubClient *client = static_cast<PubSubClient*>(pvParam);
    while(1) {
        client->loop();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

MqttService::MqttService() {
    mqtt_client_ = new PubSubClient(wifi_client_);
    mqtt_client_->setCallback([this](char *mqtt_topic, byte *payload, unsigned int length) {
        OnMessage(mqtt_topic, payload, length);
    });

    xTaskCreate(mqttLoopTask, "Mqtt_LoopTask", 2048, mqtt_client_, 1, &mqtt_loop_handle_);
}

MqttService::~MqttService() {
    if (mqtt_loop_handle_) {
        vTaskDelete(mqtt_loop_handle_);
    }

    if (mqtt_client_!=nullptr) {
        mqtt_client_->disconnect();
    }
}

bool MqttService::Connect(const std::string& broker_server, const std::string& username, const std::string& password) {
    
    if (mqtt_client_->connected()) {
        return true;
    }

    mqtt_client_->setServer(broker_server.c_str(), 1833);
    mqtt_client_->setKeepAlive(60);

    //stateInfo = "";
    String client_id = "esp32-client-test";
    Log::Info(TAG, "正在连接小鹏AIoT平台: %s .....", broker_server.c_str());
    if (mqtt_client_->connect(client_id.c_str(), username.c_str(), password.c_str())) {
        Log::Info(TAG, "连接到小鹏AIoT平台.");
        return true;
    } 

    Log::Info(TAG, "连接失败, rc=%d", mqtt_client_->state());

    return false;
}

void MqttService::Disconnect() {
    if (mqtt_client_!=nullptr) {
        mqtt_client_->disconnect();
    }
}

void MqttService::PublishData(const std::string& topic, const std::string& payload) {
    mqtt_client_->publish(topic.c_str(), payload.c_str());
}

void MqttService::SubscribeTopic(const std::string& mqtt_topic, std::function<void(const std::string&)> callback) {
    subscribe_callback_[mqtt_topic] = callback;
    mqtt_client_->subscribe(mqtt_topic.c_str());
}

void MqttService::OnMessage(char *mqtt_topic, byte *payload, unsigned int length) {
    if (length < 1) {
      return;
    }

    char data[length];
    for (unsigned int i = 0; i < length; i++) {
        data[i] = (char) payload[i];
    }
    
    std::string topic = mqtt_topic;
    std::string payload_str = data;

    auto it = subscribe_callback_.find(mqtt_topic);
    if (it != subscribe_callback_.end()) {
        it->second(payload_str);
    }

}