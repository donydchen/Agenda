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



SOURCES += main.cpp\
        mainwindow.cpp \
    loginwindow.cpp \
    backend/src/controller/sqlite/SqliteService.cpp \
    backend/src/controller/json/JsonService.cpp \
    backend/src/controller/json/Storage.cpp \
    backend/src/model/Date.cpp \
    backend/src/model/Meeting.cpp \
    backend/src/model/User.cpp \

HEADERS  += mainwindow.h \
    loginwindow.h \
    backend/src/controller/AgendaService.h \
    backend/src/controller/json/JsonService.h \
    backend/src/controller/json/Storage.h \
    backend/src/controller/sqlite/SqliteService.h \
    backend/src/model/Date.h \
    backend/src/model/Meeting.h \
    backend/src/model/User.h \


FORMS    += mainwindow.ui \
    loginwindow.ui


copydata.commands = $(COPY_DIR) $$PWD/resources $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
