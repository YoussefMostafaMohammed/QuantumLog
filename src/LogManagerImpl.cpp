#include "LogManager.hpp"

void LogManager::addSink(std::unique_ptr<ILogSink> logSink){
    sinks.push_back(std::move(logSink));
}

void LogManager::addMessage(LogMessage logMessage){
    messages.push_back(logMessage);
}

void LogManager::routeMessages(){
    for(auto &sink:sinks){
        for(auto message:messages){
            sink->write(message);
        }
    }
}