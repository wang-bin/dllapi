TEMPLATE = app


TARGET = tst_dllapi
STATICLINK = 0
PROJECTROOT = $$PWD/..
include($$PROJECTROOT/src/libDllAPI.pri)
preparePaths($$OUT_PWD/../out)

#win32:LIBS += -lUser32

SOURCES += main.cpp \
    hello.cpp
HEADERS += \ 
    hello.h


