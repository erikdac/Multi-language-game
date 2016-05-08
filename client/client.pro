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
    game/map.cpp \
    game/keyboardcontroller.cpp \
    game/screenrefresher.cpp \
    game/objects/player.cpp \
    ui/onlinewidget.cpp \
    ui/targetwidget.cpp \
    ui/playerwidget.cpp \
    game/objects/environment.cpp \
    game/objects/water.cpp \
    game/objects/grass.cpp \
    game/objects/stone.cpp

HEADERS  += mainwindow.h \
    json11/json11.hpp \
    network/connection.h \
    network/reader.h \
    ui/gamewidget.h \
    ui/loginwidget.h \
    ui/mainwindow.h \
    game/map.h \
    game/keyboardcontroller.h \
    game/screenrefresher.h \
    game/objects/player.h \
    game/objects/graphics.h \
    ui/onlinewidget.h \
    ui/targetwidget.h \
    ui/playerwidget.h \
    game/objects/environment.h \
    game/objects/water.h \
    game/objects/grass.h \
    game/objects/stone.h

FORMS    += ui/gamewidget.ui \
    ui/loginwidget.ui \
    ui/mainwindow.ui \
    ui/onlinewidget.ui \
    ui/playerwidget.ui

QMAKE_CXXFLAGS += -std=c++0x

LIBS += -lglut

DISTFILES +=
