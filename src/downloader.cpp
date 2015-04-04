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

#include <QImage>
#include <QStringList>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "downloader.h"
#include "dbmanager.h"
#include "card.h"

Downloader::Downloader(QObject *parent) :
    QObject(parent), _busy(false), _totalImgs(0)
{
}

Downloader *Downloader::instance()
{
    static Downloader *downloader;
    if (!downloader)
    {
        downloader = new Downloader();
    }
    return downloader;
}

void Downloader::updateDB()
{
    if (_busy)
        return;
    setBusy(true);

    connect(&_manager, &DownloadManager::downloadItemFinished, this, &Downloader::foreignSetsDownloaded);
    _manager.append("http://netrunnerdb.com/api/sets/");
}

void Downloader::updateMissingImages()
{
    if (_busy)
        return;
    setBusy(true);

    QSqlQuery query;
    DBManager *db = DBManager::instance();

    query = db->prepare("SELECT strId, remoteImg FROM cards;");
    db->execute(query);

    QStringList imgUrls;
    while (query.next())
    {
        Card card;
        card.setId(query.value(0).toString());

        QString remoteImg = query.value(1).toString();
        QString imgFile = card.imageFile();

        QFileInfo fi(imgFile);
        if (!fi.absoluteDir().exists())
        {
            fi.absoluteDir().mkpath(fi.absolutePath());
        }

        if (remoteImg != "" && !fi.exists())
        {
                imgUrls.append(remoteImg);
                _filenames.enqueue(fi.absoluteFilePath());
                _cardIds.enqueue(card.id());
        }
    }

    connect(&_manager, &DownloadManager::downloadItemFinished, this, &Downloader::foreignImgDownloaded);
    connect(&_manager, &DownloadManager::finished, this, &Downloader::cleanUp);
    _totalImgs = imgUrls.size();
    _manager.append(imgUrls);
}

void Downloader::updateImage(const QString &id)
{
    if (_busy)
        return;
    setBusy(true);

    QSqlQuery query;
    DBManager *db = DBManager::instance();

    query = db->prepare("SELECT remoteImg FROM cards WHERE strId = ?;");
    query.bindValue(0, id);
    db->execute(query);

    if (query.next())
    {
        Card card;
        card.setId(id);

        QString remoteImg = query.value(0).toString();
        QString imgFile = card.imageFile();

        if (remoteImg !=  "")
        {
            connect(&_manager, &DownloadManager::downloadItemFinished, this, &Downloader::foreignImgDownloaded);
            connect(&_manager, &DownloadManager::finished, this, &Downloader::cleanUp);
            _totalImgs += 1;
            _filenames.enqueue(imgFile);
            _cardIds.enqueue(id);
            _manager.append(remoteImg);
        } else {
            setBusy(false);
        }
    }
}

bool Downloader::busy()
{
    return _busy;
}

void Downloader::foreignSetsDownloaded()
{
    QSqlQuery query;
    DBManager *db = DBManager::instance();

    QJsonArray sets = QJsonDocument::fromJson(_manager.downloadedData()).array();

    for (int i = 0; i < sets.size(); ++i)
    {
        QJsonObject set = sets.at(i).toObject();

        QString name = set["name"].toString();
        QString strId = set["code"].toString();
        int cycle = qRound(set["cyclenumber"].toDouble());
        int numberInCycle = qRound(set["number"].toDouble());
        int knownCards = qRound(set["known"].toDouble());
        int totalCards = qRound(set["total"].toDouble());

        query = db->prepare("INSERT OR IGNORE INTO sets ("
                            " strId, name, knownCards, totalCards,"
                            " cycleId, numberInCycle"
                            " ) VALUES (?, ?, ?, ?, ?, ?);");
        query.bindValue(0, strId);
        query.bindValue(1, name);
        query.bindValue(2, knownCards);
        query.bindValue(3, totalCards);
        query.bindValue(4, cycle);
        query.bindValue(5, numberInCycle);
        db->execute(query);
    }

    disconnect(&_manager, &DownloadManager::downloadItemFinished, this, 0);
    connect(&_manager, &DownloadManager::downloadItemFinished, this, &Downloader::foreignCardsDownloaded);
    _manager.append("http://netrunnerdb.com/api/cards/");
}

void Downloader::foreignCardsDownloaded()
{
    QSqlQuery query;
    DBManager *db = DBManager::instance();

    // get known sets
    query = db->prepare("SELECT id, strId FROM sets;");
    db->execute(query);

    QHash<QString, int> setIdMap;
    while (query.next())
    {
        setIdMap.insert(query.value(1).toString(), query.value(0).toInt());
    }

    // get last known modification string
    query = db->prepare("SELECT strId, lastModified FROM cards;");
    db->execute(query);

    QHash<QString, QString> idModifiedMap;
    while (query.next())
    {
        idModifiedMap.insert(query.value(0).toString(), query.value(1).toString());
    }

    // get known sides
    query = db->prepare("SELECT id, name FROM sides;");
    db->execute(query);

    QHash<QString, int> sideIdMap;
    while (query.next())
    {
        sideIdMap.insert(query.value(1).toString(), query.value(0).toInt());
    }

    // get known factions
    query = db->prepare("SELECT id, name FROM factions;");
    db->execute(query);

    QHash<QString, int> factionIdMap;
    while (query.next())
    {
        factionIdMap.insert(query.value(1).toString(), query.value(0).toInt());
    }

    // get known types
    query = db->prepare("SELECT id, name FROM types;");
    db->execute(query);

    QHash<QString, int> typeIdMap;
    while (query.next())
    {
        typeIdMap.insert(query.value(1).toString(), query.value(0).toInt());
    }

    // downloaded data
    QJsonArray cards = QJsonDocument::fromJson(_manager.downloadedData()).array();

    // update/insert cards
    for (int i = 0; i < cards.size(); ++i)
    {
        QJsonObject card = cards.at(i).toObject();

        QString strId;
        if (card.contains("code"))
            strId = card["code"].toString();
        else
            continue;

        QString lastModified;
        if (card.contains("last-modified"))
        {
            lastModified = card["last-modified"].toString();

            if (idModifiedMap.contains(strId))
            {
                if (lastModified != idModifiedMap[strId])
                {
                    Card card;
                    card.setId(strId);

                    // remove image
                    QFileInfo img(card.imageFile());
                    if (img.exists())
                    {
                        QFile::remove(img.absoluteFilePath());
                    }

                    //remove thumbnail
                    QFileInfo thumb(card.thumbnailFile());
                    if (thumb.exists())
                    {
                        QFile::remove(thumb.absoluteFilePath());
                    }

                    // remove db entry
                    query = db->prepare("DELETE FROM cards WHERE strId = ?;");
                    query.bindValue(0, strId);
                    db->execute(query);
                }
                else
                {
                    continue;
                }
            }
        }
        else
            continue;

        QString set;
        if (card.contains("set_code"))
            set = card["set_code"].toString();
        else
            continue;

        QString name;
        if (card.contains("title"))
            name = card["title"].toString();
        else
            continue;

        QString side;
        if (card.contains("side"))
            side = card["side"].toString();
        else
            side = "";

        QString faction;
        if (card.contains("faction"))
            faction = card["faction"].toString();
        else
            faction = "";

        QString type;
        if (card.contains("type"))
            type = card["type"].toString();
        else
            type = "";

        QString subtype;
        if (card.contains("subtype"))
            subtype = card["subtype"].toString();
        else
            subtype = "";

        QString ruletext;
        if (card.contains("text"))
        {
            ruletext = card["text"].toString();
        }
        else
            ruletext = "";

        int cost;
        if (card.contains("cost"))
            cost = qRound(card["cost"].toDouble());
        else
            cost = -1;

        int link;
        if (card.contains("baselink"))
            link = qRound(card["baselink"].toDouble());
        else
            link = -1;

        QString flavor;
        if (card.contains("flavor"))
            flavor = card["flavor"].toString();
        else
            flavor = "";

        int influenceLimit;
        if (card.contains("influencelimit"))
            influenceLimit = qRound(card["influencelimit"].toDouble());
        else
            influenceLimit = -1;

        int minDeckSize;
        if (card.contains("minimumdecksize"))
            minDeckSize = qRound(card["minimumdecksize"].toDouble());
        else
            minDeckSize = -1;

        int numberInSet;
        if (card.contains("number"))
            numberInSet = qRound(card["number"].toDouble());
        else
            numberInSet = -1;

        int factionCost;
        if (card.contains("factioncost"))
            factionCost = qRound(card["factioncost"].toDouble());
        else
            factionCost = -1;

        int uniq;
        if (card.contains("uniqueness"))
            uniq = card["uniqueness"].toBool() == false ? 0 : 1;
        else
            uniq = 0;

        int limited;
        if (card.contains("limited"))
            limited = card["limited"].toBool() == false ? 0 : 1;
        else
            limited = 0;

        QString remoteImg;
        if (card.contains("imagesrc"))
        {
            if (card["imagesrc"].toString() == "")
                remoteImg = "";
            else
                remoteImg = QString("%1%2").arg("http://netrunnerdb.com", card["imagesrc"].toString());
        }
        else
            remoteImg = "";

        int strength;
        if (card.contains("strength"))
            strength = qRound(card["strength"].toDouble());
        else
            strength = -1;

        int memory;
        if (card.contains("memoryunits"))
            memory = qRound(card["memoryunits"].toDouble());
        else
            memory = -1;

        int trashCost;
        if (card.contains("trash"))
            trashCost = qRound(card["trash"].toDouble());
        else
            trashCost = -1;

        int advancementCost;
        if (card.contains("advancementcost"))
            advancementCost = qRound(card["advancementcost"].toDouble());
        else
            advancementCost = -1;

        int agendaPoints;
        if (card.contains("agendapoints"))
            agendaPoints = qRound(card["agendapoints"].toDouble());
        else
            agendaPoints = -1;

        if (!sideIdMap.contains(side))
        {
            query = db->prepare("INSERT INTO sides (name) VALUES (?);");
            query.bindValue(0, side);
            db->execute(query);

            QVariant lastId = query.lastInsertId();
            if (lastId.isValid())
            {
                sideIdMap.insert(side, lastId.toInt());
            }
        }

        if (!factionIdMap.contains(faction))
        {
            query = db->prepare("INSERT INTO factions (name) VALUES (?);");
            query.bindValue(0, faction);
            db->execute(query);

            QVariant lastId = query.lastInsertId();
            if (lastId.isValid())
            {
                factionIdMap.insert(faction, lastId.toInt());
            }
        }

        if (!typeIdMap.contains(type))
        {
            query = db->prepare("INSERT INTO types (name) VALUES (?);");
            query.bindValue(0, type);
            db->execute(query);

            QVariant lastId = query.lastInsertId();
            if (lastId.isValid())
            {
                typeIdMap.insert(type, lastId.toInt());
            }
        }

        query = db->prepare(
                    "INSERT INTO cards ("
                    " strId, lastModified, name, ruletext, numberInSet, cost, subtype, factionCost,"
                    " strength, advancementCost, agendaPoints, memory, trashCost, link, minDeckSize,"
                    " influenceLimit, limited, uniq, remoteImg,"
                    " sideId, factionId, typeId, setId"
                    " ) VALUES ("
                    " :strId, :lastModified, :name, :ruletext, :numberInSet, :cost, :subtype, :factionCost,"
                    " :strength, :advancementCost, :agendaPoints, :memory, :trashCost, :link, :minDeckSize,"
                    " :influenceLimit, :limited, :uniq, :remoteImg,"
                    " :sideId, :factionId, :typeId, :setId"
                    " );");

        query.bindValue(":strId", strId);
        query.bindValue(":lastModified", lastModified);
        query.bindValue(":name", name);
        query.bindValue(":ruletext", ruletext);
        query.bindValue(":numberInSet", numberInSet);
        query.bindValue(":cost", cost);
        query.bindValue(":subtype", subtype);
        query.bindValue(":factionCost", factionCost);
        query.bindValue(":strength", strength);
        query.bindValue(":advancementCost", advancementCost);
        query.bindValue(":agendaPoints", agendaPoints);
        query.bindValue(":memory", memory);
        query.bindValue(":trashCost", trashCost);
        query.bindValue(":link", link);
        query.bindValue(":minDeckSize", minDeckSize);
        query.bindValue(":influenceLimit", influenceLimit);
        query.bindValue(":limited", limited);
        query.bindValue(":uniq", uniq);
        query.bindValue(":remoteImg", remoteImg);
        query.bindValue(":sideId", sideIdMap.value(side));
        query.bindValue(":factionId", factionIdMap.value(faction));
        query.bindValue(":typeId", typeIdMap.value(type));
        query.bindValue(":setId", setIdMap.value(set));
        db->execute(query);
    }

    cleanUp();
}

void Downloader::foreignImgDownloaded()
{
    QImage img = QImage::fromData(_manager.downloadedData());
    img.save(_filenames.dequeue());

    emit updatedImage(_cardIds.dequeue());

    float value = _totalImgs - _filenames.size();
    value /= _totalImgs;
    emit downloadImgProgress(value);
}

void Downloader::cleanUp()
{
    disconnect(&_manager, &DownloadManager::downloadItemFinished, this, 0);
    disconnect(&_manager, &DownloadManager::finished, this, 0);
    _filenames.clear();
    _cardIds.clear();
    _totalImgs = 0;

    emit finished();
    setBusy(false);
}

void Downloader::setBusy(bool busy)
{
    if (busy == _busy)
        return;

    _busy = busy;
    emit busyChanged();
}
