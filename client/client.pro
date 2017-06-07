#-------------------------------------------------
#
# Project created by QtCreator 2015-06-08T18:08:31
#
#-------------------------------------------------

QT       += core network gui opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = client
TEMPLATE = app

SOURCES += \
    main.cpp\
    network/connection.cpp \
    ui/loginwidget.cpp \
    game/movementcontroller.cpp \
    ui/playerwidget.cpp \
    ui/targetwidget.cpp \
    ui/screenwidget.cpp \
    ui/window.cpp \
    ui/gamewidget.cpp \
    ui/loadingwidget.cpp \
    game/gamestruct.cpp \
    ui/gameengine/stackedwidget.cpp \
    ui/gameengine/enginewidget.cpp \
    ui/gameengine/glwidget.cpp \
    graphics/polygon.cpp \
    graphics/graphics.cpp \
    game/entities/environment.cpp \
    game/entities/player.cpp \
    game/entities/self.cpp \
    game/entities/troll.cpp \
    external/json11/json11.cpp \
    external/pugixml/src/pugixml.cpp \
    game/entities/actor.cpp \
    game/parser.cpp

HEADERS += \
    ui/window.h \
    network/connection.h \
    ui/loginwidget.h \
    game/movementcontroller.h \
    ui/playerwidget.h \
    ui/targetwidget.h \
    ui/screenwidget.h \
    game/eventhandler.h \
    ui/gamewidget.h \
    ui/loadingwidget.h \
    config.h \
    game/gamestruct.h \
    ui/gameengine/gamestate.h \
    ui/gameengine/stackedwidget.h \
    ui/gameengine/enginewidget.h \
    ui/gameengine/glwidget.h \
    graphics/graphics.h \
    game/entities/actor.h \
    game/entities/environment.h \
    game/entities/player.h \
    game/entities/self.h \
    game/entities/troll.h \
    external/json11/json11.hpp \
    external/pugixml/src/pugiconfig.hpp \
    external/pugixml/src/pugixml.hpp \
    game/parser.h \
    graphics/polygon.h

FORMS += \
    ui/loginwidget.ui \
    ui/playerwidget.ui \
    ui/screenwidget.ui \
    ui/window.ui \
    ui/gamewidget.ui \
    ui/loadingwidget.ui

CONFIG += -std=c++11 -O2 -pthread -Wall -isystem -Wconversion -pedantic

LIBS += -lglut -lopengl32

RESOURCES += \
    resources.qrc

DISTFILES +=
