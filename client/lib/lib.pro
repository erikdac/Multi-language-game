TARGET = lib

TEMPLATE = lib

CONFIG += -std=c++14

HEADERS += \
    json11/json11.hpp \
    pugixml/src/pugixml.hpp

SOURCES += \
    json11/json11.cpp \
    pugixml/src/pugixml.cpp
