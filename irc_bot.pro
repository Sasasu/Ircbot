TEMPLATE = app
CONFIG += console c++11 -std=c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lboost_system  -lboost_thread  -lpthread -lboost_regex  -static-libstdc++ -static-libgcc -static
SOURCES += main.cpp \
    ircmessage.cpp

DISTFILES += \
    readme.md \
    LICENSE.md

HEADERS += \
    ircmessage.h

DEFINES += RELEASE
#DEFINES += DEBUG
