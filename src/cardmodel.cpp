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

#include "cardmodel.h"

#include <sailfishapp.h>

#include "dbmanager.h"
#include "downloader.h"

CardModel::CardModel(QObject *parent) :
    QAbstractListModel(parent)
{
    connect(Downloader::instance(), &Downloader::updatedImage, this, &CardModel::updatedImage);

    init();
}

int CardModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _cards.size();
}

QVariant CardModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    Card *card = _cards.at(index.row());

    switch (role)
    {
    case idRole:
        return card->id();
    case nameRole:
        return card->name();
    case numberRole:
        return card->number();
    case sideRole:
        return card->side()->name;
    case factionRole:
        return card->faction()->name;
    case typeRole:
        return card->type()->name;
    case subtypeRole:
        return card->subtype();
    case factionCostRole:
        return card->factionCost();
    case costRole:
        return card->cost();
    case strengthRole:
        return card->strength();
    case advancementCostRole:
        return card->advancementCost();
    case agendaPointsRole:
        return card->agendaPoints();
    case memoryRole:
        return card->memory();
    case trashCostRole:
        return card->trashCost();
    case linkRole:
        return card->link();
    case minDeckSizeRole:
        return card->minDeckSize();
    case influenceLimitRole:
        return card->influenceLimit();
    case limitedRole:
        return card->limited();
    case uniqueRole:
        return card->unique();
    case textRole:
        return card->text();
    case setRole:
        return card->set()->name;
    case setSortOrderRole:
        return card->set()->cycleId * 100 + card->set()->numberInCycle;
    case imageRole:
        return card->image();
    case thumbnailRole:
        return card->thumbnail();
    }

    return QVariant();
}

QHash<int, QByteArray> CardModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[idRole] = "id";
    roles[nameRole] = "name";
    roles[numberRole] = "number";
    roles[sideRole] = "side";
    roles[factionRole] = "faction";
    roles[typeRole] = "type";
    roles[subtypeRole] = "subtype";
    roles[factionCostRole] = "factionCost";
    roles[costRole] = "cost";
    roles[strengthRole] = "strength";
    roles[advancementCostRole] = "advancementCost";
    roles[agendaPointsRole] = "agendaPoints";
    roles[memoryRole] = "memory";
    roles[trashCostRole] = "trashCost";
    roles[linkRole] = "link";
    roles[minDeckSizeRole] = "minDeckSize";
    roles[influenceLimitRole] = "influenceLimit";
    roles[limitedRole] = "limited";
    roles[uniqueRole] = "unique",
    roles[textRole] = "text";
    roles[setRole] = "set";
    roles[setSortOrderRole] = "setSortOrder";
    roles[imageRole] = "image";
    roles[thumbnailRole] = "thumbnail";

    return roles;
}

bool sortSetAscCycleOrder(const Set *set1, const Set *set2)
{
    if (set1->cycleId == set2->cycleId)
        return set1->numberInCycle < set2->numberInCycle;

    return set1->cycleId < set2->cycleId;
}

QStringList CardModel::sets() const
{
    QList<Set *> setList = _sets.values();
    qStableSort(setList.begin(), setList.end(), sortSetAscCycleOrder);

    QStringList setNames;
    for (int i = 0; i < setList.size(); ++i)
    {
        setNames.append(setList.at(i)->name);
    }

    return setNames;
}

QStringList CardModel::sides() const
{
    QList<Side *> sideList = _sides.values();

    QStringList sideNames;
    for (int i = 0; i < sideList.size(); ++i)
    {
        sideNames.append(sideList.at(i)->name);
    }
    sideNames.sort(Qt::CaseInsensitive);
    return sideNames;
}

QStringList CardModel::factions() const
{
    QList<Faction *> factionList = _factions.values();

    QStringList factionNames;
    for (int i = 0; i < factionList.size(); ++i)
    {
        factionNames.append(factionList.at(i)->name);
    }
    factionNames.sort(Qt::CaseInsensitive);
    return factionNames;
}

QStringList CardModel::types() const
{
    QList<Type *> typeList = _types.values();

    QStringList typeNames;
    for (int i = 0; i < typeList.size(); ++i)
    {
        typeNames.append(typeList.at(i)->name);
    }
    typeNames.sort(Qt::CaseInsensitive);
    return typeNames;
}

void CardModel::loadCards()
{
    insertCards(0, getCards());
}

void CardModel::unloadCards()
{
    qDeleteAll(_cards);
    _cards.clear();
}

void CardModel::updatedImage(const QString &id)
{
    QVector<int> roles;
    roles << imageRole << thumbnailRole;

    for (int i = 0; i < rowCount(); ++i) {
        if (_cards.at(i)->id() == id) {
            emit dataChanged(index(i), index(i), roles);
            return;
        }
    }
}

void CardModel::init()
{
    QSqlQuery query;
    DBManager *db = DBManager::instance();

    query = db->prepare("SELECT id, strId, name, cycleId, numberInCycle FROM sets ORDER BY cycleId, numberInCycle;");
    db->execute(query);
    while (query.next())
    {
        Set* tmp = new Set(this);
        tmp->strId = query.value(1).toString();
        tmp->name = query.value(2).toString();
        tmp->cycleId = query.value(3).toInt();
        tmp->numberInCycle =  query.value(4).toInt();

        _sets.insert(query.value(0).toUInt(), tmp);
    }

    query = db->prepare("SELECT id, name FROM sides ORDER BY name;");
    db->execute(query);
    while (query.next())
    {
        Side* tmp = new Side(this);
        tmp->name = query.value(1).toString();
        _sides.insert(query.value(0).toUInt(), tmp);
    }

    query = db->prepare("SELECT id, name FROM factions ORDER BY name;");
    db->execute(query);
    while (query.next())
    {
        Faction* tmp = new Faction(this);
        tmp->name = query.value(1).toString();
        _factions.insert(query.value(0).toUInt(), tmp);
    }

    query = db->prepare("SELECT id, name FROM types ORDER BY name;");
    db->execute(query);
    while (query.next())
    {
        Type* tmp = new Type(this);
        tmp->name = query.value(1).toString();
        _types.insert(query.value(0).toUInt(), tmp);
    }
}

QList<Card *> CardModel::getCards()
{
    QSqlQuery query;
    DBManager *db = DBManager::instance();

    query = db->prepare(
                "SELECT strId, name, ruletext, numberInSet, cost, subtype,"
                " factioncost, strength, advancementCost, agendaPoints, memory,"
                " trashCost, link, minDeckSize, influenceLimit, limited, uniq,"
                " sideId, factionId, typeId, setId"
                " FROM cards;"
                );
    db->execute(query);

    QList<Card *> cards;
    while (query.next())
    {
        Card *card = new Card(this);
        card->setId(query.value(0).toString());
        card->setName(query.value(1).toString());
        card->setText(query.value(2).toString());
        card->setNumber(query.value(3).toInt());
        card->setCost(query.value(4).toInt());
        card->setSubtype(query.value(5).toString());
        card->setFactionCost(query.value(6).toInt());
        card->setStrength(query.value(7).toInt());
        card->setAdvancementCost(query.value(8).toInt());
        card->setAgendaPoints(query.value(9).toInt());
        card->setMemory(query.value(10).toInt());
        card->setTrashCost(query.value(11).toInt());
        card->setLink(query.value(12).toInt());
        card->setMinDeckSize(query.value(13).toInt());
        card->setInfluenceLimit(query.value(14).toInt());
        card->setLimited(query.value(15).toInt());
        card->setUnique(query.value(16).toInt());
        card->setSide(_sides.value(query.value(17).toUInt()));
        card->setFaction(_factions.value(query.value(18).toUInt()));
        card->setType(_types.value(query.value(19).toUInt()));
        card->setSet(_sets.value(query.value(20).toUInt()));

        cards.append(card);
    }

    return cards;
}

QList<Card *> CardModel::getCards(QStringList strIds)
{
    if (!strIds.size())
    {
        return QList<Card *>();
    }

    QSqlQuery query;
    DBManager *db = DBManager::instance();

    QString whereClause = QString(" WHERE strId IN ('%1') ").arg(strIds.join("', '"));

    query = db->prepare(QString(
                            "SELECT strId, name, ruletext, numberInSet, cost, subtype,"
                            " factioncost, strength, advancementCost, agendaPoints, memory,"
                            " trashCost, link, minDeckSize, influenceLimit, limited, uniq,"
                            " sideId, factionId, typeId, setId"
                            " FROM cards"
                            " %1;"
                            ).arg(whereClause));
    db->execute(query);

    QList<Card *> cards;
    while (query.next())
    {
        Card *card = new Card(this);
        card->setId(query.value(0).toString());
        card->setName(query.value(1).toString());
        card->setText(query.value(2).toString());
        card->setNumber(query.value(3).toInt());
        card->setCost(query.value(4).toInt());
        card->setSubtype(query.value(5).toString());
        card->setFactionCost(query.value(6).toInt());
        card->setStrength(query.value(7).toInt());
        card->setAdvancementCost(query.value(8).toInt());
        card->setAgendaPoints(query.value(9).toInt());
        card->setMemory(query.value(10).toInt());
        card->setTrashCost(query.value(11).toInt());
        card->setLink(query.value(12).toInt());
        card->setMinDeckSize(query.value(13).toInt());
        card->setInfluenceLimit(query.value(14).toInt());
        card->setLimited(query.value(15).toInt());
        card->setUnique(query.value(16).toInt());
        card->setSide(_sides.value(query.value(17).toUInt()));
        card->setFaction(_factions.value(query.value(18).toUInt()));
        card->setType(_types.value(query.value(19).toUInt()));
        card->setSet(_sets.value(query.value(20).toUInt()));

        cards.append(card);
    }

    return cards;
}

void CardModel::insertCards(const int row, const QList<Card *> cards)
{
    if (row < 0 || row > rowCount())
        return;
    if (cards.size() == 0)
        return;

    beginInsertRows(QModelIndex(), row, row + cards.size()-1);
    for (int i = 0; i < cards.size(); ++i)
    {
        _cards.insert(i + row, cards.at(i));
    }
    endInsertRows();
    emit countChanged();
}

