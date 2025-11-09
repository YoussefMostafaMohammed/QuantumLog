#pragma once
#include <iostream>
#include <string>

class LogMessage{
private:
    std::string message;    
public:
    std::string getMessage() const;
    void setMessage(std::string message);
    friend std::ostream& operator<<(std::ostream& out, const LogMessage& msg);
};
