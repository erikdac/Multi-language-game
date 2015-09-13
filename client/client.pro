#-------------------------------------------------
#
# Project created by QtCreator 2015-06-08T18:08:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    json11/json11.cpp \
    loginwidget.cpp \
    network/connection.cpp \
    network/network_reader.cpp

HEADERS  += mainwindow.h \
    json11/json11.hpp \
    loginwidget.h \
    network/connection.h \
    network/network_reader.h

FORMS    += mainwindow.ui \
    loginwidget.ui

QMAKE_CXXFLAGS += -std=c++0x
