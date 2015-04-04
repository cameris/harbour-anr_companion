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

#ifndef DECKCARDMODEL_H
#define DECKCARDMODEL_H

#include "countcardmodel.h"

class DeckCardModel : public CountCardModel
{
    Q_OBJECT

    Q_PROPERTY(uint deckId READ deckId NOTIFY deckIdChanged)
    Q_PROPERTY(QString deckName READ deckName WRITE setDeckName NOTIFY deckNameChanged)

    Q_PROPERTY(QString identityName READ identityName NOTIFY identityChanged)
    Q_PROPERTY(QString identitySide READ identitySide NOTIFY identityChanged)
    Q_PROPERTY(QString identityFaction READ identityFaction NOTIFY identityChanged)
    Q_PROPERTY(QString identityThumbnail READ identityThumbnail NOTIFY identityChanged)
    Q_PROPERTY(uint minDeckSize READ minDeckSize NOTIFY identityChanged)
    Q_PROPERTY(uint maxInfluence READ maxInfluence NOTIFY identityChanged)

    Q_PROPERTY(bool identityError READ identityError NOTIFY identityErrorChanged)
    Q_PROPERTY(uint deckSize READ deckSize NOTIFY deckSizeChanged)
    Q_PROPERTY(uint usedInfluence READ usedInfluence NOTIFY usedInfluenceChanged)
    Q_PROPERTY(uint usedAgendaPoints READ usedAgendaPoints NOTIFY usedAgendaPointsChanged)
    Q_PROPERTY(QVariantMap agendaPointsRange READ agendaPointsRange NOTIFY agendaPointsRangeChanged)
    Q_PROPERTY(QStringList includeError READ includeError NOTIFY includeErrorChanged)
    Q_PROPERTY(QStringList limitError READ limitError NOTIFY limitErrorChanged)

public:
    explicit DeckCardModel(QObject *parent = 0);
    explicit DeckCardModel(uint deckId, QObject *parent = 0);

    uint deckId() const;

    QString deckName() const;
    void setDeckName(const QString &name);

    QString identityName() const;
    QString identitySide() const;
    QString identityFaction() const;
    QString identityThumbnail() const;
    uint minDeckSize() const;
    uint maxInfluence() const;

    bool identityError() const;
    uint deckSize() const;
    uint usedInfluence() const;
    uint usedAgendaPoints() const;
    QVariantMap agendaPointsRange() const;
    QStringList includeError() const;
    QStringList limitError() const;

    Q_INVOKABLE void loadCards();

    void exportOCTGN();

signals:
    void deckIdChanged();
    void deckNameChanged();

    void identityChanged();

    void identityErrorChanged();
    void deckSizeChanged();
    void usedInfluenceChanged();
    void usedAgendaPointsChanged();
    void agendaPointsRangeChanged();
    void includeErrorChanged();
    void limitErrorChanged();

public slots:
    void updateDeck();

private:
    uint _deckId;
    QString _name;

    Card* _identity;
    bool _identityError;
    uint _deckSize;
    uint _usedInfluence;
    uint _usedAgendaPoints;
    uint _minAgendaPoints;
    QList<Card *> _limitError;
    QList<Card *> _includeError;

    void updateIdentity();
    void dbUpdateCount(const QString &strId);
    void dbInsertCard(const QString &strId);
    void dbDeleteCard(const QString &strId);
};

#endif // DECKCARDMODEL_H
