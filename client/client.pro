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
    client.cpp \
    connection.cpp \
    login.cpp \
    json11/json11.cpp \
    loginWidget.cpp

HEADERS  += mainwindow.h \
    connection.hpp \
    login.hpp \
    json11/json11.hpp \
    loginwidget.h

FORMS    += mainwindow.ui \
    loginwidget.ui

QMAKE_CXXFLAGS += -std=c++0x
