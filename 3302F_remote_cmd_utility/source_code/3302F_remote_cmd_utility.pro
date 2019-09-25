#-------------------------------------------------
#
# Project created by QtCreator 2014-06-01T15:57:42
#
#-------------------------------------------------

QT       += core gui
QT       +=serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3302F_remote_cmd_utility
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cmdinputwidget.cpp \
    filestringlist.cpp \
    batchDialog.cpp

HEADERS  += mainwindow.h \
    cmdinputwidget.h \
    filestringlist.h \
    batchDialog.h

FORMS    += mainwindow.ui \
    batchDialog.ui

RESOURCES += \
    pictures/QComShell_Pic.qrc
