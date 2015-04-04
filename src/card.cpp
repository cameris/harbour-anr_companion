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

#include "card.h"

#include <QSettings>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QImageReader>

#define THUMB_WIDTH 180
#define THUMB_HEIGHT 252

Card::Card(QObject *parent) :
    QObject(parent), _id(""), _name(""), _number(-1), _side(nullptr), _faction(nullptr),
    _type(nullptr), _subtype(""), _factionCost(-1), _cost(-1), _strength(-1),
    _advancementCost(-1), _agendaPoints(-1), _memory(-1), _trashCost(-1), _link(-1),
    _minDeckSize(-1), _influenceLimit(-1), _limited(0), _unique(0), _text(""),
    _set(nullptr)
{
}

QString Card::id() const
{
    return _id;
}

void Card::setId(const QString &id)
{
    _id = id;
}

QString Card::name() const
{
    return _name;
}

void Card::setName(const QString &name)
{
    _name = name;
}

int Card::number() const
{
    return _number;
}

void Card::setNumber(int number)
{
    _number = number;
}

Side *Card::side() const
{
    return _side;
}

void Card::setSide(Side* side)
{
    _side = side;
}

Faction *Card::faction() const
{
    return _faction;
}

void Card::setFaction(Faction* faction)
{
    _faction = faction;
}

Type* Card::type() const
{
    return _type;
}

void Card::setType(Type* type)
{
    _type = type;
}

QString Card::subtype() const
{
    return _subtype;
}

void Card::setSubtype(const QString &subtype)
{
    _subtype = subtype;
}

int Card::factionCost() const
{
    return _factionCost;
}

void Card::setFactionCost(int factionCost)
{
    _factionCost = factionCost;
}

int Card::cost() const
{
    return _cost;
}

void Card::setCost(int cost)
{
    _cost = cost;
}
int Card::strength() const
{
    return _strength;
}

void Card::setStrength(int strength)
{
    _strength = strength;
}

int Card::advancementCost() const
{
    return _advancementCost;
}

void Card::setAdvancementCost(int advancementCost)
{
    _advancementCost = advancementCost;
}

int Card::agendaPoints() const
{
    return _agendaPoints;
}

void Card::setAgendaPoints(int agendaPoints)
{
    _agendaPoints = agendaPoints;
}
int Card::memory() const
{
    return _memory;
}

void Card::setMemory(int memory)
{
    _memory = memory;
}
int Card::trashCost() const
{
    return _trashCost;
}

void Card::setTrashCost(int trashCost)
{
    _trashCost = trashCost;
}
int Card::link() const
{
    return _link;
}

void Card::setLink(int link)
{
    _link = link;
}
int Card::minDeckSize() const
{
    return _minDeckSize;
}

void Card::setMinDeckSize(int minDeckSize)
{
    _minDeckSize = minDeckSize;
}
int Card::influenceLimit() const
{
    return _influenceLimit;
}

void Card::setInfluenceLimit(int influenceLimit)
{
    _influenceLimit = influenceLimit;
}
int Card::limited() const
{
    return _limited;
}

void Card::setLimited(int limited)
{
    _limited = limited;
}

int Card::unique() const
{
    return _unique;
}

void Card::setUnique(int unique)
{
    _unique = unique;
}

QString Card::text() const
{
    return _text;
}

void Card::setText(const QString &text)
{
    _text = text;
}
Set *Card::set() const
{
    return _set;
}

void Card::setSet(Set *set)
{
    _set = set;
}

QString Card::image() const
{
    if (_id == "")
    {
        return "";
    }

    QFileInfo img(imageFile());
    if (img.exists())
    {
        return img.absoluteFilePath();
    }
    return "";
}

QString Card::imageFile() const
{
    QSettings settings;
    QDir imagesDir(settings.value("cards/images").toString());
    QString image = QString("%1.png").arg(_id);
    return imagesDir.absoluteFilePath(image);
}

QString Card::thumbnail() const
{
    if (_id == "")
    {
        return "";
    }

    QFileInfo thumb(thumbnailFile());
    QFileInfo image(imageFile());

    if (!thumb.absoluteDir().exists())
    {
        thumb.absoluteDir().mkpath(thumb.absolutePath());
    }

    if (thumb.exists())
    {
        return thumb.absoluteFilePath();
    }

    if (!image.absoluteDir().exists())
    {
        return "";
    }

    // taken from https://github.com/nemomobile/nemo-qml-plugin-thumbnailer
    QImage thumbnail;
    QSize originalSize;
    QSize thumbSize(THUMB_WIDTH, THUMB_HEIGHT);

    QImageReader imageReader(image.absoluteFilePath());
    if (!imageReader.canRead())
    {
        return "";
    }

    originalSize = imageReader.size();

    if (originalSize != thumbSize && originalSize.isValid())
    {
        QSize scaledSize(originalSize);
        scaledSize.scale(thumbSize, Qt::KeepAspectRatioByExpanding);

        QPoint center((scaledSize.width()-1)/2, (scaledSize.height()-1)/2);
        QRect cr(0, 0, thumbSize.width(), thumbSize.height());
        cr.moveCenter(center);
        imageReader.setScaledClipRect(cr);
        imageReader.setScaledSize(scaledSize);
    }
    thumbnail = imageReader.read();
    thumbnail.save(thumb.absoluteFilePath(),"JPG");

    return thumb.absoluteFilePath();
}

QString Card::thumbnailFile() const
{
    QSettings settings;
    QDir thumbsDir(settings.value("cards/images").toString() + "/thumbnails");
    QString thumb = QString("%1.jpg").arg(_id);
    return thumbsDir.absoluteFilePath(thumb);
}














