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
    json/json11.cpp \
    network/connection.cpp \
    ui/loginwidget.cpp \
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
    ui/window.cpp \
    game/objects/troll.cpp \
    ui/gamewidget.cpp \
    ui/loadingwidget.cpp \
    game/gamestruct.cpp \
    ui/gameengine/stackedwidget.cpp \
    ui/gameengine/enginewidget.cpp \
    ui/gameengine/glwidget.cpp \
    game/entities/environment.cpp \
    game/entities/grass.cpp \
    game/entities/player.cpp \
    game/entities/self.cpp \
    game/entities/stone.cpp \
    game/entities/troll.cpp \
    game/entities/water.cpp

HEADERS  += mainwindow.h \
    json/json11.hpp \
    network/connection.h \
    ui/loginwidget.h \
    game/objects/actor.h \
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
    ui/window.h \
    game/eventhandler.h \
    game/objects/troll.h \
    ui/gamewidget.h \
    ui/loadingwidget.h \
    config.h \
    game/gamestruct.h \
    ui/gameengine/gamestate.h \
    ui/gameengine/stackedwidget.h \
    ui/gameengine/enginewidget.h \
    ui/gameengine/glwidget.h \
    game/entities/actor.h \
    game/entities/environment.h \
    game/entities/graphics.h \
    game/entities/grass.h \
    game/entities/player.h \
    game/entities/self.h \
    game/entities/stone.h \
    game/entities/troll.h \
    game/entities/water.h

FORMS    += \
    ui/loginwidget.ui \
    ui/playerwidget.ui \
    ui/screenwidget.ui \
    ui/window.ui \
    ui/gamewidget.ui \
    ui/loadingwidget.ui

QMAKE_CXXFLAGS += -std=c++11 -O2 -pthread -Wall -isystem -Wconversion -pedantic

LIBS += -lglut

DISTFILES +=
