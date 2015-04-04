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

#ifndef NUMERICFILTER_H
#define NUMERICFILTER_H

#include <QObject>
#include <QStringList>

class CardProxyModel;

class NumericFilter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString operation READ operation WRITE setOperation NOTIFY operationChanged)
    Q_PROPERTY(QString operand READ operand WRITE setOperand NOTIFY operandChanged)
public:
    explicit NumericFilter(CardProxyModel *parent = 0);

    Q_INVOKABLE static QStringList operations();

    QString name() const;
    void setName(const QString &name);

    QString operation() const;
    void setOperation(const QString &operation = "=");

    QString operand() const;
    void setOperand(const QString &operand = "");

    bool evaluate(const int value);

    void saveSettings();

signals:
    void nameChanged();
    void operationChanged();
    void operandChanged();

private:
    CardProxyModel *_parent;

    QString _name;
    QString _operation;
    QString _operand;

};

#endif // NUMERICFILTER_H
