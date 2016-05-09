#-------------------------------------------------
#
# Project created by QtCreator 2016-03-10T18:33:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LALWE
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    assembler.cpp \
    ram.cpp \
    logger.cpp \
    constants.cpp

HEADERS  += mainwindow.h \
    assembler.h \
    ram.h \
    logger.h \
    constants.h

FORMS    += mainwindow.ui

CONFIG  += c++11
