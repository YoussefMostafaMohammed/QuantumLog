#include "ConsoleSinkImpl.hpp"

int ConsoleSinkImpl:: write(LogMessage logMessage){
    std::cout<<logMessage.getMessage()<<"\n";
    return 0;
}

