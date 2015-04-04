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
    id: cardPage

    property alias currentIndex: cardView.currentIndex
    property alias cardmodel: cardView.model
    property string preferredView: settings.value("cards/preferredView", "image")
    property bool allowPick: false

    signal picked(string id)

    backNavigation: cardView.interactive

    SilicaFlickable {
        anchors.fill:parent

        PullDownMenu {

            MenuItem {
                text: "Update image"
                enabled: !downloader.busy
                onClicked: {
                    downloader.updateImage(cardView.currentItem.itemData.id)
                }
            }

            MenuItem {
                text: "Switch to " + (preferredView === "image" ? "text" : "image")
                onClicked: {
                    preferredView = preferredView === "image" ? "text" : "image"
                    settings.setValue("cards/preferredView", preferredView)
                }
            }

            MenuItem {
                text: "Add card"
                visible: allowPick
                onClicked: {
                    cardPage.picked(cardView.currentItem.itemData.id)
                }
            }
        }


        SlideshowView {
            id: cardView

            anchors.fill: parent

            delegate: Item {
                property variant itemData: model

                width: cardView.itemWidth
                height: cardView.itemHeight

                CardImage {
                    id: delegateCard
                    width: parent.width
                    height: parent.height
                    source: model.image
                    visible: model.image !== "" && preferredView === "image"

                    onScaledChanged: {
                        if (scaled) {
                            cardView.interactive = false
                        }
                        else {
                            cardView.interactive = true
                        }
                    }
                }

                CardText {
                    visible: !delegateCard.visible
                    anchors.centerIn: parent
                    width: parent.width - 2*Theme.paddingLarge
                }
            }

            Component.onCompleted: {
                positionViewAtIndex(currentIndex, PathView.SnapPosition)
            }

        }

        MouseArea {
            // dummy to allow back navigation instead of
            // prev/next item on top of the page
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            height: Theme.itemSizeHuge
        }
    }
}
