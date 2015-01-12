#-------------------------------------------------
#
# Project created by QtCreator 2015-01-12T15:54:34
#
#-------------------------------------------------

QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simple_web_socket_server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    simplewebsocketserver.cpp \
    simplewebsocketthread.cpp

HEADERS  += mainwindow.h \
    simplewebsocketserver.h \
    simplewebsocketthread.h

FORMS    += mainwindow.ui

DISTFILES += \
    LICENSE
