#include <iostream>
#include <string>
#include <memory>
#include <thread>  
#include "LogMessage.hpp"
#include "LogManager.hpp"
#include <chrono> 
#include "FileSinkImpl.hpp"
#include "ConsoleSinkImpl.hpp"
#include "MyTypes.hpp"
#include "LogFormatter.hpp"
#include "LogSinkFactory.hpp"
#include "LogManagerBuilder.hpp"
#include "MqttTelemetryClient.hpp"


int main(void) {

    std::unique_ptr<LogSinkFactory> logSinkFactory = std::make_unique<LogSinkFactory>();
    std::unique_ptr<ILogSink> consoleSinkImpl = logSinkFactory->createLogSink(::Enums::LogSinkType::CONSOLE);
    std::unique_ptr<ILogSink> fileSinkImpl = logSinkFactory->createLogSink(::Enums::LogSinkType::FILE);

    LogFormatter<Polices::CPU> cpuFormater;
    LogFormatter<Polices::GPU> gpuFormater;
    LogFormatter<Polices::RAM> ramFormater;
    
    LogManagerBuilder logManagerBuilder = LogManagerBuilder();
    logManagerBuilder.addSink(std::move(fileSinkImpl));
    logManagerBuilder.addSink(std::move(consoleSinkImpl));
    
    std::shared_ptr<LogManager> logManager = logManagerBuilder.build();

    std::string brokerAddress = "tcp://localhost:1883";
    std::string clientId = "QuantumLog_TelemetryClient_" + std::to_string(std::time(nullptr));
    
    MqttTelemetryClient mqttClient(*logManager, brokerAddress, clientId);
    
    if (!mqttClient.connect()) {
        std::cerr << "WARNING: Could not connect to MQTT broker at " << brokerAddress << "\n";
        std::cerr << "          MQTT telemetry reception will be disabled.\n";
        std::cerr << "          Start broker with: sudo systemctl start mosquitto\n\n";
    } else {
        std::string telemetryTopic = "sensors/SENSOR/+";
        if (!mqttClient.subscribe(telemetryTopic)) {
            std::cerr << "WARNING: Failed to subscribe to MQTT topic\n";
        } else {
            std::cout << "✓ MQTT connected and subscribed to: " << telemetryTopic << "\n";
            std::cout << "  MQTT callbacks will run on the library's internal thread\n\n";
        }
    }

    std::cout << "✓ Starting synthetic CPU/GPU/RAM telemetry generation\n";
    std::cout << "  To test MQTT, publish messages in another terminal:\n";
    std::cout << "  mosquitto_pub -t 'sensors/telemetry/temp' -m '{\"sensor_id\":\"temp_1\",\"value\":42.5}'\n";
    std::cout << "  Press Ctrl+C to exit\n\n";

    LogMessage logMessage;



    while(true){
        for (float i = 0.0f; i < 100.0f; i += 2.24f) {
            logMessage = cpuFormater.format(i);
            logManager->logMessage(logMessage);
            
            logMessage = gpuFormater.format(i + 1.4f);
            logManager->logMessage(logMessage);
            
            logMessage = ramFormater.format(i + 3.2f);
            logManager->logMessage(logMessage);         

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));    
        }
    } 

    return 0;
}