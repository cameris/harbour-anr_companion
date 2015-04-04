/*
 * Copyright (C) 2014 cameris
 *
 * This file is part of ANR Companion.
 *
 * ANR Companion is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ANR Companion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ANR Companion.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QVariant>

class Settings : public QObject
{
    Q_OBJECT

public:
    explicit Settings(QObject *parent = 0);

    Q_INVOKABLE void setValue(const QString &setting, const QVariant &value);
    Q_INVOKABLE QVariant value(const QString &setting);
    Q_INVOKABLE QVariant value(const QString &setting, const QVariant &defaultValue);
    Q_INVOKABLE bool remove(const QString &setting);
};

#endif // SETTINGS_H
