#-------------------------------------------------
#
# Project created by QtCreator 2015-10-12T08:38:57
#
#-------------------------------------------------

QT       += core gui
QT 	 += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dongchat
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    drawer.cpp \
    server.cpp \
    client.cpp

HEADERS  += widget.h \
    drawer.h \
    server.h \
    client.h

FORMS    += widget.ui \
    drawer.ui \
    server.ui \
    client.ui

DISTFILES +=

RESOURCES += \
    imagesrsc.qrc
