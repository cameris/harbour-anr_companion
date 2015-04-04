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

ContextMenu {
    id: gridContextMenu

    property Item gridDelegate
    property int modelCount
    property int _zBefore

    onActiveChanged: {
        if (active == true)
        {
            _zBefore = gridDelegate.z
            gridDelegate.z = modelCount + 1
        }
        else
        {
            gridDelegate.z = _zBefore
        }
    }

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color:
                    Theme.rgba(Theme.highlightBackgroundColor, 1.0) }
            GradientStop { position: 0.8; color:
                    Theme.rgba(Theme.highlightBackgroundColor, 0.8) }
            GradientStop { position: 1.0; color:
                    Theme.rgba(Theme.highlightBackgroundColor, 0.5) }
        }

        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: Theme.rgba("black", 0.6) }
                GradientStop { position: 0.5; color: Theme.rgba("black", 0.6) }
                GradientStop { position: 1.0; color: Theme.rgba("black", 0.8) }
            }
        }

        z: -1
        Component.onCompleted: {
            parent = gridContextMenu
        }
    }
}
