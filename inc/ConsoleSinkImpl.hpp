#pragma once
#include <iostream>
#include "LogMessage.hpp" 
#include "ILogSink.hpp"

class ConsoleSinkImpl : public ILogSink{
    int write(LogMessage logMessage) override;
};
