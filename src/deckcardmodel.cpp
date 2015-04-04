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

#include "deckcardmodel.h"

#include <QXmlStreamWriter>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

#include "dbmanager.h"

DeckCardModel::DeckCardModel(QObject *parent):
    CountCardModel(parent), _deckId(0), _identity(nullptr)
{
    connect(this, &DeckCardModel::totalCountChanged, this, &DeckCardModel::updateDeck);
}

DeckCardModel::DeckCardModel(uint deckId, QObject *parent) :
    CountCardModel(parent), _deckId(deckId), _identity(nullptr)
{
    connect(this, &DeckCardModel::totalCountChanged, this, &DeckCardModel::updateDeck);

    updateIdentity();

    QSqlQuery query;
    DBManager *db = DBManager::instance();

    query = db->prepare("SELECT name FROM decks WHERE id = ?;");
    query.bindValue(0, _deckId);
    db->execute(query);

    query.next();
    _name = query.value(0).toString();
}

uint DeckCardModel::deckId() const
{
    return _deckId;
}

QString DeckCardModel::deckName() const
{
    return _name;
}

void DeckCardModel::setDeckName(const QString &name)
{
    if (name == _name)
        return;

    QSqlQuery query;
    DBManager *db = DBManager::instance();

    query = db->prepare("UPDATE decks SET name = ? WHERE id = ?;");
    query.bindValue(0, name);
    query.bindValue(1, _deckId);
    db->execute(query);

    _name = name;
    emit deckNameChanged();
}

QString DeckCardModel::identityName() const
{
    if (_identity)
    {
        return _identity->name();
    }
    return "";
}

QString DeckCardModel::identitySide() const
{
    if (_identity)
    {
        return _identity->side()->name;
    }
    return "";
}

QString DeckCardModel::identityFaction() const
{
    if (_identity)
    {
        return _identity->faction()->name;
    }
    return "";
}

QString DeckCardModel::identityThumbnail() const
{
    if (_identity)
    {
        return _identity->thumbnail();
    }
    return "";
}

bool DeckCardModel::identityError() const
{
    return _identityError;
}

uint DeckCardModel::minDeckSize() const
{
    if (_identity)
    {
        return _identity->minDeckSize();
    }
    return 0;
}

uint DeckCardModel::maxInfluence() const
{
    if (_identity)
    {
        return _identity->influenceLimit();
    }
    return 0;
}

uint DeckCardModel::deckSize() const
{
    return _deckSize;
}

uint DeckCardModel::usedInfluence() const
{
    return _usedInfluence;
}

uint DeckCardModel::usedAgendaPoints() const
{
    return _usedAgendaPoints;
}

QVariantMap DeckCardModel::agendaPointsRange() const
{
    QVariantMap range;
    range.insert("min", _minAgendaPoints);
    range.insert("max", _minAgendaPoints+1);

    return range;
}

QStringList DeckCardModel::includeError() const
{
    QStringList includeError;
    foreach (Card *card, _includeError)
    {
        includeError.append(card->name());
    }
    return includeError;
}

QStringList DeckCardModel::limitError() const
{
    QStringList limitError;
    foreach (Card *card, _limitError)
    {
        limitError.append(card->name());
    }
    return limitError;
}

void DeckCardModel::loadCards()
{
    QSqlQuery query = DBManager::instance()->prepare(
                "SELECT strId, count FROM deckCards WHERE deckId = ?;");
    query.bindValue(0, _deckId);
    DBManager::instance()->execute(query);

    QStringList ids;
    while (query.next())
    {
        QString id = query.value(0).toString();
        _count.insert(id, query.value(1).toInt());
        ids.append(id);
    }

    insertCards(0, getCards(ids));

    CountCardModel::loadCards();
}

void DeckCardModel::exportOCTGN()
{
    QSqlQuery query;
    DBManager *db = DBManager::instance();

    QDir deckdir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/anr-decks/";

    if (!deckdir.exists())
    {
        deckdir.mkpath(deckdir.absolutePath());
    }

    QFile file(deckdir.absoluteFilePath(deckdir.absoluteFilePath(_name + ".o8d")));

    file.open(QFile::WriteOnly);

    QXmlStreamWriter wr(&file);
    wr.setAutoFormatting(true);
    wr.writeStartDocument();

    wr.writeStartElement("deck");
    wr.writeAttribute("game", ANR_OCTGNID);

    wr.writeStartElement("section");
    wr.writeAttribute("name", "Identity");
    wr.writeAttribute("shared", "False");

    query = db->prepare(
                "SELECT cards.strId, deckCards.count, cards.name FROM cards"
                " LEFT JOIN types ON cards.typeId = types.id"
                " LEFT JOIN deckCards ON cards.strId = deckCards.strId"
                " WHERE types.name = 'Identity' and deckCards.deckId = ?;");
    query.bindValue(0, _deckId);
    db->execute(query);

    while (query.next())
    {
        QString strId = query.value(0).toString();
        QString count = query.value(1).toString();
        QString name = query.value(2).toString();

        wr.writeStartElement("card");
        wr.writeAttribute("qty", count);
        wr.writeAttribute("id", "bc0f047c-01b1-427f-a439-d451eda" + strId);
        wr.writeCharacters(name);
        wr.writeEndElement();
    }

    wr.writeEndElement(); // section

    wr.writeStartElement("section");
    wr.writeAttribute("name", "R&amp;D / Stack");
    wr.writeAttribute("shared", "False");

    query = db->prepare(
                "SELECT cards.strId, deckCards.count, cards.name FROM cards"
                " LEFT JOIN types ON cards.typeId = types.id"
                " LEFT JOIN deckCards ON cards.strId = deckCards.strId"
                " WHERE types.name != 'Identity' and deckCards.deckId = ?;");
    query.bindValue(0, _deckId);
    db->execute(query);

    while (query.next())
    {
        QString strId = query.value(0).toString();
        QString count = query.value(1).toString();
        QString name = query.value(2).toString();

        wr.writeStartElement("card");
        wr.writeAttribute("qty", count);
        wr.writeAttribute("id", "bc0f047c-01b1-427f-a439-d451eda" + strId);
        wr.writeCharacters(name);
        wr.writeEndElement();
    }

    wr.writeEndElement(); // section
    wr.writeEndElement(); // deck

    wr.writeEndDocument();

    file.close();
}

void DeckCardModel::updateDeck()
{
    bool identityError = false;

    updateIdentity();

    if (!_identity)
    {
        identityError = true;
    }

    if (!identityError && (_count.value(_identity->id(), 0) != 1))
    {
        identityError = true;
    }

    if (identityError != _identityError)
    {
        _identityError = identityError;
        emit identityErrorChanged();
    }

    uint deckSize = 0;
    uint usedInfluence = 0;
    uint usedAgendaPoints = 0;
    QList<Card *> includeError;
    QList<Card *> limitError;
    uint minAgendaPoints = 0;

    if (!_identityError)
    {
        for (int i = 0; i < _cards.size(); ++i)
        {
            Card *card = _cards.at(i);
            int count = _count.value(card->id(), 0);

            if (card->type()->name != "Identity")
            {
                deckSize += count;

                if (card->side() != _identity->side())
                {
                    includeError.append(card);
                }
                else if (_identity->faction() != card->faction())
                {
                    if (card->factionCost() < 0)
                    {
                        includeError.append(card);
                    }
                    else
                    {
                        usedInfluence += count * card->factionCost();
                    }
                }

                if (card->limited() == 1)
                {
                    if (count > 1)
                    {
                        limitError.append(card);
                    }
                }
                else
                {
                    if (count > 3)
                    {
                        limitError.append(card);
                    }
                }

                if (card->type()->name == "Agenda")
                {
                    usedAgendaPoints += count * card->agendaPoints();
                }
            }
        }

        if (_identity->side()->name == "Corp")
        {
            uint refsize = deckSize < minDeckSize() ? minDeckSize() : deckSize;

            minAgendaPoints = (refsize/5)*2 + 2;
        }
    }

    if (deckSize != _deckSize)
    {
        _deckSize = deckSize;
        emit deckSizeChanged();
    }
    if (usedInfluence != _usedInfluence)
    {
        _usedInfluence = usedInfluence;
        emit usedInfluenceChanged();
    }
    if (usedAgendaPoints != _usedAgendaPoints)
    {
        _usedAgendaPoints = usedAgendaPoints;
        emit usedAgendaPointsChanged();
    }
    if (minAgendaPoints != _minAgendaPoints)
    {
        _minAgendaPoints = minAgendaPoints;
        emit agendaPointsRangeChanged();
    }
    if (includeError.size() != _includeError.size())
    {
        _includeError = includeError;
        emit includeErrorChanged();
    }
    if (limitError.size() != _limitError.size())
    {
        _limitError = limitError;
        emit limitErrorChanged();
    }
}

void DeckCardModel::updateIdentity()
{
    QSqlQuery query;
    DBManager *db = DBManager::instance();

    query = db->prepare(
                "SELECT cards.strId FROM deckCards"
                " LEFT JOIN cards ON deckCards.strId = cards.StrId"
                " LEFT JOIN types ON cards.typeId = types.id"
                " WHERE types.name = 'Identity' AND deckCards.deckId = ?;");
    query.bindValue(0, _deckId);
    db->execute(query);

    QStringList identityIds;
    while (query.next())
    {
        identityIds.append(query.value(0).toString());
    }

    if (identityIds.size() == 1)
    {
        if ((_identity == nullptr) || (_identity->id() != identityIds.first()))
        {
            _identity = getCards(identityIds).first();
            emit identityChanged();
        }
    }
    else
    {
        if (_identity != nullptr)
        {
            delete _identity;
            _identity = nullptr;
            emit identityChanged();
        }
    }
}

void DeckCardModel::dbUpdateCount(const QString &strId)
{
    QSqlQuery query = DBManager::instance()->prepare(
                "UPDATE deckCards SET count = ? WHERE deckId = ? AND strId = ?;");
    query.bindValue(0, _count.value(strId, 1));
    query.bindValue(1, _deckId);
    query.bindValue(2, strId);
    DBManager::instance()->execute(query);
}

void DeckCardModel::dbInsertCard(const QString &strId)
{
    QSqlQuery query = DBManager::instance()->prepare(
                "INSERT INTO deckCards (deckId, strId, count) VALUES (?, ?, ?)");
    query.bindValue(0, _deckId);
    query.bindValue(1, strId);
    query.bindValue(2, _count.value(strId, 1));
    DBManager::instance()->execute(query);
}

void DeckCardModel::dbDeleteCard(const QString &strId)
{
    QSqlQuery query = DBManager::instance()->prepare(
                "DELETE FROM deckCards WHERE deckId = ? AND strId = ?;");
    query.bindValue(0, _deckId);
    query.bindValue(1, strId);
    DBManager::instance()->execute(query);
}
