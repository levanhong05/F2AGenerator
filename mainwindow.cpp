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

#include "treeitem.h"
#include "treemodel.h"

#include "setting.h"

#include "aboutdialog.h"
#include "languagedialog.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    _highlighter = new Highlighter(ui->txtSourceCode->document());
    _currentLanguage = 0;

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    ui->txtSourceCode->setFont(font);

    ui->codeTreeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);

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

void MainWindow::on_actionLanguage_triggered()
{
    LanguageDialog *language = new LanguageDialog(this);

    connect(language, SIGNAL(languageChanged(QString)), this, SLOT(selectLanguage(QString)));

    language->setCurrentLanguage(_currentLanguage);

    language->show();
}

void MainWindow::selectLanguage(QString language)
{
    // remove the old translator
     qApp->removeTranslator(&_translator);

    if (language == QString("vi_VN")) { //Vietnamese
        _translator.load(":/flowchart/translation/qml_vi.qm");

        _currentLanguage = 1;
    } else if (language == QString("en_US")){ //English
        _translator.load(":/flowchart/translation/qml_en.qm");

        _currentLanguage = 0;
    }

    qApp->installTranslator(&_translator);

    this->update();
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
        _sourceName = "";

        return;
    }

    path = fileName.mid(0, fileName.lastIndexOf("/"));

    settings.write(key, path, "Paths");

    _sourceName = fileName;

    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly)) {
        ui->txtSourceCode->setPlainText(file.readAll());
    }
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->exit();
}

void MainWindow::on_btnClose_clicked()
{
    qApp->exit();
}

void MainWindow::on_btnGenerate_clicked()
{
    SourceNode *node = new SourceNode();
    node->loadFromFile(_sourceName);

    parseTree(node);
}

bool MainWindow::parseTree(SourceNode *node)
{
    //TreeModel model(node);

    //ui->codeTreeView->setModel(&model);
    //ui->codeTreeView->update();

    ui->codeTreeWidget->clear();

    ui->codeTreeWidget->setColumnCount(3);
    ui->codeTreeWidget->setHeaderLabels(QStringList() << tr("Name") << tr("Value") << tr("Type"));

    QList<QTreeWidgetItem *> items;

    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->codeTreeWidget, QStringList() << "root");

    ui->codeTreeWidget->addTopLevelItem(rootItem);

    addTreeChild(node, rootItem);

    ui->codeTreeWidget->expandAll();

    return true;
}

void MainWindow::addTreeChild(SourceNode *node, QTreeWidgetItem *parent)
{
    int index = 0;

    while (index < node->childsCount()) {
        QTreeWidgetItem *item = new QTreeWidgetItem();

        SourceNode *child = node->childAt(index);

        item->setText(0, child->name());
        item->setText(1, child->value());
        item->setText(2, (child->nodeType() == SourceNode::FUNCTION_NODE ? tr("Function") :
                           child->nodeType() == SourceNode::SWITCH_NODE ? tr("Switch") :
                           child->nodeType() == SourceNode::LOOP_NODE ? tr("Loop") :
                           child->nodeType() == SourceNode::CONDITION_NODE ? tr("Condition") :
                           child->nodeType() == SourceNode::PARAM_NODE ? tr("Parameters") : tr("Data")));

        if (child->hasChilds()) {
            addTreeChild(child, item);
        }

        parent->addChild(item);

        index++;
    }
}
