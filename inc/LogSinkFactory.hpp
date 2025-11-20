#pragma once
#include <iostream>
#include <memory>
#include "ILogSink.hpp"
#include "MyTypes.hpp"


class LogSinkFactory{
    public:
        static std::unique_ptr<ILogSink> createLogSink(::Enums::LogSinkType_enum logSinkType);
        virtual ~LogSinkFactory() = default;
};