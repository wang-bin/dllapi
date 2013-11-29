QT =
TEMPLATE = app
TARGET = hello
STATICLINK = 0
PROJECTROOT = $$PWD/../..
BUILD_DIR=$$OUT_PWD/../../out
include($$PROJECTROOT/src/libDllAPI.pri)
preparePaths($$OUT_PWD/../../out)

#win32:LIBS += -lUser32

SOURCES += main.cpp \
    hello.cpp
HEADERS += \ 
    hello.h


