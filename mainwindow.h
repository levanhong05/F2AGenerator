/****************************************************************************
**
** Copyright (C) 2019 Eric Lee.
** Contact: levanhong05@gmail.com
** Company: Freelancer
**
** This file is part of the F2A Generator project.
**
**All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTranslator>

#include "highlighter.h"

#include "sourcenode.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionOpen_triggered();

    void on_actionQuit_triggered();

    void on_btnClose_clicked();

    void on_btnGenerate_clicked();

    void on_actionLanguage_triggered();

public slots:
    void selectLanguage(QString language);

protected:
    // this event is called, when a new translator is loaded or the system language is changed
    void changeEvent(QEvent*);

private:
    bool parseTree(SourceNode *node);

    void addTreeChild(SourceNode *node, QTreeWidgetItem *parent);

private:
    Ui::MainWindow *ui;

    Highlighter *_highlighter;
    QTranslator _translator;

    QString _sourceName;

    int _currentLanguage;

};

#endif // MAINWINDOW_H
