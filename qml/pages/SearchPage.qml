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

import "../components"

Page {
    id: searchPage
    objectName: "cardSearchPage"

    property string activeLayout: settings.value(cardModel.settingsSection + "/layout", "list")
    property bool allowPick: false

    signal picked(string id)

    function scrollToTop() {
        viewLoader.item.scrollToTop()
    }

    onStatusChanged: {
        if (status === PageStatus.Deactivating)
        {
            cardModel.saveSettings()
        }
    }

    CardProxyModel {
        id: cardModel
        source: CardModel { id: cardSourceModel}
        settingsSection: "search"

        Component.onCompleted: {
            cardSourceModel.loadCards()
        }
    }

    Loader {
        id: viewLoader
        anchors.fill: parent
        sourceComponent: activeLayout === "list" ? cardListComponent : cardGridComponent
    }

    SearchHeader {
        id: headerContainer
        width: searchPage.width
        cardModel: cardModel

        header: PageHeader { title: "Card Search" }
    }

    Component {
        id: sharedPullDown
        PullDownMenu {
            MenuItem {
                text: "Reset filters and sorting"
                onClicked: {
                    cardModel.resetFiltersAndSorting()
                }
            }
            MenuItem {
                text: "Switch to " + (activeLayout === "list" ? "grid" : "list")
                onClicked: {
                    activeLayout = activeLayout === "list" ? "grid" : "list"
                    settings.setValue(cardModel.settingsSection + "/layout", activeLayout)
                }
            }
        }
    }

    Component {
        id: cardGridComponent
        SilicaGridView {
            id: cardGridView
            anchors.fill:parent

            currentIndex: -1

            ViewPlaceholder {
                enabled: cardGridView.count === 0
                verticalOffset: headerContainer.height/2
                text: "No cards, update database"
            }

            header: Item {
                id: header
                width: headerContainer.width
                height: headerContainer.height
                Component.onCompleted: headerContainer.parent = header
            }


            cellWidth: Math.floor(width / 3)
            cellHeight: Math.floor(cellWidth * 7/5)

            Loader {
                sourceComponent: sharedPullDown
            }

            onMovingVerticallyChanged: {
                headerContainer.searchField.focus = false
            }

            model: cardModel

            delegate: ListItem {
                id: gridDelegate
                width: cardGridView.cellWidth
                contentHeight: cardGridView.cellHeight
                showMenuOnPressAndHold: searchPage.allowPick

                menu: GridContextMenu {
                    gridDelegate:  gridDelegate
                    modelCount: cardGridView.count

                    MenuItem {
                        text: "Add card"
                        onClicked: {
                            searchPage.picked(id)
                        }
                    }
                }

                Loader {
                    id: thumbLoader
                    anchors.fill: parent

                    active: thumbnail

                    sourceComponent: Image {
                        asynchronous: true
                        fillMode: Image.PreserveAspectCrop
                        source: thumbnail
                    }
                }

                Loader {
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width

                    active: !thumbLoader.active
                    sourceComponent: Component {
                        Item {
                            Label {
                                property var factionSymbol: factionMap(faction)
                                anchors.centerIn: parent
                                font.family: netrunnerFont.name
                                color: factionSymbol.color
                                text: factionSymbol.string
                                font.pixelSize: parent.width * 0.8
                                opacity: 0.2
                            }

                            Label {
                                anchors.centerIn: parent
                                width: parent.width - Theme.paddingMedium
                                wrapMode: Text.Wrap
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: Theme.fontSizeSmall
                                color: gridDelegate.highlighted ? Theme.highlightColor : Theme.primaryColor

                                textFormat: Text.RichText
                                text: Theme.highlightText(name, cardModel.stringRegExp, Theme.highlightColor)
                            }
                        }
                    }
                }

                onClicked: {
                    headerContainer.searchField.focus = false
                    var page = pageStack.push(Qt.resolvedUrl("CardPage.qml"), {
                                                  currentIndex: index,
                                                  cardmodel: cardModel,
                                                  allowPick: searchPage.allowPick
                                              })

                    if (searchPage.allowPick) {
                        page.picked.connect(searchPage.picked)
                    }

                    function repositionView() {
                        if (page.status === PageStatus.Deactivating)
                        {
                            page.picked.disconnect(picked)
                            page.statusChanged.disconnect(repositionView)
                            cardGridView.positionViewAtIndex(page.currentIndex, ListView.Contain)
                        }
                    }

                    page.statusChanged.connect(repositionView)
                }
            }

            VerticalScrollDecorator {}

        }
    }

    Component {
        id: cardListComponent
        SilicaListView {
            id: cardListView
            anchors.fill: parent

            currentIndex: -1

            ViewPlaceholder {
                enabled: cardListView.count === 0
                verticalOffset: headerContainer.height/2
                text: "No cards, update database"
            }

            header: Item {
                id: header
                width: headerContainer.width
                height: headerContainer.height
                Component.onCompleted: headerContainer.parent = header
            }

            Loader {
                sourceComponent: sharedPullDown
            }

            onMovingVerticallyChanged: {
                headerContainer.searchField.focus = false
            }

            model: cardModel

            delegate: ListItem {
                id: listDelegate

                property int delegateMargin: Theme.paddingLarge

                contentHeight: Math.max(thumbLoader.height, textColumn.height) + delegateMargin

                showMenuOnPressAndHold: searchPage.allowPick

                menu: ContextMenu {
                    MenuItem {
                        text: "Add card"
                        onClicked: {
                            searchPage.picked(id)
                        }
                    }
                }

                Loader {
                    id: thumbLoader
                    x: delegateMargin
                    width: active ? parent.width/5 : 0
                    anchors {
                        verticalCenter: parent.verticalCenter
                    }
                    active: thumbnail

                    sourceComponent: Component {
                        Image {
                            asynchronous: true
                            fillMode: Image.PreserveAspectFit
                            sourceSize.width: parent.width
                            source: thumbnail
                        }
                    }
                }
                Column {
                    id: textColumn
                    anchors {
                        left: thumbLoader.right
                        right: parent.right
                        verticalCenter: parent.verticalCenter

                        leftMargin: thumbLoader.active ? Theme.paddingMedium : 0
                        rightMargin: delegateMargin
                    }

                    Label {
                        id: nameLabel
                        width: parent.width
                        wrapMode: Text.WordWrap
                        font.pixelSize: Theme.fontSizeMedium
                        color: listDelegate.highlighted ? Theme.highlightColor : Theme.primaryColor

                        textFormat: Text.RichText
                        text: Theme.highlightText(name, cardModel.stringRegExp, Theme.highlightColor)
                    }
                    Flow {
                        width: parent.width
                        spacing: Theme.paddingSmall

                        Label {
                            property var factionSymbol: factionMap(faction)
                            font.pixelSize: Theme.fontSizeSmall
                            font.family: netrunnerFont.name
                            color: factionSymbol.color
                            text: factionSymbol.string
                        }
                        Label {
                            font.pixelSize: Theme.fontSizeSmall
                            color: listDelegate.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                            text: type
                        }
                    }
                }

                onClicked: {
                    headerContainer.searchField.focus = false
                    var page = pageStack.push(Qt.resolvedUrl("CardPage.qml"), {
                                                  currentIndex: index,
                                                  cardmodel: cardModel,
                                                  allowPick: searchPage.allowPick
                                              })

                    if (searchPage.allowPick) {
                        page.picked.connect(searchPage.picked)
                    }

                    function repositionView() {
                        if (page.status === PageStatus.Deactivating)
                        {
                            page.picked.disconnect(picked)
                            page.statusChanged.disconnect(repositionView)
                            cardListView.positionViewAtIndex(page.currentIndex, ListView.Contain)
                        }
                    }

                    page.statusChanged.connect(repositionView)
                }
            }

            VerticalScrollDecorator {}
        }
    }
}
