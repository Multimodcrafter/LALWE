#-------------------------------------------------
#
# Project created by QtCreator 2016-03-10T18:33:33
#
#-------------------------------------------------

QT       += qml quick

TARGET = LALWE
TEMPLATE = app


SOURCES += main.cpp\
    assembler.cpp \
    ram.cpp \
    logger.cpp \
    constants.cpp \
    processor.cpp \
    controller.cpp \
    register.cpp \
    alu.cpp

HEADERS  += \
    assembler.h \
    ram.h \
    logger.h \
    constants.h \
    processor.h \
    controller.h \
    register.h \
    alu.h

CONFIG  += c++11

RESOURCES += \
    qml.qrc
