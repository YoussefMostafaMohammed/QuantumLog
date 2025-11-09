#pragma once
#include <iostream>
#include "LogMessage.hpp"

class ILogSink{
public:
    virtual int write(const LogMessage logMessage) = 0;
    virtual ~ILogSink() = default;
};
