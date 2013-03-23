#-------------------------------------------------
#
# Project created by QtCreator 2013-03-20T07:57:14
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = ETI_BEREK
TEMPLATE = app

LIBS += "C:/Program Files (x86)/OpenAL 1.1 SDK/libs/Win32/OpenAL32.lib"
INCLUDEPATH += "C:/Program Files (x86)/OpenAL 1.1 SDK/include"

SOURCES += main.cpp\
    gui/mainglwidget.cpp \
    mainsystem.cpp \
    gui/mainwindow.cpp \
    sound/sound.cpp \
    graphics/modelling.cpp \
    physics/actions.cpp \
    graphics/graphicssystem.cpp \
    world/map/map.cpp \
    world/map/sector.cpp \
    world/world.cpp \
    world/player.cpp \
    physics/physicsutils.cpp

HEADERS  += gui/mainglwidget.h \
    utils/structures.h \
    mainsystem.h \
    gui/mainwindow.h \
    sound/sound.h \
    graphics/modelling.h \
    physics/actions.h \
    graphics/graphicssystem.h \
    world/map/map.h \
    world/map/sector.h \
    world/world.h \
    world/player.h \
    physics/physicsutils.h

FORMS    += gui/mainwindow.ui
