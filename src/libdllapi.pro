TEMPLATE = lib

QT       =
CONFIG   += console
CONFIG   -= app_bundle

CONFIG *= dllapi-buildlib
#var with '_' can not pass to pri?
PROJECTROOT = $$PWD/..
!include(libdllapi.pri): error(could not find libdllapi.pri)
preparePaths($$OUT_PWD/../out)

SOURCES += dllapi.cpp

HEADERS += dllapi_global.h dllapi.h \
    dllapi_p.h


win32 {
HEADERS += ts_string.h
SOURCES += ts_string.cpp
} else {
    QMAKE_LFLAGS += -ldl
}


