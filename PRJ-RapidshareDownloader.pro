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
    addlinkspanel.cpp \
    downloader.cpp \
    comboboxdelegate.cpp \
    mlineedit.cpp \
    progressbardelegate.cpp \
    aboutbox.cpp

HEADERS  += mainwindow.h \
    settingspanel.h \
    addlinkspanel.h \
    downloader.h \
    comboboxdelegate.h \
    mlineedit.h \
    progressbardelegate.h \
    aboutbox.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc

OTHER_FILES += \
    LICENSE.txt \
    README.txt
