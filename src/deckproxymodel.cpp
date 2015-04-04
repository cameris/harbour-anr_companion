#include "deckproxymodel.h"

#include "deckmodel.h"

DeckProxyModel::DeckProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);

    _sortDefaultList << "Side" << "Faction" << "Identity" << "Name";
    _sortDefaultDescList << false << false << false << false;

    _sortList = _sortDefaultList;
    _sortDescList = _sortDefaultDescList;
}

void DeckProxyModel::classBegin()
{
}

void DeckProxyModel::componentComplete()
{
    loadSortStrings();

    invalidateFilter();
    sort(0);
}

QObject *DeckProxyModel::source() const
{
    return sourceModel();
}

void DeckProxyModel::setSource(QObject *source)
{
    if (!source)
        return;

    setSourceModel(qobject_cast<QAbstractItemModel *>(source));
}

bool DeckProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    for (int i = 0; i < _sortList.size(); ++i)
    {
        int role = _sortNameRoleMap.value(_sortList.at(i), DeckModel::nameRole);

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

void DeckProxyModel::loadSortStrings()
{
    if (_sortNameRoleMap.size())
    {
        _sortNameRoleMap.clear();
    }

    _sortNameRoleMap.insert("Name", DeckModel::nameRole);
    _sortNameRoleMap.insert("Side", DeckModel::identitySideRole);
    _sortNameRoleMap.insert("Faction", DeckModel::identityFactionRole);
    _sortNameRoleMap.insert("Identity", DeckModel::identityNameRole);

    emit sortStringsChanged();
}
