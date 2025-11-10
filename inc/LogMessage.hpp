#pragma once
#include <iostream>
#include <string>

class LogMessage{
private:
    std::string name;
    std::string text;        
    std::string context;    
    std::string time;    
    std::string severity;    
public:
    std::string getText() const;
    void setText(std::string message);
    friend std::ostream& operator<<(std::ostream& out, const LogMessage& msg);
};
