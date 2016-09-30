#-------------------------------------------------
#
# Project created by QtCreator 2016-09-30T11:53:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Agenda
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    backend/src/AgendaService.cpp \
    backend/src/AgendaUI.cpp \
    backend/src/Date.cpp \
    backend/src/Meeting.cpp \
    backend/src/Storage.cpp \
    backend/src/User.cpp \
    loginwindow.cpp

HEADERS  += mainwindow.h \
    backend/src/AgendaService.h \
    backend/src/AgendaUI.h \
    backend/src/Date.h \
    backend/src/Meeting.h \
    backend/src/Storage.h \
    backend/src/User.h \
    loginwindow.h

FORMS    += mainwindow.ui \
    loginwindow.ui

DISTFILES += \
    backend/src/Agenda.json \
    backend/doc/Agenda_Architecture_Graph(NOT_UML).pdf \
    backend/doc/File_Format.pdf \
    backend/pic/1.png \
    backend/pic/2.png \
    backend/README.md
