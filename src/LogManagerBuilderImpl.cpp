#include "LogManager.hpp"
#include "LogManagerBuilder.hpp"


LogManagerBuilder::LogManagerBuilder(){
    logManager=std::make_unique<LogManager>();
}

LogManagerBuilder&  LogManagerBuilder::addSink(std::unique_ptr<ILogSink> logSink){
    logManager->addSink(std::move(logSink));
    return *this;
}

LogManagerBuilder&  LogManagerBuilder::addMessage(LogMessage logMessage){
    logManager->addMessage(logMessage);
    return *this;
}

std::unique_ptr<LogManager> LogManagerBuilder::build(){
    return std::move(logManager);
}