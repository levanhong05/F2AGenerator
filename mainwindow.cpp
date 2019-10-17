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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>

#include "parser.h"
#include "setting.h"
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    _highlighter = new Highlighter(ui->txtSourceCode->document());

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    ui->txtSourceCode->setFont(font);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *about = new AboutDialog(this);
    about->show();
}

void MainWindow::on_actionOpen_triggered()
{
    Setting settings;

    QString key = "SourcesPath";

    QString path = settings.read(key, "Paths");

    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Source"), path, tr("C/C++ Source File (*.c *.cpp)"));

    if (fileName.isEmpty()) {
        return;
    }

    if (QFile::exists(fileName)) {
        path = fileName.mid(0, fileName.lastIndexOf("/"));

        settings.write(key, path, "Paths");

        QFile file(fileName);

        if (file.open(QIODevice::ReadOnly)) {
            ui->txtSourceCode->setPlainText(file.readAll());
        }
    }
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->exit();
}
