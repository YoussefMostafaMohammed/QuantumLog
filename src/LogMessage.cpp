#include "LogMessage.hpp"

std::string LogMessage::getText()const{
        return text;
}

void LogMessage::setText(std::string text){
        this->text=text;
}


std::ostream& operator <<(std::ostream& out, const LogMessage& msg){
    out<<msg.text;
    return out;
}