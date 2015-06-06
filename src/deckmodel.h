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

#ifndef DECKMODEL_H
#define DECKMODEL_H

#include <QAbstractListModel>

#include "deckcardmodel.h"

class DeckModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    explicit DeckModel(QObject *parent = 0);

    enum DeckRoles {
        idRole = Qt::UserRole +1,
        nameRole,
        identityNameRole,
        identitySideRole,
        identityFactionRole,
        identityThumbnailRole,
    };

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE DeckCardModel* getDeck(const uint deckId);
    Q_INVOKABLE void addDeck(const QString &name);
    Q_INVOKABLE void copyDeck(const uint deckId, const QString &name = "");
    Q_INVOKABLE void removeDeck(const uint deckId);
    Q_INVOKABLE void exportOCTGN(const uint deckId);

signals:
    void countChanged();

private slots:
    void updateDeckName();
    void updateIdentity();

private:
    QList<DeckCardModel *> _decks;

    void updateRow(QObject* sender, QVector<int> roles);

    void insertDecks(const int row, const QList<DeckCardModel *> ids);
};

#endif // DECKMODEL_H
