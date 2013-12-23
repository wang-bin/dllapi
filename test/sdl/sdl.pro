QT =
TEMPLATE = app
CONFIG += console
TARGET = sdl
STATICLINK = 0
PROJECTROOT = $$PWD/../..
BUILD_DIR=$$OUT_PWD/../../out
include($$PROJECTROOT/src/libDllAPI.pri)
preparePaths($$OUT_PWD/../../out)

SOURCES = main.cpp dllapi/SDL/SDL.cpp
HEADERS = dllapi/SDL/SDL.h
