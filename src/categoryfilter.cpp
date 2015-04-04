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

#include "categoryfilter.h"

#include "cardproxymodel.h"

CategoryFilter::CategoryFilter(CardProxyModel *parent) :
    QObject(parent), _parent(parent), _name(""), _include(true)
{
}

QString CategoryFilter::name() const
{
    return _name;
}

void CategoryFilter::setName(const QString &name)
{
    _name = name;
    emit nameChanged();
}

bool CategoryFilter::include() const
{
    return _include;
}

void CategoryFilter::setInclude(bool include)
{
    if (include == _include)
        return;
    _include = include;
    emit includeChanged();
}

void CategoryFilter::saveSettings()
{
    _parent->settings()->setValue(
                QString("%1/%2").arg(_parent->settingsSection(), _name), _include);
}
