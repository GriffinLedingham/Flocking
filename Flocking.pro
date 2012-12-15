#-------------------------------------------------
#
# Project created by QtCreator 2012-11-04T00:29:51
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Flocking
TEMPLATE = app


SOURCES += \
    mainwindow.cpp \
    main.cpp \
    basicopenglview.cpp \
    bird.cpp \
    flock.cpp \
    springConstraint.cpp \
    cloth.cpp \
    massPoint.cpp \
    cube.cpp

HEADERS  += \
    ui_mainwindow.h \
    mainwindow.h \
    basicopenglview.h \
    bird.h \
    flock.h \
    cloth.h \
    springConstraint.h \
    massPoint.h \
    cube.h

FORMS    += \
    mainwindow.ui
