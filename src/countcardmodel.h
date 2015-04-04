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

#ifndef COUNTCARDMODEL_H
#define COUNTCARDMODEL_H

#include "cardmodel.h"

class CountCardModel : public CardModel
{
    Q_OBJECT

    Q_PROPERTY(uint totalCount READ totalCount NOTIFY totalCountChanged)
public:
    explicit CountCardModel(QObject *parent = 0);

    enum CardRoles {
        countRole = LastCardRole +1
    };

    // QAbstractItemModel interface
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void addCard(const QString &strId, const int count = 1);
    Q_INVOKABLE void removeCard(const QString &strId, const int count = 1);

    Q_INVOKABLE void loadCards();
    Q_INVOKABLE void unloadCards();

    uint totalCount() const;

signals:
    void totalCountChanged();

protected:
    QHash<QString, int> _count;
    uint _totalCount;

    virtual void dbUpdateCount(const QString &strId) = 0;
    virtual void dbInsertCard(const QString &strId) = 0;
    virtual void dbDeleteCard(const QString &strId) = 0;

    void updateTotalCount();

};

#endif // COUNTCARDMODEL_H
