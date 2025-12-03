#include "LogManager.hpp"

void LogManager::addSink(std::unique_ptr<ILogSink> logSink){
    sinks.push_back(std::move(logSink));
}

void LogManager::addMessage(LogMessage logMessage){
    messages.push(logMessage);
}

void LogManager::routeMessages() {
    while (true) {
        auto msg = messages.pop();
        if (!msg) break; 

        for (auto &sink : sinks) {
            sink->write(*msg);
        }
    }
}