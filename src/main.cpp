#include <iostream>
#include "LogMessage.hpp"
#include "LogManager.hpp"
#include "FileSinkImpl.hpp"
#include "ConsoleSinkImpl.hpp"

int main(void){
    std::unique_ptr<ILogSink> consoleSinkImpl=std::make_unique<ConsoleSinkImpl>();
    std::unique_ptr<ILogSink> fileSinkImpl=std::make_unique<FileSinkImpl>();

    LogMessage logMessage1=LogMessage();
    LogMessage logMessage2=LogMessage();
    LogMessage logMessage3=LogMessage();
    LogMessage logMessage4=LogMessage();
    LogMessage logMessage5=LogMessage();
    LogMessage logMessage6=LogMessage();
    LogMessage logMessage7=LogMessage();

    logMessage1.setText("Hello ");
    logMessage2.setText("My ");
    logMessage3.setText("Name ");
    logMessage4.setText("Is ");
    logMessage5.setText("Youssef ");
    logMessage6.setText("Mostafa ");
    logMessage7.setText("Mohammed \n");
    
    LogManager logManger=LogManager();
    logManger.addSink(std::move(fileSinkImpl));
    logManger.addSink(std::move(consoleSinkImpl));
    
    logManger.addMessage(logMessage1);
    logManger.addMessage(logMessage2);
    logManger.addMessage(logMessage3);
    logManger.addMessage(logMessage4);
    logManger.addMessage(logMessage5);
    logManger.addMessage(logMessage6);
    logManger.addMessage(logMessage7);
    logManger.routeMessages();
    
    return 0;
}