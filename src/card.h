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

#ifndef CARD_H
#define CARD_H

#include <QObject>
#include <QStringList>

class Set : public QObject
{
    Q_OBJECT
public:
    explicit Set(QObject *parent = 0) : QObject(parent) {}

    QString strId;
    QString name;
    int cycleId;
    int numberInCycle;
};

class Side : public QObject
{
    Q_OBJECT
public:
    explicit Side(QObject *parent = 0) : QObject(parent) {}

    QString name;
};

class Type : public QObject
{
    Q_OBJECT
public:
    explicit Type(QObject *parent = 0) : QObject(parent) {}

    QString name;
};

class Faction : public QObject
{
    Q_OBJECT
public:
    explicit Faction(QObject *parent = 0) : QObject(parent) {}

    QString name;
};

class Card : public QObject
{
    Q_OBJECT
public:
    explicit Card(QObject *parent = 0);

    QString id() const;
    void setId(const QString &id);

    QString name() const;
    void setName(const QString &name);

    int number() const;
    void setNumber(int number);

    Side* side() const;
    void setSide(Side *side);

    Faction* faction() const;
    void setFaction(Faction *faction);

    Type* type() const;
    void setType(Type *type);

    QString subtype() const;
    void setSubtype(const QString &subtype);

    int factionCost() const;
    void setFactionCost(int factionCost);

    int cost() const;
    void setCost(int cost);

    int strength() const;
    void setStrength(int strength);

    int advancementCost() const;
    void setAdvancementCost(int advancementCost);

    int agendaPoints() const;
    void setAgendaPoints(int agendaPoints);

    int memory() const;
    void setMemory(int memory);

    int trashCost() const;
    void setTrashCost(int trashCost);

    int link() const;
    void setLink(int link);

    int minDeckSize() const;
    void setMinDeckSize(int minDeckSize);

    int influenceLimit() const;
    void setInfluenceLimit(int influenceLimit);

    int limited() const;
    void setLimited(int limited);

    int unique() const;
    void setUnique(int unique);

    QString text() const;
    void setText(const QString &text);

    Set *set() const;
    void setSet(Set *set);

    QString image() const;
    QString imageFile() const;
    QString thumbnail() const;
    QString thumbnailFile() const;

private:
    QString _id;
    QString _name;
    int _number;
    Side* _side;
    Faction* _faction;
    Type* _type;
    QString _subtype;
    int _factionCost;
    int _cost;
    int _strength;
    int _advancementCost;
    int _agendaPoints;
    int _memory;
    int _trashCost;
    int _link;
    int _minDeckSize;
    int _influenceLimit;
    int _limited;
    int _unique;
    QString _text;
    Set* _set;
};

#endif // CARD_H
