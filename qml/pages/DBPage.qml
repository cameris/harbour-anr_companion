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

Page {
    id: page

    SilicaFlickable {
        anchors.fill: parent

        Column {
            id: column
            width: page.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: "Database"
            }

            Button {
                id: updateDBButton
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Update database"
                enabled: !downloader.busy
                onClicked: {
                    function updateComplete () {
                        dbProgress.visible = false
                        dbProgress.running = false
                        downloader.finished.disconnect(updateComplete)
                    }

                    dbProgress.running = true
                    dbProgress.visible = true

                    downloader.finished.connect(updateComplete)

                    downloader.updateDB()
                }

                BusyIndicator {
                    anchors.horizontalCenter: parent.horizontalCenter
                    id: dbProgress
                    visible: false
                }
            }

            Button {
                id: updateImgButton
                height: imgProgress.height
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Download missing card images"
                enabled: !downloader.busy
                visible: !imgProgress.visible
                onClicked: {
                    imgProgress.visible = true
                    downloader.updateMissingImages()
                }
            }

            ProgressBar {
                id: imgProgress

                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                visible: false
                label: "Downloading card images "
                valueText: Math.round(value*100) + " %"

                function updateValue(val) {
                    if (!visible) {
                        visible = true
                    }
                    value = val
                }

                function finishedDownload() {
                    if (visible) {
                        visible = false
                    }
                }

                Component.onCompleted: {
                    downloader.downloadImgProgress.connect(updateValue)
                    downloader.finished.connect(finishedDownload)
                }
                Component.onDestruction: {
                    downloader.downloadImgProgress.disconnect(updateValue)
                    downloader.finished.disconnect(finishedDownload)
                }
            }
        }
    }
}


