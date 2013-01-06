#-------------------------------------------------
#
# Project created by QtCreator 2012-10-26T13:26:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flashprogrammer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    win_qextserialport.cpp \
    qextserialbase.cpp \
    qextserialenumerator.cpp \
    cm3updater.cpp

HEADERS  += mainwindow.h \
    win_qextserialport.h \
    qextserialbase.h \
    qextserialenumerator.h \
    cm3updater.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    myico.rc

LIBS += -lsetupapi

RESOURCES += \
    img.qrc

RC_FILE += myico.rc
