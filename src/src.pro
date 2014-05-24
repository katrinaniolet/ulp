#-------------------------------------------------
#
# Project created by QtCreator 2014-05-10T03:16:06
#
#-------------------------------------------------

QT       += core gui sql
QT -= gui
CONFIG += console
CONFIG -= app_bundle

greaterThan(QT_MAJOR_VERSION, 4): QT -= widgets

TARGET = src
TEMPLATE = app


SOURCES += main.cpp\
    database.cpp \
    textprocessor.cpp \
    originaltextprocessor.cpp \
    databaseexception.cpp \
    textprocessorexception.cpp \
    processor.cpp \
    filewriter.cpp \
    idealoutputfilewriter.cpp \
    nospacesfilewriter.cpp \
    nospacesprocessor.cpp

HEADERS  += \
    database.h \
    textprocessor.h \
    originaltextprocessor.h \
    databaseexception.h \
    textprocessorexception.h \
    processor.h \
    filewriter.h \
    idealoutputfilewriter.h \
    nospacesfilewriter.h \
    nospacesprocessor.h

FORMS    +=

OTHER_FILES += ../texts/czech/13083-0.txt
