#include <iostream>
#include "LogSinkFactory.hpp"
#include "LogMessage.hpp"
#include "FileSinkImpl.hpp"
#include "ConsoleSinkImpl.hpp"
#include "MyTypes.hpp"

std::unique_ptr<ILogSink> LogSinkFactory::createLogSink(::Enums::LogSinkType logSinkType){
    switch(logSinkType){
        case ::Enums::LogSinkType::CONSOLE: return std::make_unique<ConsoleSinkImpl>();
        case ::Enums::LogSinkType::FILE: return std::make_unique<FileSinkImpl>();
        case ::Enums::LogSinkType::SOCKET: return std::make_unique<ConsoleSinkImpl>();
        default:
            std::cout << "Enter a valid type\n";
            return nullptr;     
    }
}