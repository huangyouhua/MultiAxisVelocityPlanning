#-------------------------------------------------
#
# Project created by QtCreator 2019-08-08T10:29:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Interface
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    src/myMatrix.c \
    src/Interpolation.c \
    src/LookAhead.c \
    src/PCS.c \
    src/velocity_plan.c \
    calculationthread.cpp \
    common.cpp \
    calvelthread.cpp

HEADERS += \
        mainwindow.h \
    common.h \
    calculationthread.h \
    calvelthread.h

FORMS += \
        mainwindow.ui

#LIBS+= /media/hyh/Study/Desktop/TempFile/Interface/Interface/src/myMatrix.c
#INCLUDEPATH += /media/hyh/Study/Desktop/TempFile/Interface/Interface/src
INCLUDEPATH += src
