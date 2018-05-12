#-------------------------------------------------
#
# Project created by QtCreator 2015-06-08T18:08:31
#
#-------------------------------------------------

QT       += core network gui opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = src
TEMPLATE = lib

CONFIG += -pthread -isystem

SOURCES += \
    game/entities/actor.cpp \
    game/entities/environment.cpp \
    game/entities/player.cpp \
    game/entities/self.cpp \
    game/entities/troll.cpp \
    game/gamestruct.cpp \
    game/movementcontroller.cpp \
    graphics/graphics.cpp \
    graphics/polygon.cpp \
    network/connection.cpp \
    ui/gameengine/enginewidget.cpp \
    ui/gameengine/glwidget.cpp \
    ui/gameengine/stackedwidget.cpp \
    ui/gamewidget.cpp \
    ui/loadingwidget.cpp \
    ui/loginwidget.cpp \
    ui/playerwidget.cpp \
    ui/screenwidget.cpp \
    ui/targetwidget.cpp \
    ui/window.cpp \
    game/parser.cpp

HEADERS += \
    game/entities/actor.h \
    game/entities/environment.h \
    game/entities/player.h \
    game/entities/self.h \
    game/entities/troll.h \
    game/eventhandler.h \
    game/gamestruct.h \
    game/movementcontroller.h \
    graphics/graphics.h \
    graphics/polygon.h \
    network/connection.h \
    ui/gameengine/enginewidget.h \
    ui/gameengine/gamestate.h \
    ui/gameengine/glwidget.h \
    ui/gameengine/stackedwidget.h \
    ui/gamewidget.h \
    ui/loadingwidget.h \
    ui/loginwidget.h \
    ui/playerwidget.h \
    ui/screenwidget.h \
    ui/targetwidget.h \
    ui/window.h \
    config.h \
    game/parser.h

FORMS += \
    ui/gamewidget.ui \
    ui/loadingwidget.ui \
    ui/loginwidget.ui \
    ui/playerwidget.ui \
    ui/screenwidget.ui \
    ui/window.ui

LIBS += -lopengl32

RESOURCES += \
    resources.qrc

DISTFILES += \
    resources/environments/grass.xml \
    resources/environments/stone.xml \
    resources/environments/water.xml

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/release/ -llib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/debug/ -llib
else:unix: LIBS += -L$$OUT_PWD/../lib/ -llib

INCLUDEPATH += $$PWD/../lib
DEPENDPATH += $$PWD/../lib
