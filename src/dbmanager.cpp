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

#include "dbmanager.h"

#include <QSqlError>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlDriver>

#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>

#include <QDebug>

DBManager *DBManager::instance()
{
    static DBManager *dbManager;
    if (!dbManager)
    {
        dbManager = new DBManager();
    }
    return dbManager;
}

DBManager::DBManager(QObject *parent) :
    QObject(parent)
{
    QDir dbDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    const QString dbFile = "data.sqlite";

    if (!dbDir.mkpath(dbDir.absolutePath()))
    {
        qWarning() << "Can't create directory " + dbDir.absolutePath();
    }

    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(dbDir.absoluteFilePath(dbFile));

    bool dbCreated = QFileInfo(dbDir.absoluteFilePath(dbFile)).exists();

    if (!_db.open())
    {
        qWarning() << "Failed to open user database " << dbDir.absoluteFilePath(dbFile);
        return;
    }

    QSqlQuery query;
    query = prepare("PRAGMA foreign_keys = ON;");
    execute(query);

    if (!dbCreated)
    {
        query = prepare(
                    "CREATE TABLE sets ("
                    "   id INTEGER PRIMARY KEY,"
                    "   strId TEXT UNIQUE,"
                    "   name TEXT,"
                    "   knownCards INTEGER,"
                    "   totalCards INTEGER,"
                    "   cycleId INTEGER,"
                    "   numberInCycle INTEGER"
                    "   );");
        execute(query);

        query = prepare(
                    "CREATE TABLE sides ("
                    "   id INTEGER PRIMARY KEY,"
                    "   name TEXT"
                    "   );");
        execute(query);

        query = prepare(
                    "CREATE TABLE factions ("
                    "   id INTEGER PRIMARY KEY,"
                    "   name TEXT"
                    "   );");
        execute(query);

        query = prepare(
                    "CREATE TABLE types ("
                    "   id INTEGER PRIMARY KEY,"
                    "   name TEXT"
                    "   );");
        execute(query);

        query = prepare(
                    "CREATE TABLE cards ("
                    "   id INTEGER PRIMARY KEY,"
                    "   strId TEXT,"
                    "   lastModified TEXT,"
                    "   name TEXT,"
                    "   ruletext TEXT,"
                    "   numberInSet INTEGER,"
                    "   cost INTEGER,"
                    "   subtype TEXT,"
                    "   factionCost INTEGER,"
                    "   strength INTEGER,"
                    "   advancementCost INTEGER,"
                    "   agendaPoints INTEGER,"
                    "   memory INTEGER,"
                    "   trashCost INTEGER,"
                    "   link INTEGER,"
                    "   minDeckSize INTEGER,"
                    "   influenceLimit INTEGER,"
                    "   limited INTEGER,"
                    "   uniq INTEGER,"
                    "   remoteImg TEXT,"
                    "   sideId INTEGER,"
                    "   factionId INTEGER,"
                    "   typeId INTEGER,"
                    "   setId INTEGER"
                    "   );");
        execute(query);

        query = prepare(
                    "CREATE TABLE decks ("
                    "   id INTEGER PRIMARY KEY,"
                    "   name TEXT"
                    "   );");
        execute(query);

        query = prepare(
                    "CREATE TABLE deckCards ("
                    "   id INTEGER PRIMARY KEY,"
                    "   deckId INTEGER NOT NULL REFERENCES decks(id) ON DELETE CASCADE,"
                    "   strId TEXT NOT NULL,"
                    "   count INTEGER,"
                    "   UNIQUE(deckId, strId)"
                    "   );");
        execute(query);
    }
}

QSqlQuery DBManager::prepare(const QString &statement)
{
    QSqlQuery query;
    query.setForwardOnly(true);
    if (!query.prepare(statement))
    {
        qWarning() << Q_FUNC_INFO << "failed to prepare query";
        qWarning() << query.lastQuery();
        qWarning() << query.lastError();
        return QSqlQuery();
    }
    return query;
}

bool DBManager::execute(QSqlQuery &query)
{
    if (!query.exec())
    {
        qWarning() << Q_FUNC_INFO << "failed to execute query";
        qWarning() << query.lastQuery();
        qWarning() << query.lastError();
        return false;
    }
    return true;
}
