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

#ifndef CATEGORYFILTER_H
#define CATEGORYFILTER_H

#include <QObject>

class CardProxyModel;

class CategoryFilter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(bool include READ include WRITE setInclude NOTIFY includeChanged)
public:
    explicit CategoryFilter(CardProxyModel *parent = 0);

    QString name() const;
    void setName(const QString &name);

    bool include() const;
    void setInclude(bool include = true);

    void saveSettings();

signals:
    void nameChanged();
    void includeChanged();

private:
    CardProxyModel *_parent;

    QString _name;
    bool _include;

};

#endif // CATEGORYFILTER_H
