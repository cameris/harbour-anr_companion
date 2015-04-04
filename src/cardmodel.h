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

#ifndef CARDMODEL_H
#define CARDMODEL_H

#include <QAbstractListModel>

#include "card.h"

class CardModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    explicit CardModel(QObject *parent = 0);

    enum CardRoles {
        idRole = Qt::UserRole +1,
        nameRole,
        numberRole,
        sideRole,
        factionRole,
        typeRole,
        subtypeRole,
        factionCostRole,
        costRole,
        strengthRole,
        advancementCostRole,
        agendaPointsRole,
        memoryRole,
        trashCostRole,
        linkRole,
        minDeckSizeRole,
        influenceLimitRole,
        limitedRole,
        uniqueRole,
        textRole,
        setRole,
        setSortOrderRole,
        imageRole,
        thumbnailRole,
        LastCardRole = thumbnailRole
    };

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    QStringList sets() const;
    QStringList sides() const;
    QStringList factions() const;
    QStringList types() const;

    Q_INVOKABLE void loadCards();
    Q_INVOKABLE void unloadCards();

signals:
    void countChanged();

private slots:
    void updatedImage(const QString &id);

protected:
    QHash<uint, Set *> _sets;
    QHash<uint, Side *> _sides;
    QHash<uint, Faction *> _factions;
    QHash<uint, Type *> _types;
    QList<Card *> _cards;

    void init();

    QList<Card *> getCards();
    QList<Card *> getCards(QStringList strIds);
    void insertCards(const int row, const QList<Card *> cards);
};

#endif // CARDMODEL_H
