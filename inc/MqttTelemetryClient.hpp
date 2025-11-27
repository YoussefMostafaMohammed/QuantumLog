#pragma once
#include "LogManager.hpp"
#include <string>
#include <functional>
#include <mqtt/message.h>

namespace mqtt {
    class async_client;
    class token;
}

// Forward declare to make it a friend
class mqtt_callback;

class MqttTelemetryClient {
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    friend class mqtt_callback;
    
    void onMessageReceived(mqtt::const_message_ptr msg);
    
public:
    MqttTelemetryClient(LogManager& logManager, 
                       const std::string& brokerAddress, 
                       const std::string& clientId);
                       
    ~MqttTelemetryClient();
    
    bool connect();
    void disconnect();
    bool subscribe(const std::string& topic);
    
    using MessageCallback = std::function<void(const std::string& sensorName, float value)>;
    void setMessageCallback(MessageCallback callback);
};