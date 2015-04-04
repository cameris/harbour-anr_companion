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

#include "numericfilter.h"

#include "cardproxymodel.h"

NumericFilter::NumericFilter(CardProxyModel *parent) :
    QObject(parent), _parent(parent), _name(""), _operation("="), _operand("")
{
}

QStringList NumericFilter::operations()
{
    QStringList operations;
    operations << ">" << ">=" << "=" << "<=" << "<";
    return operations;
}

QString NumericFilter::name() const
{
    return _name;
}

void NumericFilter::setName(const QString &name)
{
    _name = name;
    emit nameChanged();
}
QString NumericFilter::operation() const
{
    return _operation;
}

void NumericFilter::setOperation(const QString &operation)
{
    if (operation == _operation)
        return;
    _operation = operation;
    emit operationChanged();
}
QString NumericFilter::operand() const
{
    return _operand;
}

void NumericFilter::setOperand(const QString &operand)
{
    if (operand == _operand)
        return;
    _operand = operand;
    emit operandChanged();
}

bool NumericFilter::evaluate(const int value)
{
    if (_operand == "")
        return true;

    int operand = _operand.toInt();

    if (_operation == ">")
        return value > operand;
    if (_operation == ">=")
        return value >= operand;
    if (_operation == "=")
        return value == operand;
    if (_operation == "<=")
        return value <= operand;
    if (_operation == "<")
        return value < operand;

    return true;
}

void NumericFilter::saveSettings()
{
    _parent->settings()->setValue(
                QString("%1/%2/operation").arg(_parent->settingsSection(), _name), _operation);
    _parent->settings()->setValue(
                QString("%1/%2/operand").arg(_parent->settingsSection(), _name), _operand);
}
