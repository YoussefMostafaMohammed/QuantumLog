#include <iostream>
#include <string>
#include <memory>  
#include "LogMessage.hpp"
#include "LogManager.hpp"
#include "FileSinkImpl.hpp"
#include "ConsoleSinkImpl.hpp"
#include "MyTypes.hpp"
#include "LogFormatter.hpp"
#include "ConsoleSinkImpl.hpp"
#include "LogSinkFactory.hpp"
#include "LogManagerBuilder.hpp"

int main(void){
    
    std::unique_ptr<LogSinkFactory>  logSinkFactory=std::make_unique<LogSinkFactory>();
    
    std::unique_ptr<ILogSink> consoleSinkImpl=logSinkFactory->createLogSink(::Enums::LogSinkType::CONSOLE);
    std::unique_ptr<ILogSink> fileSinkImpl=logSinkFactory->createLogSink(::Enums::LogSinkType::FILE);

    LogFormatter<Polices::CPU> cpuFormater;
    LogFormatter<Polices::GPU> gpuFormater;
    LogFormatter<Polices::RAM> ramFormater;
    
    LogManagerBuilder logManagerBuilder=LogManagerBuilder();

    LogMessage logMessage;
    for(float i =0.0f;i<100.0f;i+=2.24f){
        logMessage=cpuFormater.format(i);
        logManagerBuilder.addMessage(logMessage);
        logMessage=gpuFormater.format(i+1.4f);
        logManagerBuilder.addMessage(logMessage);
        logMessage=ramFormater.format(i+3.2f);
        logManagerBuilder.addMessage(logMessage);
    }

    logManagerBuilder.addSink(std::move(fileSinkImpl));
    logManagerBuilder.addSink(std::move(consoleSinkImpl));
    
    std::unique_ptr<LogManager>  logManager=logManagerBuilder.build();

    logManager->routeMessages();

    return 0;
}