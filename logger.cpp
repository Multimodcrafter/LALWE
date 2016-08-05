#include "logger.h"
#include <string>
#include <iostream>

Logger* Logger::loggerInst = nullptr;

Logger::Logger(QObject& guiObject)
{
    this->guiObject = &guiObject;
    Logger::loggerInst = this;
}

void Logger::debug(std::string message) {
    if(Logger::loglevel <= 0) {
        std::cout << "Debug: " << message << std::endl;
        printLn(QString::fromStdString("<p style=\"color:blue\">Debug: " + message + "</p>"));
    }
}

void Logger::debug(std::string message, sint value) {
    if(Logger::loglevel <= 0) {
        std::cout << "Debug: " << message << value << std::endl;
        printLn(QString::fromStdString("<p style=\"color:blue\">Debug: " + message) + QVariant::fromValue(value).toString() + "</p>");
    }
}

void Logger::info(std::string message) {
    if(Logger::loglevel <= 1) {
        std::cout << "Info: " << message << std::endl;
        printLn(QString::fromStdString("<p style=\"color:green\">Info: " + message + "</p>"));
    }
}

void Logger::warning(std::string message) {
    if(Logger::loglevel <= 2) {
        std::cout << "Warning: " << message << std::endl;
        printLn(QString::fromStdString("<p style=\"color:#ffaa00\">Warning: " + message + "</p>"));
    }
}

void Logger::warning(std::string message, sint value) {
    if(Logger::loglevel <= 2) {
        std::cout << "Warning: " << message << value << std::endl;
        printLn(QString::fromStdString("<p style=\"color:#ffaa00\">Warning: " + message) + QVariant::fromValue(value).toString() + "</p>");
    }
}

void Logger::error(std::string message) {
    if(Logger::loglevel <= 3) {
        std::cerr << "Error: " << message << std::endl;
        printLn(QString::fromStdString("<p style=\"color:red\">Error: " + message + "</p>"));
    }
}

void Logger::error(std::string message, sint value) {
    if(Logger::loglevel <= 3) {
        std::cerr << "Error: " << message << value << std::endl;
        printLn(QString::fromStdString("<p style=\"color:red\">Error: " + message) + QVariant::fromValue(value).toString() + "</p>");
    }
}

void Logger::printLn(const QString &line) {
    QMetaObject::invokeMethod(guiObject,"printLine",Q_ARG(QVariant,line));
}
