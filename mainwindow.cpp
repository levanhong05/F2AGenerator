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
#include <QStyleFactory>

#include "setting.h"
#include "aboutdialog.h"

#include "sourcenode.h"

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

//    QFile styleFile(":/flowchart/styles/stylesheet.qss");
//    styleFile.open(QFile::ReadOnly);
//    QString styleContent = QLatin1String(styleFile.readAll());

//    qApp->setStyleSheet(styleContent);

//    qApp->setStyle(QStyleFactory::create("Fusion"));

//    QPalette darkPalette;
//    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
//    darkPalette.setColor(QPalette::WindowText, Qt::white);
//    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
//    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
//    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
//    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
//    darkPalette.setColor(QPalette::Text, Qt::white);
//    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
//    darkPalette.setColor(QPalette::ButtonText, Qt::white);
//    darkPalette.setColor(QPalette::BrightText, Qt::red);
//    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

//    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
//    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

//    qApp->setPalette(darkPalette);

//    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
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

        //SourceNode *node = new SourceNode();
        //node->loadFromFile(fileName);
    }
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->exit();
}
