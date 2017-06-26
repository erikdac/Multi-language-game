TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    lib \
    src \
    app \
    tests

src.depends = lib
app.depends = src
tests.depends = src
