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

Item {
    width: parent.width

    property var factionSymbol: factionMap(model.faction)

    Label {
        anchors.centerIn: parent
        font.family: netrunnerFont.name
        color: factionSymbol.color
        text: factionSymbol.string
        font.pixelSize: parent.width * 0.8
        opacity: 0.2
    }

    Column {
        spacing: Theme.paddingSmall
        anchors.centerIn: parent
        width: parent.width

        Row {
            spacing: Theme.paddingMedium
            width: parent.width

            Loader {
                id: thumbLoader
                width: active ? parent.width/4 : 0
                anchors.verticalCenter: parent.verticalCenter
                active: model.thumbnail

                sourceComponent: Image {
                    asynchronous: true
                    fillMode: Image.PreserveAspectFit
                    source: model.thumbnail
                }
            }

            Column {
                spacing: Theme.paddingSmall
                width: parent.width - thumbLoader.width
                anchors.verticalCenter: parent.verticalCenter

                Label {
                    width: parent.width
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeLarge
                    text: (model.unique === 1 ? "♦" : "") +  model.name
                }

                Flow {
                    width: parent.width

                    Label {
                        text: model.type + (model.subtype ? ": " : "")
                    }
                    Repeater {
                        model: subtype.split(" - ")
                        delegate: Label {
                            text: (index !== 0 ? " - " : "") + modelData
                        }
                    }

                    // Identity
                    Label {
                        visible: model.type === "Identity"
                        text: " • Deck: " + model.minDeckSize
                    }
                    Label {
                        visible: model.type === "Identity"
                        text: " • Influence: " + (model.influenceLimit > -1 ? model.influenceLimit : "\u221e")
                    }

                    // Agenda
                    Label {
                        visible: model.type === "Agenda"
                        text: " • Adv: " + model.advancementCost
                    }
                    Label {
                        visible: model.type === "Agenda"
                        text: " • Score: " + model.agendaPoints
                    }

                    // Asset and Upgrade
                    Label {
                        visible: model.type === "Asset" || model.type === "Upgrade"
                        text: " • Rez: " + model.cost
                    }
                    Label {
                        visible: model.type === "Asset" || model.type === "Upgrade"
                        text: " • Trash: " + model.trashCost
                    }

                    // ICE
                    Label {
                        visible: model.type === "ICE"
                        text: " • Rez: " + model.cost
                    }
                    Label {
                        visible: model.type === "ICE"
                        text: " • Strength: " + model.strength
                    }

                    // Operation & Event
                    Label {
                        visible: model.type === "Operation" || model.type === "Event"
                        text: " • Cost: " + model.cost
                    }

                    // Hardware & Resource
                    Label {
                        visible: model.type === "Hardware" || model.type === "Resource"
                        text: " • Install: " + model.cost
                    }

                    // Program
                    Label {
                        visible: model.type === "Program"
                        text: " • Install: " + model.cost
                    }
                    Label {
                        visible: model.type === "Program"
                        text: " • Memory: " + model.memory
                    }
                    Label {
                        visible: model.type === "Program" && model.strength > -1
                        text: " • Strength: " + model.strength
                    }

                    // Influence
                    Label {
                        visible: model.factionCost > -1
                        text: " • Influence: " + model.factionCost
                    }
                }
            }
        }

        Separator {
            color: Theme.highlightColor
            width: parent.width
        }

        Label {
            function formatText(text) {
                text = text.replace(/\n/g, "<br>")
                text = text.replace(/&minus;/g, "-")
                text = text.replace(/&ndash;/g, "-")
                text = text.replace(/&/g, "&amp;")
                text = text.replace(/\[Credits*\]/g, "<font face=\""+netrunnerFont.name+"\">\ue606</font>")
                text = text.replace(/\[Link\]/g, "<font face=\""+netrunnerFont.name+"\">\ue616</font>")
                text = text.replace(/\[Subroutine\]/g, "<font face=\""+netrunnerFont.name+"\">\ue611</font>")
                text = text.replace(/\[Trash\]/g, "<font face=\""+netrunnerFont.name+"\">\ue612</font>")
                text = text.replace(/\[Recurring Credits*\]/g, "<font face=\""+netrunnerFont.name+"\">\ue60d</font>")
                text = text.replace(/\[Memory Unit\]/g, "<font face=\""+netrunnerFont.name+"\">\ue60b</font>")
                text = text.replace(/\[Click\]/g, "<font face=\""+netrunnerFont.name+"\">\ue605</font>")
                return text
            }

            width: parent.width
            wrapMode: Text.WordWrap
            textFormat: Text.RichText
            text: formatText(model.text)
        }

        Separator {
            color: Theme.highlightColor
            width: parent.width
        }

        Flow {
            width: parent.width

            Label {
                font.pixelSize: Theme.fontSizeSmall
                font.family: netrunnerFont.name
                color: factionSymbol.color
                text: factionSymbol.string
            }

            Label {
                font.pixelSize: Theme.fontSizeSmall
                text: model.faction
            }
            Label {
                visible: model.set
                font.pixelSize: Theme.fontSizeSmall
                text: " • " + model.set
            }
            Label {
                visible: model.number > -1
                font.pixelSize: Theme.fontSizeSmall
                text: " #" + model.number
            }
        }
    }
}
