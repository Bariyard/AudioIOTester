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

CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    audiodevicebase.cpp \
    audiopreferencedialog.cpp \
    testmodule.cpp \
    audioplayer.cpp \
    waveform.cpp \
    microphone.cpp \
    amplitudemonitor.cpp \
    CustomComponent/doubleslider.cpp \
    globalvolumn.cpp \
    synthesizerview.cpp \
    oscillator.cpp \
    microphoneview.cpp

HEADERS  += mainwindow.h \
    audiodevicebase.h \
    audiopreferencedialog.h \
    testmodule.h \
    audioplayer.h \
    waveform.h \
    microphone.h \
    amplitudemonitor.h \
    CustomComponent/doubleslider.h \
    globalvolumn.h \
    synthesizerview.h \
    oscillator.h \
    microphoneview.h

FORMS    += mainwindow.ui \
    audiopreferencedialog.ui \
    synthesizerview.ui \
    microphoneview.ui

#include(3rdParties/portaudio)
INCLUDEPATH += '3rdParties/portaudio/include'
LIBS += -L/usr/local/lib -lportaudio

#include(3rdParties/portaudio)
INCLUDEPATH += '3rdParties/libsndfile/include'
LIBS += -L/usr/local/lib -lsndfile
