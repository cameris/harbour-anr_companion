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
    id: deckPage

    property string activeLayout: settings.value(cardModel.settingsSection + "/layout", "list")

    property var deck

    onStatusChanged: {
        if (status === PageStatus.Deactivating)
        {
            cardModel.saveSettings()
        }
    }

    Component.onCompleted: {
        deckPage.deck.loadCards()
    }
    Component.onDestruction: {
        deckPage.deck.unloadCards()
    }

    DeckCardProxyModel {
        id: cardModel
        source: deckPage.deck
        settingsSection: "deck"
    }

    Loader {
        id: viewLoader
        anchors.fill: parent
        sourceComponent: activeLayout === "list" ? cardListComponent : cardGridComponent
    }

    SearchHeader {
        id: headerContainer
        width: deckPage.width
        cardModel: cardModel

        header: PageHeader { title: deckPage.deck.deckName }

        footer: Column {
            width: parent.width
            spacing: Theme.paddingMedium
            anchors.topMargin: Theme.paddingLarge
            anchors.bottomMargin: Theme.paddingLarge

            Separator {
                width: parent.width
                color: Theme.highlightColor
            }

            Column {
                id: statscolumn
                x: Theme.paddingLarge
                width: parent.width - 2 * Theme.paddingLarge
                property string errorColor: "#ff4d4d"

                Label {
                    visible: !deckPage.deck.identityError
                    width: parent.width
                    color: deckPage.deck.deckSize < deckPage.deck.minDeckSize ? statscolumn.errorColor : Theme.primaryColor
                    text: "Deck: " + deckPage.deck.deckSize + " / min. " + deckPage.deck.minDeckSize
                }

                Label {
                    visible: !deckPage.deck.identityError
                    width: parent.width
                    color: deckPage.deck.usedInfluence > deckPage.deck.maxInfluence ? statscolumn.errorColor : Theme.primaryColor
                    text: "Influence: " + deckPage.deck.usedInfluence + " / max. " + deckPage.deck.maxInfluence
                }

                Label {
                    visible: deckPage.deck.agendaPointsRange["min"] > 0 && !deckPage.deck.identityError
                    width: parent.width
                    color: (deckPage.deck.usedAgendaPoints < deckPage.deck.agendaPointsRange["min"] ||
                            deckPage.deck.usedAgendaPoints > deckPage.deck.agendaPointsRange["max"])
                            ? statscolumn.errorColor : Theme.primaryColor
                    text: "Agenda Points: " + deckPage.deck.usedAgendaPoints + " / " +
                          deckPage.deck.agendaPointsRange["min"] + " - " + deckPage.deck.agendaPointsRange["max"]
                }

                Label {
                    width: parent.width
                    visible: deckPage.deck.identityError
                    wrapMode: Text.WordWrap
                    color: statscolumn.errorColor
                    text: "There must be exactly one Identity in a deck"

                }

                Column
                {
                    id: includeErrorColumn
                    width: parent.width
                    visible: !deckPage.deck.identityError

                    Label {
                        width: parent.width
                        visible: includeErrorRepeater.count
                        wrapMode: Text.WordWrap
                        font.pixelSize: Theme.fontSizeSmall
                        color: statscolumn.errorColor
                        text: "Can't be included with the identity:"
                    }

                    Repeater {
                        id: includeErrorRepeater
                        model: deckPage.deck.includeError
                        delegate: Label {
                            x: Theme.paddingLarge
                            width: includeErrorColumn.width - 2 * Theme.paddingLarge
                            wrapMode: Text.WordWrap
                            font.pixelSize: Theme.fontSizeSmall
                            color: statscolumn.errorColor
                            text: "• " +  modelData
                        }
                    }
                }

                Column
                {
                    id: limitErrorColumn
                    width: parent.width
                    visible: !deckPage.deck.identityError

                    Label {
                        width: parent.width
                        visible: limitErrorRepeater.count
                        wrapMode: Text.WordWrap
                        font.pixelSize: Theme.fontSizeSmall
                        color: statscolumn.errorColor
                        text: "Over copy limit:"
                    }

                    Repeater {
                        id: limitErrorRepeater
                        model: deckPage.deck.limitError
                        delegate: Label {
                            x: Theme.paddingLarge
                            width: limitErrorColumn.width - 2 * Theme.paddingLarge
                            wrapMode: Text.WordWrap
                            font.pixelSize: Theme.fontSizeSmall
                            color: statscolumn.errorColor
                            text: "• " +  modelData
                        }
                    }
                }
            }

            Separator {
                width: parent.width
                color: Theme.highlightColor
            }

            Rectangle {
                height: Theme.paddingSmall
                width: 1
                opacity: 0
            }
        }
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
            MenuItem {
                text: "Add card"
                onClicked: {
                    headerContainer.searchField.focus = false
                    var picker = pageStack.push("SearchPage.qml", {
                                                    allowPick: true
                                                })

                    picker.picked.connect(function (id) {
                        deckPage.deck.addCard(id)
                    })
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
            header: Item {
                id: header
                width: headerContainer.width
                height: headerContainer.height
                Component.onCompleted: headerContainer.parent = header
            }

            ViewPlaceholder {
                enabled: cardGridView.count === 0
                verticalOffset: headerContainer.height/2
                text: "No cards in section"
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

                menu: GridContextMenu {
                    gridDelegate: gridDelegate
                    modelCount: cardGridView.count

                    MenuItem {
                        text: "Remove card"
                        onClicked: {
                            gridDelegate.remorseAction("Removing card", function () {
                                deckPage.deck.removeCard(id, count)
                            })
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
                    width: parent.width - Theme.paddingMedium

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

                Rectangle {
                    visible: model.type !== "Identity" || model.count !== 1
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }
                    height: parent.height/2
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: Theme.rgba("black", 0.0) }
                        GradientStop { position: 0.3; color: Theme.rgba("black", 0.5) }
                        GradientStop { position: 1.0; color: Theme.rgba("black", 0.9) }
                    }
                }

                TextField {
                    id: countField
                    visible: model.type !== "Identity" || model.count !== 1
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }

                    horizontalAlignment: TextInput.AlignHCenter

                    textMargin: 0
                    labelVisible: false

                    placeholderText: "?"
                    placeholderColor: "#ff4d4d"

                    inputMethodHints: Qt.ImhDigitsOnly
                    validator: RegExpValidator { regExp: /^[1-9][0-9]*/ }

                    font.bold: true
                    font.pixelSize: Theme.fontSizeHuge

                    background: null

                    onTextChanged: {
                        if (text != model.count && text > 0)
                        {
                            cardModel.dynamicSortFilter = false
                            model.count = text
                            cardModel.dynamicSortFilter = true
                        }
                    }
                    Binding {
                        target: countField
                        property: "text"
                        value: model.count
                    }

                    EnterKey.iconSource: "image://theme/icon-m-enter-close"
                    EnterKey.onClicked: countField.focus = false
                }

                onClicked: {
                    headerContainer.searchField.focus = false
                    var page = pageStack.push(Qt.resolvedUrl("CardPage.qml"),
                                              {currentIndex: index, cardmodel: cardModel})

                    function repositionView() {
                        if (page.status === PageStatus.Deactivating)
                        {
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
            header: Item {
                id: header
                width: headerContainer.width
                height: headerContainer.height
                Component.onCompleted: headerContainer.parent = header
            }

            ViewPlaceholder {
                enabled: cardListView.count === 0
                verticalOffset: headerContainer.height/2
                text: "No cards in section"
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

                contentHeight: Math.max(delegateRow.height, textColumn.height) + delegateMargin

                menu: ContextMenu {
                    MenuItem {
                        text: "Remove card"
                        onClicked: {
                            listDelegate.remorseAction("Removing card", function () {
                                deckPage.deck.removeCard(id, count)
                            })
                        }
                    }
                }

                MouseArea {
                    id: countArea
                    anchors {
                        left: parent.left
                        top: parent.top
                        bottom: parent.bottom
                    }
                    width: delegateRow.x + delegateRow.width
                    enabled: model.type !== "Identity" || model.count !== 1

                    Row {
                        id: delegateRow
                        x: delegateMargin

                        anchors.verticalCenter: parent.verticalCenter

                        spacing: Theme.paddingMedium

                        TextField {
                            id: countField
                            opacity: (model.type !== "Identity" || model.count !== 1) ? 1 : 0
                            anchors {
                                verticalCenter: parent.verticalCenter
                            }

                            horizontalAlignment: TextInput.AlignHCenter

                            textMargin: 0
                            labelVisible: false

                            placeholderText: "?"
                            placeholderColor: "#ff4d4d"

                            inputMethodHints: Qt.ImhDigitsOnly
                            validator: RegExpValidator { regExp: /^[1-9][0-9]*/ }

                            font.bold: true
                            font.pixelSize: Theme.fontSizeLarge

                            background: null

                            onTextChanged: {
                                if (text != model.count && text > 0)
                                {
                                    cardModel.dynamicSortFilter = false
                                    model.count = text
                                    cardModel.dynamicSortFilter = true
                                }
                            }
                            Binding {
                                target: countField
                                property: "text"
                                value: model.count
                            }

                            EnterKey.iconSource: "image://theme/icon-m-enter-close"
                            EnterKey.onClicked: countField.focus = false
                        }

                        Loader {
                            id: thumbLoader
                            width: active ? listDelegate.width/5 : 0
                            anchors {
                                verticalCenter: parent.verticalCenter
                            }
                            active: thumbnail

                            sourceComponent: Component {
                                Image {
                                    asynchronous: true
                                    fillMode: Image.PreserveAspectFit
                                    source: thumbnail
                                }
                            }
                        }

                    }
                    onClicked: countField.forceActiveFocus()
                }

                Column {
                    id: textColumn
                    anchors {
                        left: countArea.right
                        right: parent.right
                        verticalCenter: parent.verticalCenter

                        leftMargin: Theme.paddingMedium
                        rightMargin: delegateMargin
                    }

                    Label {
                        id: nameLabel
                        width: parent.width
                        wrapMode: Text.WordWrap
                        font.pixelSize: Theme.fontSizeMedium
                        color: listDelegate.highlighted ? Theme.highlightColor : Theme.primaryColor

                        textFormat: Text.StyledText
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
                    var page = pageStack.push(Qt.resolvedUrl("CardPage.qml"),
                                              {currentIndex: index, cardmodel: cardModel})

                    function repositionView() {
                        if (page.status === PageStatus.Deactivating)
                        {
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
