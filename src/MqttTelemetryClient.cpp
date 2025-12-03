#include "MqttTelemetryClient.hpp"
#include "LogFormatter.hpp"
#include "LogMessage.hpp"
#include "MyTypes.hpp"
#include <mqtt/async_client.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

class mqtt_callback : public virtual mqtt::callback {
    MqttTelemetryClient* parent;
public:
    explicit mqtt_callback(MqttTelemetryClient* p) : parent(p) {}
    void message_arrived(mqtt::const_message_ptr msg) override;
};

class MqttTelemetryClient::Impl {
public:
    Impl(LogManager& lm, const std::string& broker, const std::string& clientId)
        : logManager(lm), client(std::make_unique<mqtt::async_client>(broker, clientId)) {}
    
    LogManager& logManager;
    std::unique_ptr<mqtt::async_client> client;
    MessageCallback userCallback;
    
    std::shared_ptr<mqtt_callback> callback;
};

void mqtt_callback::message_arrived(mqtt::const_message_ptr msg) {
    parent->onMessageReceived(msg);
}

void MqttTelemetryClient::onMessageReceived(mqtt::const_message_ptr msg) {
    try {
        auto payload = msg->get_payload_str();
        json data = json::parse(payload);
        
        std::string sensorId = data["sensor_id"];
        float value = data["value"];
        
        if (pImpl->userCallback) {
            pImpl->userCallback(sensorId, value);
        } else {
            LogMessage logMessage;
            logMessage.setAppName("QuantumLog");
            logMessage.setContext(Enums::TelemetrySrc::SENSOR);
            logMessage.setValue(value);
            logMessage.setSeverity(Polices::SENSOR::inferSeverity(value));
            logMessage.setUnit("°C");
            logMessage.setTimeStamp();
            
            pImpl->logManager.logMessage(logMessage);
        }
    } catch (const std::exception& e) {
        std::cerr << "MQTT Message Parse Error: " << e.what() << std::endl;
    }
}

MqttTelemetryClient::MqttTelemetryClient(LogManager& logManager, 
                                         const std::string& brokerAddress, 
                                         const std::string& clientId)
    : pImpl(std::make_unique<Impl>(logManager, brokerAddress, clientId)) {
    
    pImpl->callback = std::make_shared<mqtt_callback>(this);
    pImpl->client->set_callback(*pImpl->callback); // Dereference!
}

MqttTelemetryClient::~MqttTelemetryClient() = default;

bool MqttTelemetryClient::connect() {
    try {
        auto connOpts = mqtt::connect_options_builder()
            .clean_session(true)
            .finalize();
        pImpl->client->connect(connOpts)->wait();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "MQTT Connection Error: " << e.what() << std::endl;
        return false;
    }
}

void MqttTelemetryClient::disconnect() {
    try {
        pImpl->client->disconnect()->wait();
    } catch (...) {}
}

bool MqttTelemetryClient::subscribe(const std::string& topic) {
    try {
        pImpl->client->subscribe(topic, 1)->wait();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "MQTT Subscribe Error: " << e.what() << std::endl;
        return false;
    }
}

void MqttTelemetryClient::setMessageCallback(MessageCallback callback) {
    pImpl->userCallback = callback;
}