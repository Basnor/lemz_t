QT += core network
QT -= gui

TARGET = recorder
CONFIG += console c++14
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/Recorder.cpp \
    src/DspClient.cpp \
    src/ParamParser.cpp \

HEADERS += \
    src/Recorder.h \
    src/DspClient.h \
    src/ParamParser.h \

TRANSLATIONS += \
    tr/recorder_rus.ts

DISTFILES += \
    tr/recorder_rus.ts

RESOURCES += \
    translations.qrc


system(lrelease \"$$_PRO_FILE_\")

tr.commands = lupdate \"$$_PRO_FILE_\" && lrelease \"$$_PRO_FILE_\"
PRE_TARGETDEPS += tr
QMAKE_EXTRA_TARGETS += tr

INCLUDEPATH = .
