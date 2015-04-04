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

SilicaFlickable {
    id: cardFlick

    property bool scaled: width == contentWidth ? false : true
    property alias source: cardImage.source

    contentWidth: width
    contentHeight: height

    pressDelay: 0
    interactive: scaled

    function _fit() {
        contentX = 0
        contentY = 0
        contentWidth = width
        contentHeight = height
    }

    PinchArea {
        width: Math.max(cardFlick.contentWidth, cardFlick.width)
        height: Math.max(cardFlick.contentHeight, cardFlick.height)

        property real initialWidth
        property real initialHeight
        onPinchStarted: {
            initialWidth = cardFlick.contentWidth
            initialHeight = cardFlick.contentHeight
        }

        onPinchUpdated: {
            cardFlick.contentX += pinch.previousCenter.x - pinch.center.x
            cardFlick.contentY += pinch.previousCenter.y - pinch.center.y

            var newWidth = Math.max(initialWidth * pinch.scale, cardFlick.width)
            var newHeight = Math.max(initialHeight * pinch.scale, cardFlick.height)

            newWidth = Math.min(newWidth, cardFlick.width * 3)
            newHeight = Math.min(newHeight, cardFlick.height * 3)

            cardFlick.resizeContent(newWidth, newHeight, pinch.center)
        }

        onPinchFinished: {
            cardFlick.returnToBounds()
        }

        MouseArea {
            anchors.fill: parent
            onDoubleClicked: {
                if( cardFlick.scaled)
                    cardFlick._fit()
            }
        }
    }

    Item {
        width: cardFlick.contentWidth
        height: cardFlick.contentHeight
        smooth: !(cardFlick.movingVertically || cardFlick.movingHorizontally)
        anchors.centerIn: parent
        clip: true

        Image {
            id: cardImage
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
        }
    }

    ScrollDecorator {}
}
