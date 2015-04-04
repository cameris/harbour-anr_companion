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
import "pages"

ApplicationWindow
{
    function factionMap (faction) {
        switch(faction)
        {
        case "Haas-Bioroid":
            return { string: "\ue608", color: "#826a8d" }
        case "Anarch":
            return { string: "\ue604", color: "#c46140" }
        case "Weyland Consortium":
            return { string: "\ue613", color: "#688d82" }
        case "Jinteki":
            return { string: "\ue609", color: "#a20f12" }
        case "NBN":
            return { string: "\ue60c", color: "#f7b432" }
        case "Criminal":
            return { string: "\ue607", color: "#609bc9" }
        case "Shaper":
            return { string: "\ue610", color: "#6dbe5b" }
        case "Neutral":
            return { string: "", color: Theme.primaryColor }
        default:
            return { string: faction, color: Theme.primaryColor }
        }
    }

    FontLoader {
        id: netrunnerFont
        source: "fonts/netrunner.ttf"
    }


    initialPage: Component { MainPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
}
