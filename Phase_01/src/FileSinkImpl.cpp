#include "FileSinkImpl.hpp"

void FileSinkImpl::setPath(std::string path){
    this->path=path;
}

void FileSinkImpl::setFileName(std::string fileName){
    this->fileName=fileName;
}

int FileSinkImpl::write(LogMessage logMessage){
    std::ofstream file(fileName, std::ios::app);
    if (!file) {
        return 1;
    }
    file << logMessage.getMessage() << std::endl;
    file.close();
    return 0;
}