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

#ifndef LANGUAGEDIALOG_H
#define LANGUAGEDIALOG_H

#include <QDialog>

namespace Ui {
class LanguageDialog;
}

class LanguageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LanguageDialog(QWidget *parent = nullptr);
    ~LanguageDialog();

    void setCurrentLanguage(int language);

private slots:
    void on_rbnEnglish_toggled(bool checked);

    void on_rbnVietnamese_toggled(bool checked);

signals:
    void languageChanged(QString language);

private:
    Ui::LanguageDialog *ui;
};

#endif // LANGUAGEDIALOG_H
