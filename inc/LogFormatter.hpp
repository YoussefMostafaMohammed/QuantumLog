#pragma once
#include <iostream>
#include "MyTypes.hpp"
#include "LogMessage.hpp"

template<typename T>
class LogFormatter {
    public:
        LogMessage format(const std::string & msg);
};

template<>
class LogFormatter<Polices::CPU> {
    public:
        LogMessage format(const std::string & msg);
};

template<>
class LogFormatter<Polices::GPU> {
    public:
        LogMessage format(const std::string & msg);
};

template<>
class LogFormatter<Polices::RAM> {
    public:
        LogMessage format(const std::string & msg);
};
