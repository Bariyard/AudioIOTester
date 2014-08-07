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
    audiodevicebase.cpp \
    observer.cpp \
    audiopreferencedialog.cpp

HEADERS  += mainwindow.h \
    audiodevicebase.h \
    observer.h \
    audiopreferencedialog.h

FORMS    += mainwindow.ui \
    audiopreferencedialog.ui

#include(3rdParties/portaudio)
INCLUDEPATH += '3rdParties/portaudio/include'
LIBS += -L/usr/local/lib -lportaudio

