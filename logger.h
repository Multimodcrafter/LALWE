#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include "stdint.h"
#define sint int32_t

class Logger
{
public:
    Logger();
    static void debug(std::string message);
    static void debug(std::string message, sint value);
    static void warning(std::string message);
    static void error(std::string message);
    static void error(std::string message, sint value);

private:
    static const int loglevel = 0;
};

#endif // LOGGER_H
