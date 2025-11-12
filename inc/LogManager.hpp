#pragma once
#include <iostream>
#include <vector>
#include "ILogSink.hpp"
#include "LogMessage.hpp"

class LogManager{
    private:
        std::vector<ILogSink*>sinks;
        std::vector<LogMessage>messages;
    public:

    void addSink(ILogSink *logSink);

    void addMessage(LogMessage logMessage);

    void routeMessages(void);
};
