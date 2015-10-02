#-------------------------------------------------
#
# Project created by QtCreator 2015-06-08T18:08:31
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app

SOURCES += main.cpp\
    json11/json11.cpp \
    network/connection.cpp \
    network/reader.cpp \
    ui/gamewidget.cpp \
    ui/loginwidget.cpp \
    ui/mainwindow.cpp \
    game/player.cpp \
    game/map.cpp \
    game/keyboardcontroller.cpp \
    game/screenrefresher.cpp

HEADERS  += mainwindow.h \
    json11/json11.hpp \
    network/connection.h \
    network/reader.h \
    ui/gamewidget.h \
    ui/loginwidget.h \
    ui/mainwindow.h \
    game/player.h \
    game/map.h \
    game/keyboardcontroller.h \
    game/screenrefresher.h

FORMS    += ui/gamewidget.ui \
    ui/loginwidget.ui \
    ui/mainwindow.ui

QMAKE_CXXFLAGS += -std=c++0x

LIBS += -lglut
