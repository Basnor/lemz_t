QT += core network
QT -= gui

TARGET = recorder
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/Recorder.cpp \
    src/DspClient.cpp \
    src/ParamParser.cpp

HEADERS += \
    src/Recorder.h \
    src/DspClient.h \
    src/ParamParser.h

