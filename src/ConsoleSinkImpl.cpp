#include "ConsoleSinkImpl.hpp"

int ConsoleSinkImpl:: write(LogMessage logMessage){
    std::cout<<logMessage<<"\n";
    return 0;
}

