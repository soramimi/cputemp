TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

DESTDIR = $$PWD/_bin

SOURCES += \
    cputemp.cpp \
    main.cpp

HEADERS += \
    cputemp.h
