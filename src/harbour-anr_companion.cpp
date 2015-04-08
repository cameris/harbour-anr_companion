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

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QStandardPaths>

#include "settings.h"
#include "downloader.h"
#include "cardmodel.h"
#include "cardproxymodel.h"
#include "deckmodel.h"
#include "deckproxymodel.h"
#include "deckcardmodel.h"
#include "deckcardproxymodel.h"
#include "categoryfilter.h"
#include "numericfilter.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setOrganizationName("anr");
    QCoreApplication::setApplicationName("harbour-anr_companion");

    QGuiApplication *app = SailfishApp::application(argc, argv);
    QQuickView *view = SailfishApp::createView();

    qmlRegisterType<CardModel>("harbour.anr_companion", 1, 0, "CardModel");
    qmlRegisterType<CategoryFilter>("harbour.anr_companion", 1, 0, "CategoryFilter");
    qmlRegisterType<NumericFilter>("harbour.anr_companion", 1, 0, "NumericFilter");
    qmlRegisterType<CardProxyModel>("harbour.anr_companion", 1, 0, "CardProxyModel");


    qmlRegisterType<DeckModel>("harbour.anr_companion", 1, 0, "DeckModel");
    qmlRegisterType<DeckProxyModel>("harbour.anr_companion", 1, 0, "DeckProxyModel");
    qmlRegisterType<DeckCardModel>("harbour.anr_companion", 1, 0, "DeckCardModel");
    qmlRegisterType<DeckCardProxyModel>("harbour.anr_companion", 1, 0, "DeckCardProxyModel");

    Settings settings;
    if (settings.value("cards/images", "").toString() == "")
    {
        settings.setValue("cards/images",
                          QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/images");
    }
    view->rootContext()->setContextProperty("settings", &settings);
    view->rootContext()->setContextProperty("downloader", Downloader::instance());

    view->setSource(SailfishApp::pathTo("qml/harbour-anr_companion.qml"));
    view->show();

    return app->exec();
}

