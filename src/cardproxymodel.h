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

#ifndef CARDPROXYMODEL_H
#define CARDPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QQmlParserStatus>
#include <QSettings>
#include <QtQuick>
#include <QRegExp>

#include "categoryfilter.h"
#include "numericfilter.h"

class CardModel;

class CardProxyModel : public QSortFilterProxyModel, public QQmlParserStatus
{
    Q_OBJECT

    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QObject* source READ source WRITE setSource)
    Q_PROPERTY(QString settingsSection READ settingsSection WRITE setSettingsSection NOTIFY settingsSectionChanged)

    Q_PROPERTY(QString stringFilter READ stringFilter WRITE setStringFilter NOTIFY stringFilterChanged)
    Q_PROPERTY(QRegExp stringRegExp READ stringRegExp NOTIFY stringRegExpChanged)
    Q_PROPERTY(QQmlListProperty<CategoryFilter> textFilter READ textFilter NOTIFY textFilterChanged)

    Q_PROPERTY(QQmlListProperty<CategoryFilter> typeFilter READ typeFilter NOTIFY typeFilterChanged)
    Q_PROPERTY(QQmlListProperty<CategoryFilter> sideFilter READ sideFilter NOTIFY sideFilterChanged)
    Q_PROPERTY(QQmlListProperty<CategoryFilter> factionFilter READ factionFilter NOTIFY factionFilterChanged)
    Q_PROPERTY(QQmlListProperty<CategoryFilter> setFilter READ setFilter NOTIFY setFilterChanged)
    Q_PROPERTY(QQmlListProperty<NumericFilter> statsFilter READ statsFilter NOTIFY statsFilterChanged)

    Q_PROPERTY(bool descendingSort READ descendingSort WRITE setDescendingSort NOTIFY descendingSortChanged)
    Q_PROPERTY(QString sortString READ sortString WRITE setSortString NOTIFY sortStringChanged)
    Q_PROPERTY(QStringList sortStrings READ sortStrings NOTIFY sortStringsChanged)

public:
    explicit CardProxyModel(QObject *parent = 0);

    // QQmlParserStatus interface
    void classBegin();
    void componentComplete();

    QObject* source() const;
    void setSource(QObject *source);

    QString settingsSection() const;
    void setSettingsSection(const QString &settingsSection);

    QString stringFilter() const;
    void setStringFilter(const QString &stringFilter);
    QRegExp stringRegExp() const;

    QQmlListProperty<CategoryFilter> textFilter();
    QQmlListProperty<CategoryFilter> typeFilter();
    QQmlListProperty<CategoryFilter> sideFilter();
    QQmlListProperty<CategoryFilter> factionFilter();
    QQmlListProperty<CategoryFilter> setFilter();
    QQmlListProperty<NumericFilter> statsFilter();

    bool descendingSort() const;
    void setDescendingSort(const bool descendingSort);

    QString sortString() const;
    void setSortString(const QString &sortString);

    QStringList sortStrings() const;

    QSettings* settings();

    Q_INVOKABLE void saveSettings();
    Q_INVOKABLE void resetFiltersAndSorting();

signals:
    void settingsSectionChanged();
    void textFilterChanged();
    void typeFilterChanged();
    void sideFilterChanged();
    void factionFilterChanged();
    void setFilterChanged();
    void stringFilterChanged();
    void stringRegExpChanged();
    void statsFilterChanged();
    void descendingSortChanged();
    void sortStringChanged();
    void sortStringsChanged();

public slots:
    void updateTextFilter();
    void updateTypeFilter();
    void updateSideFilter();
    void updateFactionFilter();
    void updateSetFilter();
    void updateStatsFilter();

protected:
    // QSortFilterProxyModel interface
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    CardModel *_cardmodel;
    QSettings _settings;

    QString _settingsSection;
    bool _autoInvalidateFilter;

    QString _stringFilter;
    QRegExp _stringRegExp;
    QList<CategoryFilter *> _textFilters;
    QList<CategoryFilter *> _typeFilters;
    QList<CategoryFilter *> _sideFilters;
    QList<CategoryFilter *> _factionFilters;
    QList<CategoryFilter *> _setFilters;
    QList<NumericFilter *> _statsFilters;
    bool _typeFilterOn;
    bool _sideFilterOn;
    bool _factionFilterOn;
    bool _setFilterOn;

    QMap<QString, int> _sortNameRoleMap;
    QStringList _sortList;
    QList<bool> _sortDescList;
    QStringList _sortDefaultList;
    QList<bool> _sortDefaultDescList;

    void loadTextFilter();
    void loadTypeFilter();
    void loadSideFilter();
    void loadFactionFilter();
    void loadSetFilter();
    void loadStatsFilters();

    void loadSortStrings();

    void loadSortList();
    void loadSortDescList();
    void saveSortList();
    void saveSortDescList();

    void saveCategoryFilters(QList<CategoryFilter *> filters);
    void resetCategoryFilters(QList<CategoryFilter *> filters);
    void saveNumericFilters(QList<NumericFilter *> filters);
    void resetNumericFilters(QList<NumericFilter *> filters);
};

#endif // CARDPROXYMODEL_H
