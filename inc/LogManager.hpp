#pragma once
#include <iostream>
#include <vector>
#include <memory>   
#include "ILogSink.hpp"
#include "LogMessage.hpp"
#include "Buffer.hpp"
#include <condition_variable>
#include "ILogSink.hpp"
#include "LogMessage.hpp"
#include "Buffer.hpp"


class LogManager{
    private:
        std::vector<std::unique_ptr<ILogSink>> sinks;
        Buffer<LogMessage> messages;
        std::mutex sinksMutex;    
        std::mutex messagesMutex;     
 
    public:
    void addSink(std::unique_ptr<ILogSink> logSink);

    void addMessage(LogMessage logMessage);

    void routeMessages(void);
};
