TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lboost_system  -lboost_thread  -lpthread -lssl -lcrypto
SOURCES += main.cpp

DISTFILES += \
    readme.md \
    LICENSE.md
