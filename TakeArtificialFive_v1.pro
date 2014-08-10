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
           core.cpp \
    entity.cpp \
    genes/gene.cpp \
    main_gui.cpp \
    midi_engine.cpp

HEADERS  += \
            core.h \
    entity.h \
    genes/gene.h \
    midi_engine.h \
    main_gui.h \
    genes/gene_parameter.h

FORMS    += maingui.ui
