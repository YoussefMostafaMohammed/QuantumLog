#include <iostream>
#include "LogMessage.hpp"
#include "LogManager.hpp"
#include "FileSinkImpl.hpp"
#include "ConsoleSinkImpl.hpp"
#include "MyTypes.hpp"
#include "LogFormatter.hpp"
#include <string>

int main(void){
    std::unique_ptr<ILogSink> consoleSinkImpl=std::make_unique<ConsoleSinkImpl>();
    std::unique_ptr<ILogSink> fileSinkImpl=std::make_unique<FileSinkImpl>();
    LogFormatter<Polices::CPU> cpuFormater;
    LogFormatter<Polices::GPU> gpuFormater;
    LogFormatter<Polices::RAM> ramFormater;
    LogManager logManger=LogManager();

    LogMessage logMessage;
    for(float i =0.0f;i<100.0f;i+=2.24f){
        logMessage=cpuFormater.format(std::to_string(i));
        logManger.addMessage(logMessage);
        logMessage=gpuFormater.format(std::to_string(i+1.4f));
        logManger.addMessage(logMessage);
        logMessage=ramFormater.format(std::to_string(i+3.2f));
        logManger.addMessage(logMessage);
    }


    logManger.addSink(std::move(fileSinkImpl));
    logManger.addSink(std::move(consoleSinkImpl));
    
    logManger.routeMessages();
    return 0;
}