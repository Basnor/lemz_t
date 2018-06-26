QT += core network
QT -= gui

TARGET = recorder
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Recorder.cpp \
    DspClient.cpp

HEADERS += \
    Recorder.h \
    DspClient.h

