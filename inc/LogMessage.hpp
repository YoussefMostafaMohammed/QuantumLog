#pragma once
#include <iostream>
#include <string>
#include "MyTypes.hpp"

class LogMessage{
private:
    ::Enums::TelemetrySrc context;
    ::Enums::SeverityLvl severity;
    std::string name;
    std::string unit;
    uint64_t timeStamp;
    float value;

public:
    std::string getName() const;
    void setAppName(std::string name);

    std::string getValue() const;
    void setValue(float value);

    std::string getContext() const;
    void setContext(::Enums::TelemetrySrc context);
    
    std::string getTimeStamp() const;
    void setTimeStamp();
    
    std::string getSeverity() const;
    void setSeverity(::Enums::SeverityLvl severity);

    std::string getUnit() const;
    void setUnit(std::string unit);

    friend std::ostream& operator<<(std::ostream& out, const LogMessage& msg);

    std::string serializeToProtobuf() const;
};