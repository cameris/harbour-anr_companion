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

#include "countcardmodel.h"

CountCardModel::CountCardModel(QObject *parent) :
    CardModel(parent)
{
}

QVariant CountCardModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    if (role > LastCardRole)
    {
        switch (role)
        {
        case countRole:
            return _count.value(_cards.at(index.row())->id());
        }
    }

    return CardModel::data(index, role);
}

bool CountCardModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= rowCount())
        return false;

    if (role > LastCardRole)
    {
        Card *card = _cards.at(index.row());
        if (role == countRole)
        {
            int newCount = value.toInt();
            int currentCount = _count.value(card->id());

            if (newCount == currentCount)
                return true;
            if (newCount < 1)
                return false;

            _count.insert(card->id(), newCount);
            dbUpdateCount(card->id());

            QVector<int> roles;
            roles << role;
            emit dataChanged(index, index, roles);

            _totalCount += newCount - currentCount;
            emit totalCountChanged();

            return true;
        }
    }

    return false;
}

QHash<int, QByteArray> CountCardModel::roleNames() const
{
    QHash<int, QByteArray> roles = CardModel::roleNames();
    roles[countRole] = "count";
    return roles;
}

void CountCardModel::addCard(const QString &strId, const int count)
{
    if (_count.contains(strId))
    {
        int newCount = _count.value(strId) + count;
        _count.insert(strId, newCount);
        dbUpdateCount(strId);
        for (int i = 0; i < _cards.size(); ++i)
        {
            if (_cards.at(i)->id() == strId)
            {
                QVector<int> roles;
                QModelIndex tmp = QAbstractListModel::index(i);
                roles << countRole;
                emit dataChanged(tmp, tmp, roles);
                break;
            }
        }
    }
    else
    {
        _count.insert(strId, count);
        insertCards(_cards.size(), getCards(QStringList(strId)));
        dbInsertCard(strId);
    }
    _totalCount += count;
    emit totalCountChanged();
}

void CountCardModel::removeCard(const QString &strId, const int count)
{
    if (_count.contains(strId))
    {
        int newCount = _count.value(strId, 0) - count;
        if (newCount >= 1)
        {
            _count.insert(strId, newCount);
            _totalCount -= count;
            dbUpdateCount(strId);

            int i;
            for (i = 0; i < _cards.size(); ++i)
            {
                if (_cards.at(i)->id() == strId)
                {
                    break;
                }
            }

            QVector<int> roles;
            QModelIndex tmp = QAbstractListModel::index(i);
            roles << countRole;

            emit dataChanged(tmp, tmp, roles);
            emit totalCountChanged();
        }
        else
        {
            int i;
            for (i = 0; i < _cards.size(); ++i)
            {
                if (_cards.at(i)->id() == strId)
                {
                    break;
                }
            }

            _totalCount -= _count.value(strId, 0);

            beginRemoveRows(QModelIndex(), i, i);
            delete _cards.at(i);
            _cards.removeAt(i);
            _count.remove(strId);
            endRemoveRows();

            dbDeleteCard(strId);
            emit totalCountChanged();
        }
    }
}

void CountCardModel::loadCards()
{
    updateTotalCount();
}

void CountCardModel::unloadCards()
{
    CardModel::unloadCards();
    _count.clear();
    updateTotalCount();
}

uint CountCardModel::totalCount() const
{
    return _totalCount;
}

void CountCardModel::updateTotalCount()
{
    _totalCount = 0;
    foreach (int count, _count.values())
    {
        _totalCount += count;
    }
    emit totalCountChanged();
}
