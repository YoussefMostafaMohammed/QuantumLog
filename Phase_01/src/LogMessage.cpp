#include "LogMessage.hpp"

std::string LogMessage::getMessage()const{
        return message;
}

void LogMessage::setMessage(std::string message){
        this->message=message;
}


std::ostream& operator <<(std::ostream& out, const LogMessage& msg){
    out<<msg.message;
    return out;
}