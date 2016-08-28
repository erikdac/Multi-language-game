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
    ui/loginwidget.cpp \
    game/map.cpp \
    game/objects/player.cpp \
    game/objects/environment.cpp \
    game/objects/water.cpp \
    game/objects/grass.cpp \
    game/objects/stone.cpp \
    game/movementcontroller.cpp \
    game/objects/self.cpp \
    ui/playerwidget.cpp \
    ui/targetwidget.cpp \
    ui/screenwidget.cpp \
    ui/stackedwidget.cpp \
    ui/window.cpp \
    game/objects/troll.cpp \
    ui/gamewidget.cpp \
    ui/loadingwidget.cpp

HEADERS  += mainwindow.h \
    json11/json11.hpp \
    network/connection.h \
    ui/loginwidget.h \
    game/map.h \
    game/objects/player.h \
    game/objects/graphics.h \
    game/objects/environment.h \
    game/objects/water.h \
    game/objects/grass.h \
    game/objects/stone.h \
    game/movementcontroller.h \
    game/objects/self.h \
    ui/playerwidget.h \
    ui/targetwidget.h \
    ui/screenwidget.h \
    ui/gamestate.h \
    ui/stackedwidget.h \
    ui/window.h \
    game/eventhandler.h \
    game/objects/actor.h \
    game/objects/troll.h \
    ui/gamewidget.h \
    ui/loadingwidget.h

FORMS    += \
    ui/loginwidget.ui \
    ui/playerwidget.ui \
    ui/screenwidget.ui \
    ui/window.ui \
    ui/gamewidget.ui \
    ui/loadingwidget.ui

QMAKE_CXXFLAGS += -std=c++0x -pthread -Wall

LIBS += -lglut

DISTFILES +=
