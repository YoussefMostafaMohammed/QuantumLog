#include "LogManager.hpp"
#include <chrono> 
#include <thread> 

void LogManager::addSink(std::unique_ptr<ILogSink> logSink) {
    std::lock_guard<std::mutex> lock(sinksMutex);
    sinks.push_back(std::move(logSink));
}

void LogManager::addMessage(LogMessage logMessage) {
    std::lock_guard<std::mutex> lock(messagesMutex);
    messages.push(logMessage);
}

void LogManager::routeMessages() {
    while (true) {
        std::optional<LogMessage> msg;
        {
            std::lock_guard<std::mutex> lock(messagesMutex);
            msg = messages.pop();
        }
        if (msg) {  
            std::lock_guard<std::mutex> sinkLock(sinksMutex);
            for (auto &sink : sinks) {
                sink->write(*msg);
            }
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}
