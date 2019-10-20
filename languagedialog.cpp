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

#include "languagedialog.h"
#include "ui_languagedialog.h"

LanguageDialog::LanguageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LanguageDialog)
{
    ui->setupUi(this);
}

LanguageDialog::~LanguageDialog()
{
    delete ui;
}

void LanguageDialog::setCurrentLanguage(int language)
{
    if (language == 0) {
        ui->rbnEnglish->setChecked(true);
    } else if (language == 1) {
        ui->rbnVietnamese->setChecked(true);
    }
}

void LanguageDialog::on_rbnEnglish_toggled(bool checked)
{
    if (checked) {
        emit languageChanged("en_US");

        this->close();
    }
}

void LanguageDialog::on_rbnVietnamese_toggled(bool checked)
{
    if (checked) {
        emit languageChanged("vi_VN");

        this->close();
    }
}
