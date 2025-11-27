#include "LogFormatter.hpp"
#include "LogMessage.hpp"
#include "MyTypes.hpp"
#include "magic_enum.hpp" 
#include <string>

LogMessage LogFormatter<Polices::CPU>::format(const float & msg){
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

LogMessage LogFormatter<Polices::GPU>::format(const float & msg){
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

LogMessage LogFormatter<Polices::RAM>::format(const float & msg){
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

LogMessage LogFormatter<Polices::SENSOR>::format(const float & msg){
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