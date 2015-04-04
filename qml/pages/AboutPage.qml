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

import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height + Theme.paddingLarge

        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: "About"
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "A:NR Companion"
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "License: GPLv3"
            }

            Button {
                text: "Sourcecode on Github"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    openSourcecode.clicked(mouse)
                }
                OpenBrowser {
                    id: openSourcecode
                    url: "https://github.com/cameris/harbour-anr_companion"
                }

            }

            Separator {
                width: parent.width
                color: Theme.highlightColor
            }

            Flow {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingMedium

                Label {
                    text: "powered by"
                }

                Label {
                    text: "netrunnerdb.com"
                    color: Theme.highlightColor
                    OpenBrowser {
                        anchors.fill: parent
                        url: "http://netrunnerdb.com"
                    }
                }
            }

        }
    }
}
