#include <iostream>
#include "LogMessage.hpp"
#include "LogManger.hpp"
#include "FileSinkImpl.hpp"
#include "ConsoleSinkImpl.hpp"

int main(void){
    FileSinkImpl fileSinkImpl;
    ConsoleSinkImpl consoleSinkImpl;

    LogMessage logMessage1=LogMessage();
    LogMessage logMessage2=LogMessage();
    LogMessage logMessage3=LogMessage();
    LogMessage logMessage4=LogMessage();
    LogMessage logMessage5=LogMessage();
    LogMessage logMessage6=LogMessage();
    LogMessage logMessage7=LogMessage();

    logMessage1.setMessage("Hello ");
    logMessage2.setMessage("My ");
    logMessage3.setMessage("Name ");
    logMessage4.setMessage("Is ");
    logMessage5.setMessage("Youssef ");
    logMessage6.setMessage("Mostafa ");
    logMessage7.setMessage("Mohammed \n");
    
    LogManger logManger=LogManger();
    
    logManger.addSink(&fileSinkImpl);
    logManger.addSink(&consoleSinkImpl);
    
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