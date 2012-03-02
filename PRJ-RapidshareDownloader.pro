#-------------------------------------------------
#
# Project created by QtCreator 2012-02-27T20:09:35
#
#-------------------------------------------------

QT       += core gui network

TARGET = PRJ-RapidshareDownloader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingspanel.cpp \
    fontcomboboxdelegate.cpp \
    addlinkspanel.cpp \
    downloader.cpp

HEADERS  += mainwindow.h \
    settingspanel.h \
    fontcomboboxdelegate.h \
    addlinkspanel.h \
    downloader.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc
