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

#include "settings.h"

#include <QSettings>

Settings::Settings(QObject *parent) :
    QObject(parent)
{
}

void Settings::setValue(const QString &setting, const QVariant &value)
{
    QSettings settings;
    settings.setValue(setting, value);
}

QVariant Settings::value(const QString &setting)
{
    QSettings settings;
    return settings.value(setting);
}

QVariant Settings::value(const QString &setting, const QVariant &defaultValue)
{
    QSettings settings;
    return settings.value(setting, defaultValue);
}

bool Settings::remove(const QString &setting)
{
    QSettings settings;
    if (!settings.contains(setting))
        return false;

    settings.remove(setting);
    return true;
}
