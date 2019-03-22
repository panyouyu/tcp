
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS QABSTRACTSOCKET_DEBUG

SOURCES += \
        main.cpp \
        tcpclient.cpp

HEADERS += \
        tcpclient.h
