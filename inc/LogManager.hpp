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
#include "ThreadPool.hpp"

class LogManager{
    private:
        std::vector<std::unique_ptr<ILogSink>> sinks;
        Buffer<LogMessage> messages;
        std::mutex sinksMutex;    
        std::mutex messagesMutex;     
        ThreadPool threadPool;
 
    public:
    
    LogManager(size_t numThreads = std::thread::hardware_concurrency());
    
    void addSink(std::unique_ptr<ILogSink> logSink);

    void addMessage(LogMessage logMessage);

    void routeMessagess(void);

    void logMessage(const LogMessage &msg);
};
