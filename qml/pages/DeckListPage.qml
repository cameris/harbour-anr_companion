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
import harbour.anr_companion 1.0

Page {
    id: deckListPage

    DeckProxyModel {
        id: deckModel
        source: DeckModel { id: deckSourceModel}
    }

    SilicaListView {
        id: deckView
        anchors.fill: parent

        header: PageHeader {
            title: "Decks"
        }

        ViewPlaceholder {
            enabled: deckView.count === 0
            text: "No decks available"
        }

        PullDownMenu {
            MenuItem {
                text: "Add deck"
                onClicked: {
                    var dialog = pageStack.push(deckEdit, {
                                                    title: "New deck"
                                                })
                    dialog.accepted.connect(function () {
                        deckSourceModel.addDeck(dialog.name)
                    })
                }
            }
        }

        model: deckModel

        delegate: ListItem {
            id: listDelegate

            property int delegateMargin: Theme.paddingLarge

            contentHeight: Math.max(Theme.itemSizeMedium,
                                    Math.max(thumbLoader.height, textColumn.height))
                           + delegateMargin

            showMenuOnPressAndHold:  true
            menu: ContextMenu {
                MenuItem {
                    text: "Export as OCTGN deck"
                    onClicked: {
                        deckSourceModel.exportOCTGN(index)
                        exportPopUpComponent.createObject(deckListPage, {
                                                     "deckName" : model.name
                                                 })
                    }
                }
                MenuItem {
                    text: "Edit deck"
                    onClicked: {
                        var dialog = pageStack.push(deckEdit, {
                                                        title: "Edit deck",
                                                        name: model.name
                                                    })

                        dialog.accepted.connect(function () {
                            model.name = dialog.name
                        })
                    }
                }
                MenuItem {
                    text: "Copy deck"
                    onClicked: {
                        deckSourceModel.copyDeck(model.id)
                    }
                }

                MenuItem {
                    text: "Remove deck"
                    onClicked: {
                        listDelegate.remorseAction("Removing deck", function () {
                            deckSourceModel.removeDeck(model.id)
                        })
                    }
                }
            }

            Loader {
                id: thumbLoader
                x: delegateMargin
                anchors.verticalCenter: parent.verticalCenter
                width: active ? listDelegate.width/5 : 0

                active: identityThumbnail

                sourceComponent: Component {
                    Image {
                        asynchronous: true
                        fillMode: Image.PreserveAspectFit
                        source: identityThumbnail
                    }
                }
            }

            Column {
                id: textColumn
                anchors {
                    left: thumbLoader.right
                    right: parent.right
                    verticalCenter: parent.verticalCenter

                    leftMargin: Theme.paddingMedium
                    rightMargin: delegateMargin
                }

                Label {
                    id: deckNameLabel
                    width: parent.width
                    wrapMode: Text.WordWrap
                    color: listDelegate.highlighted ? Theme.highlightColor : Theme.primaryColor

                    text: model.name
                }
            }

            onClicked: {
                pageStack.push(Qt.resolvedUrl("DeckPage.qml"), {
                                   deck: deckSourceModel.getDeck(model.id)
                               })
            }
        }
    }

    Component {
        id: exportPopUpComponent
        Rectangle {
            id: exportRectangle
            property string deckName

            anchors.top: parent.top
            width: parent.width
            height: exportLabel.height

            color: Theme.highlightBackgroundColor

            Rectangle {
                anchors.fill: parent
                color: Theme.rgba(Theme.highlightDimmerColor, 0.3)
            }

            Label {
                id: exportLabel
                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    margins: Theme.paddingMedium
                }

                wrapMode: Text.WordWrap
                color: Theme.highlightColor
                opacity: parent.opacity
                font.pixelSize: Theme.fontSizeTiny
                text: "Exported to Documents" + (deckName ? "/" + deckName + ".o8d" : "")
            }

            Timer {
                interval: 4500
                running: true
                onTriggered: {
                    exportDestroyAnimation.running = true
                }
            }

            NumberAnimation on opacity {
                id: exportDestroyAnimation
                to: 0
                duration: 500
                running: false

                onRunningChanged: {
                    if (!running) {
                        exportRectangle.destroy()
                    }
                }
            }
        }
    }

    Component {
        id: deckEdit
        Dialog {
            property alias title: header.title
            property alias name: nameInput.text

            Column {
                id: column
                width: parent.width

                DialogHeader {
                    id: header
                }

                TextField {
                    id: nameInput
                    anchors {
                        margins: Theme.paddingLarge
                        left: parent.left
                        right: parent.right
                    }

                    label: "Deck name"
                    placeholderText: "Insert deck name"
                    validator: RegExpValidator { regExp: /^.+$/ }
                    inputMethodHints: Qt.ImhNoPredictiveText

                    EnterKey.iconSource: "image://theme/icon-m-enter-close"
                    EnterKey.onClicked: focus = false

                    Component.onCompleted: focus = true
                }
            }
            canAccept: nameInput.acceptableInput
        }
    }
}
