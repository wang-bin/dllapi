TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = depends libdllapi test

libdllapi.file = src/libDllAPI.pro
test.file = test/tst_DllAPI.pro
test.depends += depends libdllapi

OTHER_FILES += README

EssentialDepends =
OptionalDepends =

include(root.pri)
