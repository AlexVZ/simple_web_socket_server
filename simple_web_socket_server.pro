#-------------------------------------------------
#
# Project created by QtCreator 2015-01-12T15:54:34
#
#-------------------------------------------------

QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simple_web_socket_server
TEMPLATE = app

LIBS += -lcrypto
# PKGCONFIG += openssl

SOURCES += main.cpp\
        mainwindow.cpp \
    simplewebsocketserver.cpp \
    simplewebsocketthread.cpp \
    diffiehellmanhelper.cpp \
    aes256helper.cpp

HEADERS  += mainwindow.h \
    simplewebsocketserver.h \
    simplewebsocketthread.h \
    diffiehellmanhelper.h \
    aes256helper.h

FORMS    += mainwindow.ui

DISTFILES += \
    LICENSE
