#include "LogManger.hpp"

void LogManger::addSink(ILogSink *logSink){
    sinks.push_back(logSink);
}

void LogManger::addMessage(LogMessage logMessage){
    messages.push_back(logMessage);
}

void LogManger::routeMessages(){
    for(auto sink:sinks){
        for(auto message:messages){
            sink->write(message);
        }
    }
}