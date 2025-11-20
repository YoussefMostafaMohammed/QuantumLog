#include "LogFormater.hpp"

template<>
LogMessage LogFormatter<Polices::CPU>::format(const std::string & msg){
        LogMessage logMessage=LogMessage();
        
}

template<>
LogMessage LogFormatter<Polices::GPU>::format(const std::string & msg){
    
}

template<>
LogMessage LogFormatter<Polices::RAM>::format(const std::string & msg){
    
}