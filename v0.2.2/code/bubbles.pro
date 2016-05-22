#-------------------------------------------------
#
# Project created by QtCreator 2016-04-07T11:33:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bubbles
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

RC_ICONS = icon.ico

RESOURCES += \
    images.qrc \
    fonts.qrc
