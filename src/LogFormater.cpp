#include "LogFormatter.hpp"
#include "magic_enum.hpp" 
#include <string>

LogMessage LogFormatter<Polices::CPU>::format(const std::string & msg){
    LogMessage logMessage;

    logMessage.setName("QuantumLog");
    logMessage.setContext(std::string(magic_enum::enum_name(Polices::CPU::context)));
    logMessage.setText(msg);

    float value = std::stof(msg); 
    auto sev_enum = Polices::CPU::inferSeverity(value);
    logMessage.setSeverity(std::string(magic_enum::enum_name(sev_enum)));
    logMessage.setUnit("%");
    
    logMessage.setTime();

    return logMessage;
}

LogMessage LogFormatter<Polices::GPU>::format(const std::string & msg){
    LogMessage logMessage;

    logMessage.setName("QuantumLog");
    logMessage.setContext(std::string(magic_enum::enum_name(Polices::GPU::context)));
    logMessage.setText(msg);

    float value = std::stof(msg);
    auto sev_enum = Polices::GPU::inferSeverity(value);
    logMessage.setSeverity(std::string(magic_enum::enum_name(sev_enum)));
    logMessage.setUnit("%");
    
    logMessage.setTime();

    return logMessage;
}

LogMessage LogFormatter<Polices::RAM>::format(const std::string & msg){
    LogMessage logMessage;

    logMessage.setName("QuantumLog");
    logMessage.setContext(std::string(magic_enum::enum_name(Polices::RAM::context)));
    logMessage.setText(msg);

    float value = std::stof(msg);
    auto sev_enum = Polices::RAM::inferSeverity(value);
    logMessage.setSeverity(std::string(magic_enum::enum_name(sev_enum)));
    
    logMessage.setUnit("MB");

    logMessage.setTime();

    return logMessage;
}
