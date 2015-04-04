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

Label {
    id: label
    property int counter
    property int opponentCounter
    property bool opponentCounterVisible: opponentCounter

    height: parent.height/2
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    font.pixelSize: Theme.fontSizeHuge * 4
    font.bold: true
    text: label.counter

    Label {
        anchors {
            top: parent.top
            right: parent.right
            rightMargin: Theme.paddingMedium
        }
        verticalAlignment: Text.AlignTop
        horizontalAlignment:  Text.AlignRight
        visible: label.opponentCounterVisible
        font.pixelSize: Theme.fontSizeMedium * 4
        font.bold: true
        color: Theme.secondaryColor
        text: label.opponentCounter
    }

    IconButton {
        id: plus
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        height: parent.height/2

        icon.source: "image://theme/icon-m-add"

        onClicked: {
            label.counter += 1
        }
    }

    IconButton {
        id: minus
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: parent.height/2

        icon.source: "image://theme/icon-m-remove"

        onClicked: {
            if (label.counter > 0)
                label.counter -= 1
        }
    }
}
