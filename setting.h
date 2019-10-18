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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Setting : public QObject
{
    Q_OBJECT

public:
    explicit Setting(QObject *parent = nullptr);

    QString read(QString key, QString group = "Components");
    void write(QString key, QString value, QString group = "Components");

    QString readINI(QString path, QString key, QString group = "Components");
    void writeINI(QString path, QString key, QString value, QString group = "Components");

    QStringList readAllGroup(QString group = "Components");
};

#endif // SETTINGS_H
