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

#include "cardproxymodel.h"

#include "cardmodel.h"

CardProxyModel::CardProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), _settingsSection("search"), _autoInvalidateFilter(true),
    _stringFilter(""), _typeFilterOn(false), _sideFilterOn(false), _factionFilterOn(false),
    _setFilterOn(false)
{
    setDynamicSortFilter(true);

    _sortDefaultList << "Side" << "Faction" << "Type" << "Name";
    _sortDefaultDescList << false << false << false << false;

    _sortList = _sortDefaultList;
    _sortDescList = _sortDefaultDescList;

    _stringRegExp.setCaseSensitivity(Qt::CaseInsensitive);
    _stringRegExp.setPatternSyntax(QRegExp::Wildcard);
}

QString stringToRegExp(const QString &string)
{
    if (string.length() <= 0)
        return "";
    QString tmp = string;
    tmp.replace('a', "[a4]", Qt::CaseInsensitive);
    tmp.replace('b', "[b68]", Qt::CaseInsensitive);
    tmp.replace('e', "[e3]", Qt::CaseInsensitive);
    tmp.replace('g', "[g9]", Qt::CaseInsensitive);
    tmp.replace('i', "[i1ï]", Qt::CaseInsensitive);
    tmp.replace('l', "[l1]", Qt::CaseInsensitive);
    tmp.replace('o', "[o0]", Qt::CaseInsensitive);
    tmp.replace('s', "[s5]", Qt::CaseInsensitive);
    tmp.replace('t', "[t7]", Qt::CaseInsensitive);

    return tmp;
}

void CardProxyModel::classBegin()
{
}

void CardProxyModel::componentComplete()
{
    _autoInvalidateFilter = false;

    loadTextFilter();
    loadTypeFilter();
    loadSideFilter();
    loadFactionFilter();
    loadSetFilter();
    loadStatsFilters();

    loadSortStrings();

    setStringFilter(_settings.value(_settingsSection + "/string", "").toString());

    _autoInvalidateFilter = true;

    invalidateFilter();

    loadSortList();
    loadSortDescList();
    sort(0);
}

QObject *CardProxyModel::source() const
{
    return sourceModel();
}

void CardProxyModel::setSource(QObject *source)
{
    if (!source)
        return;

    _cardmodel = static_cast<CardModel *>(source);
    setSourceModel(qobject_cast<QAbstractItemModel *>(source));
}

QString CardProxyModel::settingsSection() const
{
    return _settingsSection;
}

void CardProxyModel::setSettingsSection(const QString &settingsSection)
{
    if (settingsSection == _settingsSection)
        return;
    _settingsSection = settingsSection;
    emit settingsSectionChanged();
}

QString CardProxyModel::stringFilter() const
{
    return _stringFilter;
}

void CardProxyModel::setStringFilter(const QString &stringFilter)
{
    if (stringFilter == _stringFilter)
        return;

    _stringFilter = stringFilter;
    _stringRegExp.setPattern(stringToRegExp(stringFilter));

    emit stringFilterChanged();
    emit stringRegExpChanged();

    if (_autoInvalidateFilter)
        invalidateFilter();
}

QRegExp CardProxyModel::stringRegExp() const
{
    return _stringRegExp;
}

QQmlListProperty<CategoryFilter> CardProxyModel::textFilter()
{
    return QQmlListProperty<CategoryFilter>(this, _textFilters);
}

QQmlListProperty<CategoryFilter> CardProxyModel::typeFilter()
{
    return QQmlListProperty<CategoryFilter>(this, _typeFilters);
}

QQmlListProperty<CategoryFilter> CardProxyModel::sideFilter()
{
    return QQmlListProperty<CategoryFilter>(this, _sideFilters);
}

QQmlListProperty<CategoryFilter> CardProxyModel::factionFilter()
{
    return QQmlListProperty<CategoryFilter>(this, _factionFilters);
}

QQmlListProperty<CategoryFilter> CardProxyModel::setFilter()
{
    return QQmlListProperty<CategoryFilter>(this, _setFilters);
}

QQmlListProperty<NumericFilter> CardProxyModel::statsFilter()
{
    return QQmlListProperty<NumericFilter>(this, _statsFilters);
}

bool CardProxyModel::descendingSort() const
{
    return _sortDescList.first();
}

void CardProxyModel::setDescendingSort(bool descendingSort)
{
    if (descendingSort == _sortDescList.first())
        return;

    _sortDescList.replace(0, descendingSort);

    setDynamicSortFilter(false);
    sort(0);
    setDynamicSortFilter(true);
    emit descendingSortChanged();
}

QString CardProxyModel::sortString() const
{
    return _sortList.first();
}

void CardProxyModel::setSortString(const QString &sortString)
{
    if (sortString == _sortList.first())
        return;

    _sortList.insert(0, sortString);
    _sortList.removeLast();
    _sortDescList.insert(0, _sortDescList.first());
    _sortDescList.removeLast();

    setDynamicSortFilter(false);
    sort(0);
    setDynamicSortFilter(true);
    emit sortStringChanged();
}

QStringList CardProxyModel::sortStrings() const
{
    QStringList sortStrings = _sortNameRoleMap.keys();
    sortStrings.sort();
    return sortStrings;
}

QSettings *CardProxyModel::settings()
{
    return &_settings;
}

void CardProxyModel::saveSettings()
{
    saveCategoryFilters(_textFilters);
    saveCategoryFilters(_typeFilters);
    saveCategoryFilters(_sideFilters);
    saveCategoryFilters(_factionFilters);
    saveCategoryFilters(_setFilters);
    saveNumericFilters(_statsFilters);

    _settings.setValue(_settingsSection + "/string", _stringFilter);

    saveSortList();
    saveSortDescList();
}

void CardProxyModel::resetFiltersAndSorting()
{
    _autoInvalidateFilter = false;

    resetCategoryFilters(_textFilters);
    resetCategoryFilters(_typeFilters);
    resetCategoryFilters(_sideFilters);
    resetCategoryFilters(_factionFilters);
    resetCategoryFilters(_setFilters);
    resetNumericFilters(_statsFilters);

    setStringFilter("");

    _autoInvalidateFilter = true;
    invalidateFilter();

    _sortList.clear();
    foreach (QString val, _sortDefaultList)
    {
        _sortList.append(val);
    }

    _sortDescList.clear();
    foreach (bool val, _sortDefaultDescList)
    {
        _sortDescList.append(val);
    }

    setDynamicSortFilter(false);
    sort(0);
    setDynamicSortFilter(true);

    emit sortStringChanged();
    emit descendingSortChanged();
}

void CardProxyModel::updateTextFilter()
{
    if (_autoInvalidateFilter)
        invalidateFilter();
}

void CardProxyModel::updateTypeFilter()
{
    _typeFilterOn = false;
    for (int i = 0; i < _typeFilters.size(); ++i) {
        if (!_typeFilters.at(i)->include())
        {
            _typeFilterOn = true;
            break;
        }
    }
    if (_autoInvalidateFilter)
        invalidateFilter();
}

void CardProxyModel::updateSideFilter()
{
    _sideFilterOn = false;
    for (int i = 0; i < _sideFilters.size(); ++i) {
        if (!_sideFilters.at(i)->include())
        {
            _sideFilterOn = true;
            break;
        }
    }
    if (_autoInvalidateFilter)
        invalidateFilter();
}

void CardProxyModel::updateFactionFilter()
{
    _factionFilterOn = false;
    for (int i = 0; i < _factionFilters.size(); ++i) {
        if (!_factionFilters.at(i)->include())
        {
            _factionFilterOn = true;
            break;
        }
    }
    if (_autoInvalidateFilter)
        invalidateFilter();
}

void CardProxyModel::updateSetFilter()
{
    _setFilterOn = false;
    for (int i = 0; i < _setFilters.size(); ++i) {
        if (!_setFilters.at(i)->include())
        {
            _setFilterOn = true;
            break;
        }
    }
    if (_autoInvalidateFilter)
        invalidateFilter();
}

void CardProxyModel::updateStatsFilter()
{
    if (_autoInvalidateFilter)
        invalidateFilter();
}

bool CardProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    // Filter Side
    if (_sideFilterOn)
    {
        for (int i = 0; i < _sideFilters.size(); ++i)
        {
            if (index.data(CardModel::sideRole).toString() == _sideFilters.at(i)->name())
            {
                if (!_sideFilters.at(i)->include())
                {
                    return false;
                }
                break;
            }
        }
    }

    // Filter Type
    if (_typeFilterOn)
    {
        for (int i = 0; i < _typeFilters.size(); ++i)
        {
            if (index.data(CardModel::typeRole).toString() == _typeFilters.at(i)->name())
            {
                if (!_typeFilters.at(i)->include())
                {
                    return false;
                }
                break;
            }
        }
    }

    // Filter Faction
    if (_factionFilterOn)
    {
        for (int i = 0; i < _factionFilters.size(); ++i)
        {
            if (index.data(CardModel::factionRole).toString() == _factionFilters.at(i)->name())
            {
                if (!_factionFilters.at(i)->include())
                {
                    return false;
                }
                break;
            }
        }
    }

    // Filter Set
    if (_setFilterOn)
    {
        for (int i = 0; i < _setFilters.size(); ++i)
        {
            if (index.data(CardModel::setRole).toString() == _setFilters.at(i)->name())
            {
                if (!_setFilters.at(i)->include())
                {
                    return false;
                }
                break;
            }
        }
    }

    // Filter Stats
    foreach (NumericFilter *filter, _statsFilters)
    {
        int value;
        if (filter->name() == "Cost")
            value = index.data(CardModel::costRole).toInt();
        else if (filter->name() == "Influence")
            value = index.data(CardModel::factionCostRole).toInt();
        else if (filter->name() == "Strength")
            value = index.data(CardModel::strengthRole).toInt();
        else if (filter->name() == "Advancement cost")
            value = index.data(CardModel::advancementCostRole).toInt();
        else if (filter->name() == "Agenda points")
            value = index.data(CardModel::agendaPointsRole).toInt();
        else if (filter->name() == "Memory")
            value = index.data(CardModel::memoryRole).toInt();
        else if (filter->name() == "Trash cost")
            value = index.data(CardModel::trashCostRole).toInt();
        else
            continue;

        if (!filter->evaluate(value))
        {
            return false;
        }
    }

    // Filter String (OR)
    if (_stringFilter != "")
    {
        bool containsString = false;
        for (int i = 0; i < _textFilters.size(); ++i)
        {
            if (_textFilters.at(i)->include())
            {
                bool value;
                if (_textFilters.at(i)->name() == "Name")
                {
                    value = index.data(CardModel::nameRole).toString().contains(_stringRegExp);
                }
                else if (_textFilters.at(i)->name() == "Text")
                {
                    value = index.data(CardModel::textRole).toString().contains(_stringRegExp);
                }
                else if (_textFilters.at(i)->name() == "Subtype")
                {
                    value = index.data(CardModel::subtypeRole).toString().contains(_stringRegExp);
                }

                if (value)
                {
                    containsString = true;
                    break;
                }
            }
        }
        if (!containsString)
        {
            return false;
        }
    }

    return true;
}

bool CardProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    for (int i = 0; i < _sortList.size(); ++i)
    {
        int role = _sortNameRoleMap.value(_sortList.at(i), CardModel::nameRole);

        QVariant leftData = left.data(role);
        QVariant rightData = right.data(role);

        if (leftData == rightData)
        {
            continue;
        }
        bool returnValue;

        if (leftData.type() == QVariant::Int)
        {
            returnValue = leftData.toInt() < rightData.toInt();
        }
        else if (leftData.type() == QVariant::UInt)
        {
            returnValue = leftData.toUInt() < rightData.toUInt();
        }
        else
        {
            returnValue = leftData.toString() < rightData.toString();
        }

        bool desc;
        if (i > _sortDescList.size())
        {
            desc = _sortDescList.last();
        }
        else
        {
            desc = _sortDescList.at(i);
        }

        return desc ? !returnValue : returnValue;

    }
    return true;
}

void CardProxyModel::loadTextFilter()
{
    if (_textFilters.size())
    {
        qDeleteAll(_textFilters);
        _textFilters.clear();
    }

    QStringList filters;
    filters << "Name" << "Text" << "Subtype";
    foreach(QString name, filters)
    {
        CategoryFilter *filter = new CategoryFilter(this);
        filter->setName(name);
        filter->setInclude(_settings.value(QString("%1/%2").arg(_settingsSection, name),
                                          true).toBool());

        connect(filter, &CategoryFilter::includeChanged, this, &CardProxyModel::updateTextFilter);
        _textFilters.append(filter);
    }
    emit textFilterChanged();
}

void CardProxyModel::loadTypeFilter()
{
    if (_typeFilters.size())
    {
        qDeleteAll(_typeFilters);
        _typeFilters.clear();
    }

    _typeFilterOn = false;

    foreach (QString name, _cardmodel->types())
    {
        CategoryFilter *filter = new CategoryFilter(this);
        filter->setName(name);
        filter->setInclude(_settings.value(QString("%1/%2").arg(_settingsSection,name),
                                          true).toBool());

        if (!filter->include())
        {
            _typeFilterOn = true;
        }

        connect(filter, &CategoryFilter::includeChanged, this, &CardProxyModel::updateTypeFilter);
        _typeFilters.append(filter);
    }
    emit typeFilterChanged();
}

void CardProxyModel::loadSideFilter()
{
    if (_sideFilters.size())
    {
        qDeleteAll(_sideFilters);
        _sideFilters.clear();
    }

    _sideFilterOn = false;

    foreach (QString name, _cardmodel->sides())
    {
        CategoryFilter *filter = new CategoryFilter(this);
        filter->setName(name);
        filter->setInclude(_settings.value(QString("%1/%2").arg(_settingsSection,name),
                                          true).toBool());

        if (!filter->include())
        {
            _sideFilterOn = true;
        }

        connect(filter, &CategoryFilter::includeChanged, this, &CardProxyModel::updateSideFilter);
        _sideFilters.append(filter);
    }
    emit sideFilterChanged();
}

void CardProxyModel::loadFactionFilter()
{
    if (_factionFilters.size())
    {
        qDeleteAll(_factionFilters);
        _factionFilters.clear();
    }

    _factionFilterOn = false;

    foreach (QString name, _cardmodel->factions())
    {
        CategoryFilter *filter = new CategoryFilter(this);
        filter->setName(name);
        filter->setInclude(_settings.value(QString("%1/%2").arg(_settingsSection,name),
                                          true).toBool());

        if (!filter->include())
        {
            _factionFilterOn = true;
        }

        connect(filter, &CategoryFilter::includeChanged, this, &CardProxyModel::updateFactionFilter);
        _factionFilters.append(filter);
    }
    emit factionFilterChanged();

}


void CardProxyModel::loadSetFilter()
{
    if (_setFilters.size())
    {
        qDeleteAll(_setFilters);
        _setFilters.clear();
    }

    _setFilterOn = false;

    foreach (QString name, _cardmodel->sets())
    {
        CategoryFilter *filter = new CategoryFilter(this);
        filter->setName(name);
        filter->setInclude(_settings.value(QString("%1/%2").arg(_settingsSection,name),
                                          true).toBool());

        if (!filter->include())
        {
            _setFilterOn = true;
        }

        connect(filter, &CategoryFilter::includeChanged, this, &CardProxyModel::updateSetFilter);
        _setFilters.append(filter);
    }
    emit setFilterChanged();
}

void CardProxyModel::loadStatsFilters()
{
    QStringList filters;
    filters << "Cost" << "Influence" << "Strength" << "Advancement cost" << "Agenda points"
            << "Memory" << "Trash cost";
    foreach(QString name, filters)
    {
        NumericFilter *filter = new NumericFilter(this);
        filter->setName(name);
        filter->setOperation(_settings.value(QString("%1/%2/operation").arg(_settingsSection,name),
                                          "=").toString());
        filter->setOperand(_settings.value(QString("%1/%2/operand").arg(_settingsSection,name),
                                          "").toString());

        connect(filter, &NumericFilter::operationChanged, this, &CardProxyModel::updateStatsFilter);
        connect(filter, &NumericFilter::operandChanged, this, &CardProxyModel::updateStatsFilter);
        _statsFilters.append(filter);
    }
    emit statsFilterChanged();
}

void CardProxyModel::loadSortStrings()
{
    if (_sortNameRoleMap.size())
    {
        _sortNameRoleMap.clear();
    }

    _sortNameRoleMap.insert("Set", CardModel::setSortOrderRole);
    _sortNameRoleMap.insert("Name", CardModel::nameRole);
    _sortNameRoleMap.insert("Side", CardModel::sideRole);
    _sortNameRoleMap.insert("Faction", CardModel::factionRole);
    _sortNameRoleMap.insert("Type", CardModel::typeRole);
    _sortNameRoleMap.insert("Cost", CardModel::costRole);
    _sortNameRoleMap.insert("Influence", CardModel::factionCostRole);
    _sortNameRoleMap.insert("Strength", CardModel::strengthRole);
    _sortNameRoleMap.insert("Agenda points", CardModel::agendaPointsRole);
    _sortNameRoleMap.insert("Advancement cost", CardModel::advancementCostRole);
    _sortNameRoleMap.insert("Memory", CardModel::memoryRole);
    _sortNameRoleMap.insert("Trash cost", CardModel::trashCostRole);

    emit sortStringsChanged();
}

void CardProxyModel::loadSortList()
{
    _sortList.clear();

    QVariantList defaultList;
    foreach (QString val, _sortDefaultList)
    {
        defaultList.append(val);
    }

    QVariantList sortList =
            _settings.value(_settingsSection + "/sort", defaultList).toList();

    for (int i = 0; i < sortList.size(); ++i)
    {
        _sortList.append(sortList.at(i).toString());
    }
    emit sortStringChanged();
}

void CardProxyModel::loadSortDescList()
{
    _sortDescList.clear();

    QVariantList defaultList;
    foreach (bool val, _sortDefaultDescList)
    {
        defaultList.append(val);
    }

    QVariantList sortDescList =
            _settings.value(_settingsSection + "/sortDescending", defaultList).toList();
    for (int i = 0; i < sortDescList.size(); ++i)
    {
        _sortDescList.append(sortDescList.at(i).toBool());
    }
    emit descendingSortChanged();
}

void CardProxyModel::saveSortList()
{
    QVariantList tmp;
    for (int i = 0; i < _sortList.size(); ++i)
    {
        tmp.append(_sortList.at(i));
    }
    _settings.setValue(_settingsSection + "/sort", tmp);
}

void CardProxyModel::saveSortDescList()
{
    QVariantList tmp;
    for (int i = 0; i < _sortDescList.size(); ++i)
    {
        tmp.append(_sortDescList.at(i));
    }
    _settings.setValue(_settingsSection + "/sortDescending", tmp);
}

void CardProxyModel::saveCategoryFilters(QList<CategoryFilter *> filters)
{
    for (int i = 0; i < filters.size(); ++i)
    {
        filters.at(i)->saveSettings();
    }
}

void CardProxyModel::resetCategoryFilters(QList<CategoryFilter *> filters)
{
    for (int i = 0; i < filters.size(); ++i)
    {
        filters.at(i)->setInclude();
    }
}

void CardProxyModel::saveNumericFilters(QList<NumericFilter *> filters)
{
    for (int i = 0; i < filters.size(); ++i)
    {
        filters.at(i)->saveSettings();
    }
}

void CardProxyModel::resetNumericFilters(QList<NumericFilter *> filters)
{
    for (int i = 0; i < filters.size(); ++i)
    {
        filters.at(i)->setOperation();
        filters.at(i)->setOperand();
    }
}
