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

#include "deckmodel.h"

#include "dbmanager.h"

DeckModel::DeckModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QSqlQuery query = DBManager::instance()->prepare(
                "SELECT id FROM decks;");
    DBManager::instance()->execute(query);

    QList<DeckCardModel *> decks;
    while (query.next())
    {
        DeckCardModel *tmp = new DeckCardModel(query.value(0).toUInt(), this);
        decks.append(tmp);
    }

    insertDecks(0, decks);
}

int DeckModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _decks.size();
}

QVariant DeckModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    DeckCardModel *deck = _decks.at(index.row());

    switch (role)
    {
    case idRole:
        return deck->deckId();
    case nameRole:
        return deck->deckName();
    case identityNameRole:
        return deck->identityName();
    case identitySideRole:
        return deck->identitySide();
    case identityFactionRole:
        return deck->identityFaction();
    case identityThumbnailRole:
        return deck->identityThumbnail();
    }

    return QVariant();
}

bool DeckModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= rowCount())
        return false;

    DeckCardModel *deck = _decks.at(index.row());

    if (role == nameRole)
    {
        QString name = value.toString();
        deck->setDeckName(name);

        return true;
    }

    return false;
}

QHash<int, QByteArray> DeckModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[idRole] = "id";
    roles[nameRole] = "name";
    roles[identityNameRole] = "identityName";
    roles[identitySideRole] = "identitySide";
    roles[identityFactionRole] = "identityFaction";
    roles[identityThumbnailRole] = "identityThumbnail";

    return roles;
}

DeckCardModel *DeckModel::getDeck(const uint deckId)
{
    for (int i = 0; i < rowCount(); ++i)
    {
        if (_decks.at(i)->deckId() == deckId)
        {
            return _decks.at(i);
        }
    }
    return nullptr;
}

void DeckModel::addDeck(const QString &name)
{
    QSqlQuery query = DBManager::instance()->prepare(
                "INSERT INTO decks (name) VALUES (?);");
    query.bindValue(0, name);
    DBManager::instance()->execute(query);

    QVariant lastId = query.lastInsertId();
    if (!lastId.isValid())
    {
        // TODO rollback?
    }

    DeckCardModel *tmp = new DeckCardModel(lastId.toUInt(), this);

    QList<DeckCardModel *> decks;
    decks.append(tmp);

    insertDecks(rowCount(), decks);
}

void DeckModel::copyDeck(const uint deckId, const QString &name)
{
    DeckCardModel *refDeck = nullptr, *newDeck = nullptr;
    QString newName = name;

    for (int i = 0; i < rowCount(); ++i)
    {
        if (_decks.at(i)->deckId() == deckId)
        {
            refDeck = _decks.at(i);
            break;
        }
    }

    if (refDeck == nullptr)
    {
        return;
    }

    if (newName == "")
    {
        newName = refDeck->deckName() + " (copy)";
    }

    addDeck(newName);
    newDeck = _decks.last();

    DBManager *db = DBManager::instance();
    QSqlQuery query = db->prepare(
                "INSERT INTO deckCards (deckId, strId, count)"
                " SELECT :newId, strId, count FROM deckCards"
                "  WHERE deckId = :oldId;");
    query.bindValue(":newId", newDeck->deckId());
    query.bindValue(":oldId", refDeck->deckId());
    db->execute(query);

    newDeck->updateDeck();
}

void DeckModel::removeDeck(const uint deckId)
{
    for (int i = 0; i < rowCount(); ++i)
    {
        if (_decks.at(i)->deckId() == deckId)
        {
            QSqlQuery query = DBManager::instance()->prepare(
                        "DELETE FROM decks WHERE id = ?;");
            query.bindValue(0, deckId);
            DBManager::instance()->execute(query);

            beginRemoveRows(QModelIndex(), i, i);
            delete _decks.at(i);
            _decks.removeAt(i);
            endRemoveRows();
            return;
        }
    }
}

void DeckModel::exportOCTGN(const int row)
{
    if (row < 0 || row >= rowCount())
        return;

    _decks.at(row)->exportOCTGN();
}

void DeckModel::updateDeckName()
{
    QVector<int> roles;
    roles << nameRole;
    updateRow(QObject::sender(), roles);
}

void DeckModel::updateIdentity()
{
    QVector<int> roles;
    roles << identityNameRole << identitySideRole << identityFactionRole << identityThumbnailRole;
    updateRow(QObject::sender(), roles);
}

void DeckModel::updateRow(QObject *sender, QVector<int> roles)
{
    for (int i = 0; i < rowCount(); ++i)
    {
        if (_decks.at(i) == sender)
        {
            emit dataChanged(index(i), index(i), roles);
        }
    }
}

void DeckModel::insertDecks(const int row, const QList<DeckCardModel *> decks)
{
    if (row < 0 || row > rowCount())
        return;
    if (decks.size() == 0)
        return;

    beginInsertRows(QModelIndex(), row, row + decks.size()-1);
    for (int i = 0; i < decks.size(); ++i)
    {
        connect(decks.at(i), &DeckCardModel::deckNameChanged, this, &DeckModel::updateDeckName);
        connect(decks.at(i), &DeckCardModel::identityChanged, this, &DeckModel::updateIdentity);
        _decks.insert(i + row, decks.at(i));
    }
    endInsertRows();
    emit countChanged();
}
