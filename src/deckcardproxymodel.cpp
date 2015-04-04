#include "deckcardproxymodel.h"

#include "cardmodel.h"

DeckCardProxyModel::DeckCardProxyModel(QObject *parent) :
    CardProxyModel(parent)
{
}

bool DeckCardProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QString leftData = left.data(CardModel::typeRole).toString();
    QString rightData = right.data(CardModel::typeRole).toString();

    if (leftData == "Identity" && rightData != "Identity")
    {
        return true;
    }
    else if (leftData != "Identity" && rightData == "Identity")
    {
        return false;
    }

    return CardProxyModel::lessThan(left, right);
}
