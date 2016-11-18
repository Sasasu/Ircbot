TEMPLATE = app
CONFIG += console c++11 -std=c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lboost_system  -lboost_thread  -lpthread  -static-libstdc++ -static-libgcc -static
SOURCES += main.cpp

DISTFILES += \
    readme.md \
    LICENSE.md
