#-------------------------------------------------
#
# Project created by QtCreator 2014-07-31T19:14:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TakeArtificialFive_v1
TEMPLATE = app

INCLUDEPATH += C:/Libraries/boost_1_56_0

LIBS += -L/C:/Libraries/boost_1_56_0/libs

SOURCES += main.cpp\
           core.cpp \
    entity.cpp \
    genes/gene.cpp \
    main_gui.cpp \
    midi_engine.cpp \
    genes/genes.cpp \
    gui/world_frame.cpp

HEADERS  += \
            core.h \
    entity.h \
    genes/gene.h \
    midi_engine.h \
    main_gui.h \
    genes/gene_parameter.h \
    genes/genes.h \
    random_generator.h \
    gui/world_frame.h

FORMS    += maingui.ui
