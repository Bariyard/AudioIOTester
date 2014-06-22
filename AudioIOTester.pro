#-------------------------------------------------
#
# Project created by QtCreator 2014-06-21T16:28:09
#
#-------------------------------------------------
cache();

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AudioIOTester
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    audioiomanager.cpp \
    audiodevicebase.cpp

HEADERS  += mainwindow.h \
    audioiomanager.h \
    audiodevicebase.h

FORMS    += mainwindow.ui

INCLUDEPATH += '/Users/bariyard/Developer/portaudio/include'
LIBS += -L/Users/bariyard/Developer/portaudio/lib -lportaudio

