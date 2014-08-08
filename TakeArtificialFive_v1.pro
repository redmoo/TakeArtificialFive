#-------------------------------------------------
#
# Project created by QtCreator 2014-07-31T19:14:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TakeArtificialFive_v1
TEMPLATE = app


SOURCES += main.cpp\
           maingui.cpp \
           core.cpp \
    midiengine.cpp

HEADERS  += maingui.h \
            core.h \
    midiengine.h

FORMS    += maingui.ui
