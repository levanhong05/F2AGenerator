#-------------------------------------------------
#
# Project created by QtCreator 2019-10-14T21:14:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = F2AGenerator
TEMPLATE = app

SOURCES +=  main.cpp\
            mainwindow.cpp \
            parser.cpp \
            aboutdialog.cpp \
            setting.cpp \
            highlighter.cpp \
            sourcenode.cpp

HEADERS  += mainwindow.h \
            parser.h \
            aboutdialog.h \
            version.h \
            setting.h \
            highlighter.h \
            sourcenode.h

FORMS    += mainwindow.ui \
            aboutdialog.ui

RESOURCES += \
            resources.qrc

win32:RC_FILE = F2AGenerator.rc

OTHER_FILES +=  images/LICENSE \
                LICENSE \
                *.md \
                F2AGenerator.rc
