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
            languagedialog.cpp \
            mainwindow.cpp \
            aboutdialog.cpp \
            setting.cpp \
            highlighter.cpp \
            sourcenode.cpp \
            treeitem.cpp \
            treemodel.cpp

HEADERS  += mainwindow.h \
            aboutdialog.h \
            languagedialog.h \
            treeitem.h \
            treemodel.h \
            version.h \
            setting.h \
            highlighter.h \
            sourcenode.h

FORMS    += mainwindow.ui \
            aboutdialog.ui \
            languagedialog.ui

RESOURCES += \
            resources.qrc

include(translation.pri)

TRANSLATIONS =  translation/qml_vi.ts \
                translation/qml_en.ts

win32:RC_FILE = F2AGenerator.rc

OTHER_FILES +=  images/LICENSE \
                LICENSE \
                *.md \
                F2AGenerator.rc
