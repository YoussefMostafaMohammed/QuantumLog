#pragma once
#include <iostream>
#include <string>
#include "MyTypes.hpp"

class LogMessage{
private:
    std::string name;
    std::string text;
    std::string context;
    std::string time;
    std::string severity;
    std::string unit;
public:
    std::string getName() const;
    void setName(std::string name);

    std::string getText() const;
    void setText(std::string text);

    std::string getContext() const;
    void setContext(std::string context);
    
    std::string getTime() const;
    void setTime();
    
    std::string getSeverity() const;
    void setSeverity(std::string severity);

    std::string getUnit() const;
    void setUnit(std::string unit);

    friend std::ostream& operator<<(std::ostream& out, const LogMessage& msg);
};