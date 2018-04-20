QT = core network

CONFIG += c++11
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000

HEADERS += relaisclient.h \
           relaisserver.h \
           relaiswebserver.h

SOURCES += main.cpp \
           relaisclient.cpp \
           relaisserver.cpp \
           relaiswebserver.cpp

include(QtWebserver/QtWebserver.pri)
