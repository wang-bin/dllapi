TEMPLATE = lib
QT = core
CONFIG -= shared dll ##otherwise the following shared is true, why?
CONFIG *= staticlib

STATICLINK = 0
PROJECTROOT = $$PWD/../../..
message($$PROJECTROOT)
BUILD_DIR=$$OUT_PWD/../../../out
include($$PROJECTROOT/src/libDllAPI.pri)
preparePaths($$OUT_PWD/../../../out)


SOURCES = swresample.cpp
HEADERS = swresample.h
