#include "LogManager.hpp"

void LogManager::addSink(std::unique_ptr<ILogSink> logSink){
    sinks.push_back(std::move(logSink));
}

void LogManager::addMessage(LogMessage logMessage){
    messages.push_back(logMessage);
}

void LogManager::routeMessages(){
    for(auto &sink:sinks){
        for (const auto& message : messages){
            sink->write(message);
        }
    }
}


LogManagerBuilder::LogManagerBuilder(){
    logManager=std::make_unique<LogManager>();
}

LogManagerBuilder&  LogManagerBuilder::addSink(std::unique_ptr<ILogSink> logSink){
    logManager->addSink(std::move(logSink));
    return *this;
}

LogManagerBuilder&  LogManagerBuLogManagerBuilderider::addMessage(LogMessage logMessage){
    logManager->addMessage(logMessage);
    return *this;
}

std::unique_ptr<LogManager> LogManagerBuilder::build(){
    return std::move(logManger);
}