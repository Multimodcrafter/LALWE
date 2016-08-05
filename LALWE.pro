#-------------------------------------------------
#
# Project created by QtCreator 2016-03-10T18:33:33
#
#-------------------------------------------------

QT       += qml quick core concurrent

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
    alu.cpp \
    lalwe_appmanager.cpp

HEADERS  += \
    assembler.h \
    ram.h \
    logger.h \
    constants.h \
    processor.h \
    controller.h \
    register.h \
    alu.h \
    lalwe_appmanager.h

CONFIG  += c++11

RESOURCES += \
    qml.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../stxxl-1.4.1/build/lib/release/ -lstxxl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../stxxl-1.4.1/build/lib/debug/ -lstxxl

INCLUDEPATH += $$PWD/../../stxxl-1.4.1/build/include
DEPENDPATH += $$PWD/../../stxxl-1.4.1/build/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../stxxl-1.4.1/build/lib/release/libstxxl.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../stxxl-1.4.1/build/lib/debug/libstxxl.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../stxxl-1.4.1/build/lib/release/stxxl.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../stxxl-1.4.1/build/lib/debug/stxxl.lib
