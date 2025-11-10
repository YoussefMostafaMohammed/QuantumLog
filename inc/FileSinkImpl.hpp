#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "LogMessage.hpp" 
#include "ILogSink.hpp"


class FileSinkImpl: public ILogSink{
private:
    std::string path="/";
    std::string fileName="log.txt";

public:
    void setPath(std::string path);
    
    void setFileName(std::string fileName);


    int write(LogMessage logMessage) override;
};