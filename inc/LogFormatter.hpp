#pragma once

#include <iostream>
#include <string>
#include "magic_enum.hpp"
#include "MyTypes.hpp"
#include "LogMessage.hpp"

//
// Generic template (fallback)
//
template<typename T>
class LogFormatter {
public:
    LogMessage format(const float& msg) {
        LogMessage logMessage;

        logMessage.setAppName("QuantumLog");
        logMessage.setContext(Polices::UNKNOWN::context);
        logMessage.setValue(msg);

        float value = msg;
        auto sev_enum = Polices::UNKNOWN::inferSeverity(value);
        logMessage.setSeverity(sev_enum);

        logMessage.setUnit("NA");
        logMessage.setTimeStamp();

        return logMessage;
    }
};

//
// CPU SPECIALIZATION
//
template<>
class LogFormatter<Polices::CPU> {
public:
    LogMessage format(const float& msg) {
        LogMessage logMessage;

        logMessage.setAppName("QuantumLog");
        logMessage.setContext(Polices::CPU::context);
        logMessage.setValue(msg);

        float value = msg;
        auto sev_enum = Polices::CPU::inferSeverity(value);
        logMessage.setSeverity(sev_enum);

        logMessage.setUnit("%");
        logMessage.setTimeStamp();

        return logMessage;
    }
};

//
// GPU SPECIALIZATION
//
template<>
class LogFormatter<Polices::GPU> {
public:
    LogMessage format(const float& msg) {
        LogMessage logMessage;

        logMessage.setAppName("QuantumLog");
        logMessage.setContext(Polices::GPU::context);
        logMessage.setValue(msg);

        float value = msg;
        auto sev_enum = Polices::GPU::inferSeverity(value);
        logMessage.setSeverity(sev_enum);

        logMessage.setUnit("%");
        logMessage.setTimeStamp();

        return logMessage;
    }
};

//
// RAM SPECIALIZATION
//
template<>
class LogFormatter<Polices::RAM> {
public:
    LogMessage format(const float& msg) {
        LogMessage logMessage;

        logMessage.setAppName("QuantumLog");
        logMessage.setContext(Polices::RAM::context);
        logMessage.setValue(msg);

        float value = msg;
        auto sev_enum = Polices::RAM::inferSeverity(value);
        logMessage.setSeverity(sev_enum);

        logMessage.setUnit("MB");
        logMessage.setTimeStamp();

        return logMessage;
    }
};

//
// SENSOR SPECIALIZATION
//
template<>
class LogFormatter<Polices::SENSOR> {
public:
    LogMessage format(const float& msg) {
        LogMessage logMessage;

        logMessage.setAppName("QuantumLog");
        logMessage.setContext(Polices::SENSOR::context);
        logMessage.setValue(msg);

        float value = msg;
        auto sev_enum = Polices::SENSOR::inferSeverity(value);
        logMessage.setSeverity(sev_enum);

        logMessage.setUnit("°C");
        logMessage.setTimeStamp();

        return logMessage;
    }
};
