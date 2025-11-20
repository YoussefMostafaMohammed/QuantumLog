#pragma once
#include <iostream>
#include <vector>
#include <memory>   
#include "ILogSink.hpp"
#include "LogMessage.hpp"

class LogManager{
    private:
        std::vector<std::unique_ptr<ILogSink>> sinks;
        std::vector<LogMessage>messages;
    public:

    void addSink(std::unique_ptr<ILogSink> logSink);

    void addMessage(LogMessage logMessage);

    void routeMessages(void);
};


