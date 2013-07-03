TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = depends libdllapi lib test

libdllapi.file = src/libDllAPI.pro
lib.depends += libdllapi
test.depends += depends libdllapi

OTHER_FILES += README.md

EssentialDepends =
OptionalDepends =

#include(root.pri)
