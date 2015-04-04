#ifndef DECKCARDPROXYMODEL_H
#define DECKCARDPROXYMODEL_H

#include "cardproxymodel.h"

class DeckCardProxyModel : public CardProxyModel
{
    Q_OBJECT
public:
    explicit DeckCardProxyModel(QObject *parent = 0);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

};

#endif // DECKCARDPROXYMODEL_H
