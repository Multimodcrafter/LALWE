#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include "stdint.h"
#define sint int32_t

class Logger
{
public:
    Logger();
    static void debug(std::string);
    static void debug(std::string, sint);
    static void warning(std::string);
    static void error(std::string);

private:
    static const int loglevel = 0;
};

#endif // LOGGER_H
