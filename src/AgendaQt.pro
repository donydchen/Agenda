#-------------------------------------------------
#
# Project created by QtCreator 2016-09-30T11:53:35
#
#-------------------------------------------------

QT       += core gui

LIBS     += -lsqlite3
QMAKE_CXXFLAGS += -std=c++11


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AgendaQt
TEMPLATE = app



SOURCES += AgendaQt.cpp\
        ui/qt5/mainwindow.cpp \
    ui/qt5/loginwindow.cpp \
    bll/sqlite/SqliteService.cpp \
    bll/json/JsonService.cpp \
    dal/json/Storage.cpp \
    entity/Date.cpp \
    entity/Meeting.cpp \
    entity/User.cpp \

HEADERS  += ui/qt5/mainwindow.h \
    ui/qt5/loginwindow.h \
    bll/AgendaService.h \
    bll/json/JsonService.h \
    bll/sqlite/SqliteService.h \
    dal/json/Storage.h \
    entity/Date.h \
    entity/Meeting.h \
    entity/User.h \


FORMS    += ui/qt5/mainwindow.ui \
    ui/qt5/loginwindow.ui


copydata.commands = $(COPY_DIR) $$PWD/resources $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
