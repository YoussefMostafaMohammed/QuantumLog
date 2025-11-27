#include <mqtt/async_client.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>
#include <random>
#include <csignal>
#include <iostream>
#include <atomic>  // Required for std::atomic

using json = nlohmann::json;  // Convenience alias

std::atomic<bool> g_running{true};

void signal_handler(int) {
    std::cout << "\nShutting down gracefully...\n";
    g_running = false;
}

int main() {
    // Graceful shutdown handler
    std::signal(SIGINT, signal_handler);
    
    try {
        mqtt::async_client client("tcp://localhost:1883", "CppTempPublisher");
        
        // Connect with error handling
        if (!client.connect()->wait_for(std::chrono::seconds(5))) {
            std::cerr << "ERROR: Failed to connect to MQTT broker!\n";
            return 1;
        }
        std::cout << "✓ Connected to broker\n";

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(15.0, 90.0);
        
        while (g_running) {
            std::string sensors[] = {"room1", "room2", "outdoor"};
            for (auto& sensor : sensors) {
                if (!g_running) break;  // Check exit flag
                
                json payload = {
                    {"sensor_id", sensor},
                    {"value", std::round(dis(gen) * 10) / 10}
                };
                
                std::string topic = "sensors/SENSOR/" + sensor;
                client.publish(topic, payload.dump())->wait();
                std::cout << "Published: " << topic << " -> " << payload.dump() << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
        
        // Cleanup
        client.disconnect()->wait();
        std::cout << "✓ Disconnected\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}