#-------------------------------------------------
#
# Project created by QtCreator 2018-01-13T22:16:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nmemo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14
CONFIG(release, debug | release):DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += \
        main.cc \
        mainwindow.cc \
    utils.cc \
    core.cc \
    common_values.cc \
    memowidget.cc \
    tabs.cc \
    books.cc \
    memos.cc \
    files.cc

HEADERS += \
        mainwindow.h \
    common_types.h \
    utils.h \
    core.h \
    common_values.h \
    memowidget.h \
    tabs.h \
    books.h \
    memos.h \
    files.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    data.qrc
