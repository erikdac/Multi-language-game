TEMPLATE = subdirs

CONFIG += ordered -std=c++14 -O2 -Wall -Wconversion -pedantic

SUBDIRS = \
    lib \
    src \
    app \
    tests

src.depends = lib
app.depends = src
tests.depends = src
