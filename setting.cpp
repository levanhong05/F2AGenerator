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

#include "setting.h"

#include <QSettings>
#include <QApplication>
#include <QStandardPaths>

#include "version.h"

Setting::Setting(QObject *parent) :
    QObject (parent)
{
}

QString Setting::read(QString key, QString group)
{
    QString value = "";

#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\" + QString(VER_COMPANYNAME_STR) + "\\" + QString(VER_PRODUCTNAME_STR) + "\\" + group, QSettings::NativeFormat);
    value = settings.value(key).toString();
#else
    QSettings settings(QString(VER_COMPANYNAME_STR), QString(VER_PRODUCTNAME_STR));

    settings.beginGroup(group);
    const QStringList childKeys = settings.childKeys();

    foreach(const QString &childKey, childKeys) {
        if (childKey == key) {
             value = settings.value(childKey).toString();

             break;
        }
    }

    settings.endGroup();
#endif

    return value;
}

void Setting::write(QString key, QString value, QString group)
{
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\" + QString(VER_COMPANYNAME_STR) + "\\" + QString(VER_PRODUCTNAME_STR) + "\\" + group, QSettings::NativeFormat);
    settings.setValue(key, value);
#else
    QSettings settings(QString(VER_COMPANYNAME_STR), QString(VER_PRODUCTNAME_STR));
    settings.beginGroup(group);
    settings.setValue(key, value);
    settings.endGroup();
#endif
}

QString Setting::readINI(QString path, QString key, QString group)
{
    QString value = "";

    QSettings settings(path, QSettings::IniFormat);

    settings.beginGroup(group);
    const QStringList childKeys = settings.childKeys();

    foreach(const QString &childKey, childKeys) {
        if (childKey == key) {
             value = settings.value(childKey).toString();

             break;
        }
    }

    settings.endGroup();

    return value;
}

void Setting::writeINI(QString path, QString key, QString value, QString group)
{
    QSettings settings(path, QSettings::IniFormat);

    settings.beginGroup(group);
    settings.setValue(key, value);
    settings.endGroup();
}

QStringList Setting::readAllGroup(QString group)
{
    QStringList values = QStringList();

#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\" + QString(VER_COMPANYNAME_STR) + "\\" + QString(VER_PRODUCTNAME_STR) + "\\" + group, QSettings::NativeFormat);

    const QStringList childKeys = settings.childKeys();

    foreach(const QString &childKey, childKeys) {
        values.append(childKey + ";;" + settings.value(childKey).toString());
    }
#else
    QSettings settings(QString(VER_COMPANYNAME_STR), QString(VER_PRODUCTNAME_STR));

    settings.beginGroup(group);
    const QStringList childKeys = settings.childKeys();

    foreach(const QString &childKey, childKeys) {
        values.append(childKey + ";;" + settings.value(childKey).toString());
    }

    settings.endGroup();
#endif

    return values;
}
