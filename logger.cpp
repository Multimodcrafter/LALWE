#include "logger.h"
#include <string>
#include <iostream>

Logger::Logger()
{

}

void Logger::debug(std::string message) {
    if(Logger::loglevel == 0) {
        std::cout << "Debug: " << message << std::endl;
    }
}

void Logger::warning(std::string message) {
    if(Logger::loglevel <= 1) {
        std::cout << "Warning: " << message << std::endl;
    }
}

void Logger::error(std::string message) {
    if(Logger::loglevel <= 2) {
        std::cerr << "Error: " << message << std::endl;
    }
}
