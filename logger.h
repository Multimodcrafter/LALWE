#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <QString>
#include <QObject>
#include <QVariant>
#include <QMetaObject>
#include "stdint.h"
#define sint int32_t

class Logger
{
public:
    Logger(QObject& guiObject);
    void debug(std::string message);
    void debug(std::string message, sint value);
    void info(std::string message);
    void warning(std::string message);
    void warning(std::string message, sint value);
    void error(std::string message);
    void error(std::string message, sint value);
    const int loglevel = 0;
    static Logger* loggerInst;
private:
    QObject* guiObject;
    void printLn(const QString& line);
};

#endif // LOGGER_H
