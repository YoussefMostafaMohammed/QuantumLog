#include <iostream>
#include "LogSinkFactory.hpp"
#include "LogMessage.hpp"
#include "FileSinkImpl.hpp"
#include "ConsoleSinkImpl.hpp"
#include "MyTypes.hpp"

std::unique_ptr<ILogSink> LogSinkFactory::createLogSink(::Enums::LogSinkType_enum logSinkType){
    switch(logSinkType){
        case ::Enums::LogSinkType_enum::CONSOLE: return std::make_unique<ConsoleSinkImpl>();
        case ::Enums::LogSinkType_enum::FILE: return std::make_unique<FileSinkImpl>();
        case ::Enums::LogSinkType_enum::SOCKET: return std::make_unique<ConsoleSinkImpl>();
        default:
            std::cout << "Enter a valid type\n";
            return nullptr;     
    }
}