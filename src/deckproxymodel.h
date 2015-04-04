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

#ifndef DECKPROXYMODEL_H
#define DECKPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QQmlParserStatus>
#include <QStringList>

class DeckProxyModel : public QSortFilterProxyModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QObject* source READ source WRITE setSource)

public:
    explicit DeckProxyModel(QObject *parent = 0);

    // QQmlParserStatus interface
    void classBegin();
    void componentComplete();

    QObject* source() const;
    void setSource(QObject *source);

signals:
    void sortStringsChanged();

protected:
    // QSortFilterProxyModel interface
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    QMap<QString, int> _sortNameRoleMap;
    QStringList _sortList;
    QList<bool> _sortDescList;
    QStringList _sortDefaultList;
    QList<bool> _sortDefaultDescList;

    void loadSortStrings();
};

#endif // DECKPROXYMODEL_H
