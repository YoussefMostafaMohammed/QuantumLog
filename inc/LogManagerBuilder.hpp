#pragma once
#include <iostream>
#include <vector>
#include <memory>   
#include "ILogSink.hpp"
#include "LogMessage.hpp"

class LogManagerBuilder{
    private:
        std::unique_ptr<LogManager> logManager;
    public:
        LogManagerBuilder();
        LogManagerBuilder& addSink(std::unique_ptr<ILogSink> logSink);
        LogManagerBuilder& addMessage(LogMessage logMessage);
        std::unique_ptr<LogManager> build();  
};

